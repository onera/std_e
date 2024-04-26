#include "std_e/parallel/algorithm/continuous_numbering.hpp"
#include "std_e/debug.hpp"
#include "std_e/parallel/mpi/collective/scan.hpp"
#include "std_e/parallel/algorithm/sort_by_rank.hpp"


namespace std_e {


auto continuous_numbering_seq(std_e::span<int64_t> xs) -> std::vector<int64_t> {
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

auto continuous_numbering_core(std_e::span<int64_t> xs) -> std::vector<int64_t> {
  int sz = xs.size();
  if (sz == 0) return {};

  auto min_max = std::minmax_element(xs.begin(), xs.end());
  auto min = *min_max.first;
  auto max = *min_max.second;

  std::vector<int64_t> counts(max-min+1, 0);
  for (int i=0; i<sz; ++i) {
    counts[xs[i]-min] = 1;
  }
  //ELOG(counts);
  std::partial_sum(counts.begin(), counts.end(), counts.begin());
  //ELOG(counts);

  std::vector<int64_t> res(sz);
  for (int i=0; i<sz; ++i) {
    res[i] = counts[xs[i]-min];
  }
  //ELOG(res);
  return res;
}

auto continuous_numbering(std_e::span<int64_t> xs, MPI_Comm comm) -> std::vector<int64_t> {
  int sz = xs.size();

  auto xs_indices = iota_vector(sz, 0);
  //ELOG(xs);
  //ELOG(xs_indices);
  auto mr = std_e::view_as_multi_range(xs,xs_indices);
  auto proj = [](const auto& x){ return get<0>(x); };

  auto rank_indices = std_e::sort_by_rank(mr,comm,proj);
  LOG("after sort by rank");
  //ELOG(xs);
  //ELOG(xs_indices);
  //ELOG(rank_indices);

  auto [xs_sorted_by_rank,recv_indices] = all_to_all(xs,rank_indices,comm);
  //auto [mr_sorted_by_rank,_] = all_to_all(mr,rank_indices,comm);
  //auto& xs_sorted_by_rank          = std_e::range<0>(mr_sorted_by_rank);
  //auto& xs_indices_sorted_by_rank  = std_e::range<1>(mr_sorted_by_rank);

  LOG("after all to all 1");
  //ELOG(xs_sorted_by_rank);
  //ELOG(recv_indices);

  LOG("after continuous_numbering_core");
  auto partial_res = continuous_numbering_core(xs_sorted_by_rank);
  //ELOG(partial_res);
  auto n_unique = *std::max_element(partial_res.begin(), partial_res.end()); // TODO could be optimized by looking at counts from continuous_numbering4
  //ELOG(n_unique);
  auto n_unique_acc = ex_scan(n_unique, MPI_SUM, 0, comm);
  //ELOG(n_unique_acc);
  std_e::offset(partial_res, n_unique_acc);
  //ELOG(partial_res);

  auto [partial_res_origin,_] = all_to_all(partial_res,recv_indices,comm);
  LOG("after all to all 2");
  //ELOG(partial_res_origin);

  std::vector<int64_t> res(sz);
  for (int i=0; i<sz; ++i) {
    res[xs_indices[i]] = partial_res_origin[i];
  }
  LOG("after permutation");
  //ELOG(res);
  return res;
}


} // std_e
