#include "std_e/parallel/algorithm/contiguous_numbering.hpp"
#include "std_e/debug.hpp"
#include "std_e/debug_par.hpp"
#include "std_e/parallel/mpi/collective/scan.hpp"
#include "std_e/parallel/algorithm/sort_by_rank.hpp"
#include "std_e/logging/time_logger.hpp"
#include "std_e/future/dynarray.hpp"


namespace std_e {


auto contiguous_numbering_seq(std_e::span<int64_t> xs) -> std::vector<int64_t> {
  int sz = xs.size();
  if (sz == 0) return {};

  int64_t max = *std::max_element(xs.begin(), xs.end());
  std::vector<int64_t> counts(max+1, 0);
  for (int i=0; i<sz; ++i) {
    counts[xs[i]] = 1;
  }
  //ELOG(counts);
  std::partial_sum(counts.begin(), counts.end(), counts.begin());
  //ELOG(counts);

  std::vector<int64_t> res(sz);
  for (int i=0; i<sz; ++i) {
    res[i] = counts[xs[i]];
  }
  //ELOG(res);
  return res;
}

auto contiguous_numbering_core(std_e::span<int64_t> xs) {
  int sz = xs.size();
  if (sz == 0) return std::pair<std::vector<int64_t>,std::vector<int64_t>>{};

  auto min_max = std::minmax_element(xs.begin(), xs.end());
  auto min = *min_max.first;
  auto max = *min_max.second;

  //auto tl0 = std_e::time_logger("maia","core count");
  std::vector<int64_t> counts(max-min+1, 0);
  for (int i=0; i<sz; ++i) {
    counts[xs[i]-min] = 1;
  }
  //tl0.stop();
  //ELOG(counts);
  //auto tl1 = std_e::time_logger("maia","core scan");
  std::partial_sum(counts.begin(), counts.end(), counts.begin());
  //tl1.stop();
  //ELOG(counts);

  //auto tl2 = std_e::time_logger("maia","core reorg");
  std::vector<int64_t> res(sz);
  for (int i=0; i<sz; ++i) {
    res[i] = counts[xs[i]-min];
  }
  //tl2.stop();
  //ELOG(res);
  return std::make_pair(std::move(res), std::move(counts));
}


template<
  class Rng,
  class Proj = identity_closure,
  class Comp = std::less<>,
  class Return_container = interval_vector<int>
> auto
sort_by_rank_uniform(Rng& x, MPI_Comm comm, Proj proj = {}, Comp comp = {}, Return_container&& = {}) -> Return_container {
  using T = typename Rng::value_type;
  using T_piv = value_return_t<Proj,T>;

  auto cmp = [proj](const auto& x, const auto& y){ return proj(x) < proj(y); };
  auto [min_it,max_it] = std::minmax_element(x.begin(), x.end(), cmp);
  int64_t l_min = std_e::get<0>(*min_it);
  int64_t l_max = std_e::get<0>(*max_it);
  int64_t g_min = all_reduce(l_min, MPI_MIN, comm);
  int64_t g_max = all_reduce(l_max, MPI_MAX, comm);
  //ELOG(g_min);
  //ELOG(g_max);
  auto pivots = uniform_intervals_exclude_ends(n_rank(comm), g_min, g_max);
  //ELOG(pivots);

  return pivot_partition_indices(x,pivots,proj,comp,Return_container{});
}

auto contiguous_numbering_sort_hyb(std_e::span<const int64_t> xs_in, MPI_Comm comm) -> std::vector<int64_t> {
  std::vector<int64_t> xs(xs_in.begin(), xs_in.end());
  int sz = xs.size();

  //auto tl0 = std_e::time_logger("maia","sort");
  auto xs_indices = iota_vector(sz, 0);
  //ELOG(xs);
  //ELOG(xs_indices);
  auto mr = std_e::view_as_multi_range(xs,xs_indices);
  auto proj = [](const auto& x){ return get<0>(x); };

  //auto rank_indices = std_e::sort_by_rank(mr,comm,proj,{},1.);
  //auto rank_indices = std_e::sort_by_rank_once(mr,comm,proj);
  auto rank_indices = std_e::sort_by_rank_uniform(mr,comm,proj);
  //ELOG(xs);
  //ELOG(xs_indices);
  //ELOG(rank_indices);
  //tl0.stop();

  //auto tl1 = std_e::time_logger("maia","all to all 1");
  auto [xs_sorted_by_rank,recv_indices] = all_to_all(xs,rank_indices,comm);
  //auto [mr_sorted_by_rank,_] = all_to_all(mr,rank_indices,comm);
  //auto& xs_sorted_by_rank          = std_e::range<0>(mr_sorted_by_rank);
  //auto& xs_indices_sorted_by_rank  = std_e::range<1>(mr_sorted_by_rank);
  //tl1.stop();

  //ELOG(xs_sorted_by_rank);
  //ELOG(recv_indices);

  //auto tl2 = std_e::time_logger("maia","core call + scan");
  auto [partial_res, counts] = contiguous_numbering_core(xs_sorted_by_rank);
  //ELOG(partial_res);
  //auto n_unique = *std::max_element(partial_res.begin(), partial_res.end()); // TODO could be optimized by looking at `counts`
  auto n_unique = counts.back();
  //ELOG(n_unique);
  auto n_unique_acc = ex_scan(n_unique, MPI_SUM, 0, comm);
  //ELOG(n_unique_acc);
  std_e::offset(partial_res, n_unique_acc);
  //ELOG(partial_res);
  //tl2.stop();

  //auto tl3 = std_e::time_logger("maia","all to all 2");
  auto [partial_res_origin,_] = all_to_all(partial_res,recv_indices,comm);
  //ELOG(partial_res_origin);
  //tl3.stop();

  //auto tl4 = std_e::time_logger("maia","final");
  std::vector<int64_t> res(sz);
  for (int i=0; i<sz; ++i) {
    res[xs_indices[i]] = partial_res_origin[i];
  }
  //tl4.stop();
  //ELOG(res);
  return res;
}




auto contiguous_numbering_one_ub(std_e::span<int64_t> x, MPI_Comm comm) -> std::vector<int64_t> {
  int n_part = 1;
  int n_elts = x.size();

  int n_procs = 0;
  MPI_Comm_size(comm, &n_procs);

  int i_proc = 0;
  MPI_Comm_rank(comm,
                &i_proc);

  int *send_buff_n   = (int *) malloc(sizeof(int) * n_procs);
  int *send_buff_idx = (int *) malloc(sizeof(int) * n_procs);

  int *recv_buff_n   = (int *) malloc(sizeof(int) * n_procs);
  int *recv_buff_idx = (int *) malloc(sizeof(int) * n_procs);

  // compute sizes and communicate them
  int32_t n_elt_loc_total = 0;
  int64_t l_max_parent = 0;

  for (int j = 0; j < n_part; j++) {
    n_elt_loc_total += n_elts;
    for (int k = 0; k < n_elts; k++) {
      l_max_parent = std::max (l_max_parent, x[k]);
    }
  }

  int64_t max_parent = 0;
  MPI_Allreduce (&l_max_parent, &max_parent, 1,
                     to_mpi_type<int64_t>, MPI_MAX, comm);

  // compute indices from sizes
  for (int j = 0; j < n_procs; j++) {
    send_buff_n[j]   = 0;
    send_buff_idx[j] = 0;
    recv_buff_n[j]   = 0;
    recv_buff_idx[j] = 0;
  }

  int64_t *d_elt_proc = (int64_t *) malloc(sizeof(int64_t) * (n_procs + 1));


  int64_t div_entiere = max_parent / n_procs;
  int64_t div_reste = max_parent % n_procs;

  d_elt_proc[0] = 1;
  for (int i = 0; i < n_procs; i++) {
    d_elt_proc[i+1] =  div_entiere;
    if (i < div_reste) {
      d_elt_proc[i+1] += 1;
    }
  }

  for (int i = 0; i < n_procs; i++) {
    d_elt_proc[i+1] += d_elt_proc[i];
  }

  std::vector<int> i_elt_procs(n_elts);
  for (int k=0; k<n_elts; ++k) {
    i_elt_procs[k] = interval_index(x[k], std_e::make_span(d_elt_proc, d_elt_proc + n_procs + 1));
  }

  for (int j = 0; j < n_part; j++) {
    for (int k = 0; k < n_elts; k++) {
      const int i_elt_proc = i_elt_procs[k];
      send_buff_n[i_elt_proc] += 1;
    }
  }

  send_buff_idx[0] = 0;
  for (int j = 1; j < n_procs; j++) {
    send_buff_idx[j] = send_buff_idx[j-1] + send_buff_n[j-1];
  }

  /* Determination du nombre d'elements recu de chaque processus */

  MPI_Alltoall(send_buff_n,
               1,
               MPI_INT,
               recv_buff_n,
               1,
               MPI_INT,
               comm);

  recv_buff_idx[0] = 0;
  for(int j = 1; j < n_procs; j++) {
    recv_buff_idx[j] = recv_buff_idx[j-1] + recv_buff_n[j-1];
  }

  // partition numbers then send them to their proc
  int64_t _l_numabs_tmp = d_elt_proc[i_proc+1] - d_elt_proc[i_proc];
  int l_numabs_tmp = (int) _l_numabs_tmp;

  int64_t *numabs_tmp = (int64_t *) malloc(sizeof(int64_t) * l_numabs_tmp);

  int64_t *n_elt_stocke_procs = (int64_t *) malloc(sizeof(int64_t) * (n_procs+1));

  int64_t *send_buff_numabs = (int64_t *) malloc(sizeof(int64_t) *
                                                       n_elt_loc_total);
  int64_t *recv_buff_numabs = (int64_t *) malloc(sizeof(int64_t) *
                                                      (recv_buff_idx[n_procs - 1] +
                                                       recv_buff_n[n_procs - 1]));

  for (int j = 0; j < n_procs; j++) {
    send_buff_n[j] = 0;
  }

  for (int j = 0; j < n_part; j++) {
    for (int k = 0; k < n_elts; k++) {
      const int i_elt_proc = i_elt_procs[k];
      send_buff_numabs[send_buff_idx[i_elt_proc] + send_buff_n[i_elt_proc]] = x[k];
      send_buff_n[i_elt_proc] += 1;
    }
  }

  MPI_Alltoallv((void *) send_buff_numabs,
                send_buff_n,
                send_buff_idx,
                to_mpi_type<int64_t>,
                (void *) recv_buff_numabs,
                recv_buff_n,
                recv_buff_idx,
                to_mpi_type<int64_t>,
                comm);

  /* Echange du nombre d'elements stockes sur chaque processus */

  const int64_t n_elt_stocke =
    (int64_t) (recv_buff_idx[n_procs - 1] + recv_buff_n[n_procs - 1]);

  MPI_Allgather((void *) &n_elt_stocke,
                1,
                to_mpi_type<int64_t>,
                (void *) (n_elt_stocke_procs + 1),
                1,
                to_mpi_type<int64_t>,
                comm);

  n_elt_stocke_procs[0] = 1;
  for (int j = 1; j < n_procs + 1; j++) {
    n_elt_stocke_procs[j] += n_elt_stocke_procs[j-1];
  }

  /* Stockage du resultat et determination de la nouvelle numerotation absolue
     independante du parallelisme */

  for (int j = 0; j < l_numabs_tmp; j++) {
    numabs_tmp[j] = 0;
  }

  for (int j = 0; j < n_procs; j++) {

    const int ideb = recv_buff_idx[j];
    const int ifin = recv_buff_idx[j] + recv_buff_n[j];

    for (int k = ideb; k < ifin; k++) {

      int64_t _idx = recv_buff_numabs[k] - d_elt_proc[i_proc];
      const int idx = (int) _idx;
      assert((idx < l_numabs_tmp) && (idx >= 0));

      numabs_tmp[idx] = 1; /* On marque les elements */
    }
  }

  int64_t cpt_elt_proc = 0;
  for (int j = 0; j < l_numabs_tmp; j++) {
    if (numabs_tmp[j] == 1) {
      cpt_elt_proc += 1;
    }
  }

  /* Mise a jour de n_elt_stocke_procs */

  MPI_Allgather((void *) &cpt_elt_proc,
                    1,
                    to_mpi_type<int64_t>,
                    (void *) (n_elt_stocke_procs + 1),
                    1,
                    to_mpi_type<int64_t>,
                    comm);

  n_elt_stocke_procs[0] = 1;
  for (int j = 1; j < n_procs + 1; j++) {
    n_elt_stocke_procs[j] += n_elt_stocke_procs[j-1];
  }

  /* On fournit une numerotation independante du parallelisme */

  cpt_elt_proc = 0;
  for (int j = 0; j < l_numabs_tmp; j++) {
    if (numabs_tmp[j] == 1) {

      numabs_tmp[j] = n_elt_stocke_procs[i_proc] + cpt_elt_proc;
      cpt_elt_proc += 1;
    }
  }

  // reuse the recv/send buffer to send/recv back data
  cpt_elt_proc = 0;
  for (int j = 0; j < n_procs; j++) {

    const int ideb = recv_buff_idx[j];
    const int ifin = recv_buff_idx[j] + recv_buff_n[j];

    for (int k = ideb; k < ifin; k++) {

      int64_t _idx = recv_buff_numabs[k] - d_elt_proc[i_proc];
      const int idx = (int) _idx;

      recv_buff_numabs[cpt_elt_proc] = numabs_tmp[idx];

      cpt_elt_proc += 1;
    }
  }

  MPI_Alltoallv((void *) recv_buff_numabs,
                    recv_buff_n,
                    recv_buff_idx,
                    to_mpi_type<int64_t>,
                    (void *) send_buff_numabs,
                    send_buff_n,
                    send_buff_idx,
                    to_mpi_type<int64_t>,
                    comm);

  // re-dispatch the result
  for (int j = 0; j < n_procs; j++) {
    send_buff_n[j] = 0;
  }

  std::vector<int64_t> g_nums(n_elts);

  for (int k = 0; k < n_elts; k++) {
    const int i_elt_proc = i_elt_procs[k];
    g_nums[k] = send_buff_numabs[send_buff_idx[i_elt_proc] + send_buff_n[i_elt_proc]];
    send_buff_n[i_elt_proc] += 1;
  }

  free(send_buff_idx);
  free(send_buff_n);
  free(recv_buff_idx);
  free(recv_buff_n);
  free(send_buff_numabs);
  free(recv_buff_numabs);
  free(d_elt_proc);
  free(numabs_tmp);
  free(n_elt_stocke_procs);

  return g_nums;
}






template<
  class RA_rng, class RA_pivot_rng
> constexpr auto
interval_indices(RA_rng& x, const RA_pivot_rng& pivots) {
  int sz = x.size();
  //int n_interval = pivots.n_interval();
  int n_interval = pivots.size()-1;

  std::vector<int> res(sz);
  std::vector<int> interval_sizes(n_interval, 0);

  for (int i=0; i<sz; ++i) {
    int idx = interval_index(x[i], pivots);

    res[i] = idx;
    ++interval_sizes[idx];
  }

  return std::make_pair(std::move(res), std::move(interval_sizes));
}

template<
  class RA_rng, class RA_pivot_rng
> constexpr auto
partition_stable(RA_rng& x, const auto& x_partition, const RA_pivot_rng& partition_starts) {
  int sz = x.size();
  int n_interval = partition_starts.size()-1;

  std::vector<int64_t> res(sz);
  std::vector<int64_t> partition_offsets(n_interval, 0);

  for (int i=0; i<sz; ++i) {
    int i_part = x_partition[i];
    res[partition_starts[i_part] + partition_offsets[i_part]] = x[i];
    ++partition_offsets[i_part];
  }

  return res;
}


template<
  class RA_rng, class RA_pivot_rng
> constexpr auto
partition_stable_inv(RA_rng& x, const auto& x_partition, const RA_pivot_rng& partition_starts) {
  int sz = x.size();
  int n_interval = partition_starts.size()-1;

  std::vector<int64_t> res(sz);
  std::vector<int64_t> partition_offsets(n_interval, 0);

  for (int i=0; i<sz; ++i) {
    int i_part = x_partition[i];
    res[i] = x[partition_starts[i_part] + partition_offsets[i_part]];
    ++partition_offsets[i_part];
  }

  return res;
}




template<class I> auto
uniform_intervals2(int n_interval, I start, I size_tot) {
  dynarray<I> iv(n_interval+1);
  uniform_intervals(begin(iv),end(iv),start,size_tot);
  return iv;
}
auto contiguous_numbering_core2(std_e::span<int64_t> xs) {
  int sz = xs.size();
  if (sz == 0) return std::pair<dynarray<int64_t>,std::vector<int64_t>>{};

  auto min_max = std::minmax_element(xs.begin(), xs.end());
  auto min = *min_max.first;
  auto max = *min_max.second;

  //auto tl0 = std_e::time_logger("maia","core count");
  std::vector<int64_t> counts(max-min+1, 0);
  for (int i=0; i<sz; ++i) {
    counts[xs[i]-min] = 1;
  }
  //tl0.stop();
  //ELOG(counts);
  //auto tl1 = std_e::time_logger("maia","core scan");
  std::partial_sum(counts.begin(), counts.end(), counts.begin());
  //tl1.stop();
  //ELOG(counts);

  //auto tl2 = std_e::time_logger("maia","core reorg");
  dynarray<int64_t> res(sz);
  for (int i=0; i<sz; ++i) {
    res[i] = counts[xs[i]-min];
  }
  //tl2.stop();
  //ELOG(res);
  return std::make_pair(std::move(res), std::move(counts));
}
template<
  class RA_rng, class RA_pivot_rng
> constexpr auto
interval_indices2(RA_rng& x, const RA_pivot_rng& pivots) {
  int sz = x.size();
  int n_interval = pivots.size()-1;

  dynarray<int> res(sz);
  std::vector<int> interval_sizes(n_interval+1, 0); // TODO why +1 ?

  for (int i=0; i<sz; ++i) {
    int idx = interval_index(x[i], pivots);

    res[i] = idx;
    ++interval_sizes[idx];
  }

  return std::make_pair(std::move(res), std::move(interval_sizes));
}
template<
  class RA_rng, class RA_pivot_rng
> constexpr auto
partition_stable2(RA_rng& x, const auto& x_partition, const RA_pivot_rng& partition_starts, auto& partition_offsets_buffer) {
  int sz = x.size();
  int n_interval = partition_starts.size()-1;

  dynarray<int64_t> res(sz);
  //std::vector<int64_t> partition_offsets(n_interval, 0);
  for (int j = 0; j < n_interval; j++) {
    partition_offsets_buffer[j] = 0;
  }

  for (int i=0; i<sz; ++i) {
    int i_part = x_partition[i];
    res[partition_starts[i_part] + partition_offsets_buffer[i_part]] = x[i];
    ++partition_offsets_buffer[i_part];
  }

  return res;
}
template<
  class RA_rng, class RA_pivot_rng
> constexpr auto
partition_stable_inv2(RA_rng& x, const auto& x_partition, const RA_pivot_rng& partition_starts, auto& partition_offsets_buffer) {
  int sz = x.size();
  int n_interval = partition_starts.size()-1;

  std::vector<int64_t> res(sz); // TODO dynarray
  //std::vector<int64_t> partition_offsets(n_interval, 0);
  for (int j = 0; j < n_interval; j++) {
    partition_offsets_buffer[j] = 0;
  }

  for (int i=0; i<sz; ++i) {
    int i_part = x_partition[i];
    res[i] = x[partition_starts[i_part] + partition_offsets_buffer[i_part]];
    ++partition_offsets_buffer[i_part];
  }

  return res;
}
auto contiguous_numbering_alltoall6(std_e::span<int64_t> x, MPI_Comm comm) -> std::vector<int64_t> {
  int n_elts = x.size();
  if (n_elts == 0) return {};

  int i_proc = std_e::rank(comm);
  int n_procs = std_e::n_rank(comm);

  // compute sizes and communicate them
  int64_t l_max_parent = *std::max_element(x.begin(), x.end());
  int64_t min_parent = 1;
  int64_t max_parent = all_reduce(l_max_parent, MPI_MAX, comm);

  auto d_elt_proc = uniform_intervals2(n_procs, min_parent, max_parent-min_parent+1);

  auto [i_elt_procs, send_buff_n] = interval_indices2(x,d_elt_proc);

  dynarray<int> recv_buff_n(n_procs);
  all_to_all(send_buff_n.data(), 1,
             recv_buff_n.data(),
             comm
  );

  // compute indices from sizes
  dynarray<int> send_buff_idx(n_procs+1);
  dynarray<int> recv_buff_idx(n_procs+1);
  send_buff_idx[0] = 0;
  recv_buff_idx[0] = 0;
  for (int j = 0; j < n_procs; ++j) {
    send_buff_idx[j+1] = send_buff_idx[j] + send_buff_n[j];
    recv_buff_idx[j+1] = recv_buff_idx[j] + recv_buff_n[j];
  }
  // partition numbers then send them to their proc
  dynarray<int64_t> send_buff_numabs = partition_stable2(x, i_elt_procs, send_buff_idx, send_buff_n);

  dynarray<int64_t> recv_buff_numabs( recv_buff_idx[n_procs-1] + recv_buff_n[n_procs-1] );
  all_to_all_v(
    send_buff_numabs.data(), send_buff_n.data(), send_buff_idx.data(),
    recv_buff_numabs.data(), recv_buff_n.data(), recv_buff_idx.data(),
    comm
  );

  // new numbering
  int64_t _l_numabs_tmp = d_elt_proc[i_proc+1] - d_elt_proc[i_proc];
  int l_numabs_tmp = (int) _l_numabs_tmp;
  std::vector<int64_t> numabs_tmp(l_numabs_tmp, 0);

  for (int i = 0; i < recv_buff_idx[n_procs-1] + recv_buff_n[n_procs-1]; ++i) {
    numabs_tmp[recv_buff_numabs[i] - d_elt_proc[i_proc]] = 1;
  }

  std::partial_sum(numabs_tmp.begin(), numabs_tmp.end(), numabs_tmp.begin());
  auto n_unique = numabs_tmp.back();
  auto n_unique_acc = ex_scan(n_unique, MPI_SUM, 0, comm);
  std_e::offset(numabs_tmp, n_unique_acc);

  // reuse the recv/send buffer to send/recv back data
  for (int i = 0; i < recv_buff_idx[n_procs-1] + recv_buff_n[n_procs-1]; ++i) {
    recv_buff_numabs[i] = numabs_tmp[recv_buff_numabs[i] - d_elt_proc[i_proc]];
  }

  all_to_all_v(
      recv_buff_numabs.data(), recv_buff_n.data(), recv_buff_idx.data(),
      send_buff_numabs.data(), send_buff_n.data(), send_buff_idx.data(),
      comm
  );

  // re-dispatch the result
  return partition_stable_inv2(send_buff_numabs, i_elt_procs, send_buff_idx, send_buff_n);
}

auto contiguous_numbering(std_e::span<int64_t> x, MPI_Comm comm, const std::string& strategy) -> std::vector<int64_t> {
  if (strategy == "sort hyb")     return contiguous_numbering_sort_hyb (x, comm);
  if (strategy == "one ub")       return contiguous_numbering_one_ub   (x, comm);
  if (strategy == "alltoall6")    return contiguous_numbering_alltoall6(x, comm);
  throw std_e::msg_exception("unknown \""+ strategy + "\" strategy");
}


} // std_e
