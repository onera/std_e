#pragma once


#include "std_e/parallel/struct/dist_array.hpp"
#include "std_e/parallel/struct/dist_jagged_array.hpp"
#include "std_e/parallel/mpi/one_sided/communication.hpp"
#include "std_e/data_structure/jagged_range.hpp"
#include "std_e/algorithm/partition_sort.hpp"
#include "std_e/algorithm/step.hpp"
#include "std_e/execution/future/future.hpp"
#include "std_e/execution/task.hpp"
#include "std_e/parallel/struct/execution_utils.hpp"


namespace std_e {


class indexed_exchange_protocol {
  private:
    protocol_win_get_indexed p;
  public:
    indexed_exchange_protocol() = default;

    template<class Int_range>
    indexed_exchange_protocol(const Int_range& ins, int type_sz)
      : p(ins,type_sz)
    {}

    indexed_exchange_protocol(const indexed_exchange_protocol&) = delete;
    indexed_exchange_protocol& operator=(const indexed_exchange_protocol&) = delete;
    indexed_exchange_protocol(indexed_exchange_protocol&&) = default;
    indexed_exchange_protocol& operator=(indexed_exchange_protocol&&) = default;

    auto
    number_of_elements() const -> int {
      return p.number_of_elements();
    }

    template<class T, class Range> auto
    gather(const dist_array<T>& a, int rank, Range& out) const -> void {
      p.request(a.win(),rank,out);
    }
};

class indexed_exchange_protocol_var_len {
  private:
    protocol_win_get_indexed_var_len p;
  public:
    indexed_exchange_protocol_var_len() = default;

    template<class Int_range, class Int_contiguous_range>
    indexed_exchange_protocol_var_len(const Int_range& ins, const Int_contiguous_range& strides, const Int_contiguous_range& displs, int type_sz)
      : p(ins,strides,displs,type_sz)
    {}

    indexed_exchange_protocol_var_len(const indexed_exchange_protocol_var_len&) = delete;
    indexed_exchange_protocol_var_len& operator=(const indexed_exchange_protocol_var_len&) = delete;
    indexed_exchange_protocol_var_len(indexed_exchange_protocol_var_len&&) = default;
    indexed_exchange_protocol_var_len& operator=(indexed_exchange_protocol_var_len&&) = default;

    auto
    number_of_elements() const -> int {
      return p.number_of_elements();
    }

    template<class T, class Range> auto
    gather(const dist_array<T>& a, int rank, Range& out) const -> void {
      ELOG(out.size());
      p.request(a.win(),rank,out);
    }
};

using exchange_protocol_by_rank = std::vector<indexed_exchange_protocol>;
using exchange_protocol_by_rank_var_len = std::vector<indexed_exchange_protocol_var_len>;

template<class TR, class IR> auto
create_exchange_protocol_from_ranks(const jagged_range<TR,IR,2>& indices_by_rank, int type_sz) {
  int n_rank = indices_by_rank.n_interval();
  exchange_protocol_by_rank protocols_by_rank(n_rank);
  for (int i=0; i<n_rank; ++i) {
    const auto& ins = indices_by_rank[i];
    protocols_by_rank[i] = indexed_exchange_protocol(ins,type_sz);
  }
  return protocols_by_rank;
}
template<class TR, class IR, class Int_contiguous_range> auto
create_exchange_protocol_from_ranks(const jagged_range<TR,IR,2>& indices_by_rank, const Int_contiguous_range& strides, const std::vector<int>& displs, int type_sz) {
  int n_rank = indices_by_rank.n_interval();
  exchange_protocol_by_rank_var_len protocols_by_rank(n_rank);
  int offset_strides = 0; // TODO ugly
  for (int i=0; i<n_rank; ++i) {
    const auto& ins = indices_by_rank[i];
    auto strides_i = make_span(strides.data()+offset_strides,strides.data()+offset_strides+ins.size());
    auto displs_i = make_span(displs.data()+offset_strides,displs.data()+offset_strides+ins.size());
    offset_strides += ins.size();
    protocols_by_rank[i] = indexed_exchange_protocol_var_len(ins,strides_i,displs_i,type_sz);
  }
  return protocols_by_rank;
}

template<class T, class Range> auto
get_protocol_indexed2(const jagged_vector<int,2>& indices_by_rank, const dist_array<T>& a, Range& out) -> Range& {
  int type_sz = sizeof(T);
  auto protocols_by_rank = create_exchange_protocol_from_ranks(indices_by_rank,type_sz);

  int n_rank = protocols_by_rank.size();
  auto first = out.data();
  for (int i=0; i<n_rank; ++i) {
    int n_i = protocols_by_rank[i].number_of_elements();
    auto out_i = make_span(first,n_i);
    protocols_by_rank[i].gather(a,i,out_i);
    first += n_i;
  }
  return out;
}
template<class T, class Range> auto
get_protocol_indexed2(const jagged_vector<int,2>& indices_by_rank,
                      const std::vector<int>& strides, const std::vector<int>& displacements,
                      const dist_array<T>& a, Range& out) -> Range& {
  int type_sz = sizeof(T);
  auto protocols_by_rank = create_exchange_protocol_from_ranks(indices_by_rank,strides,displacements,type_sz);

  int n_rank = protocols_by_rank.size();
  auto first = out.data();
  for (int i=0; i<n_rank; ++i) {
    int n_i = protocols_by_rank[i].number_of_elements();
    auto out_i = make_span(first,n_i);
    protocols_by_rank[i].gather(a,i,out_i);
    first += n_i;
  }
  return out;
}



struct exchange_protocol {
  jagged_vector<int,2> indices_by_rank;
  std::vector<int> new_to_old; // permutation
};

template<class Distribution, class Int_range> auto
create_exchange_protocol2(const Distribution& distri, Int_range ids) {
  auto [partition_is,new_to_old] = apply_indirect_partition_sort(ids,distri);
  apply_step(ids,partition_is,distri);
  jagged_vector<int,2> indices_by_rank(std::move(ids),std::move(partition_is));

  return exchange_protocol{std::move(indices_by_rank),std::move(new_to_old)};
}

constexpr auto create_exchange_protocol_fn = [](const auto& distri, const auto& ids) {
  return create_exchange_protocol2(distri,ids);
};



// TODO rename
constexpr auto get_protocol_indexed_fn2 = []<class T>(const exchange_protocol& gp, const dist_array<T>& a, std::vector<T>& res) {
  auto xx = get_protocol_indexed2(gp.indices_by_rank,a,res);
  ELOG(xx);
  return xx;
};
constexpr auto get_protocol_indexed_var_len_fn =
  []<class T>(const exchange_protocol& gp,
              const std::vector<int>& strides, const std::vector<int>& displacements,
              const dist_array<T>& a, std::vector<T>& res,
              auto&&...) // futures that must complete but whose return value is not needed (TODO join(...) | and_complete | then(...))
{
  LOG(" res size  = ",res.size());
  return get_protocol_indexed2(gp.indices_by_rank,strides,displacements,a,res);
};


template<class T>
constexpr auto alloc_result_fn2 = [](const exchange_protocol& gp) {
  return std::vector<T>(gp.new_to_old.size());
};

constexpr auto apply_perm_fn2 = [](const exchange_protocol& gp, auto& local_array, auto&&) -> auto& {
  inv_permute(local_array,gp.new_to_old);
  return local_array;
};
constexpr auto perm_fn2 = [](const exchange_protocol& gp, auto local_array, auto&&) -> auto { // Note: here, local_array is copied
  ELOG(local_array);
  inv_permute(local_array,gp.new_to_old);
  ELOG(local_array);
  return local_array;
};

constexpr auto perm_fn3 = []<class T>(const exchange_protocol& gp, const auto& indices, const auto& indices_new, const std::vector<T>& local_array, auto&&) {
  ELOG(gp.new_to_old);
  ELOG(indices);
  ELOG(indices_new);
  ELOG(local_array);
  STD_E_ASSERT(gp.new_to_old.size()==indices.size()-1);
  STD_E_ASSERT(indices.back()==(int)local_array.size());
  std::vector<T> perm_local_array(local_array.size());
  int n = gp.new_to_old.size();
  auto old_to_new = inverse_permutation(gp.new_to_old);
  ELOG(old_to_new);
  for (int i=0; i<n; ++i) {
    ELOG(i);
    int old_j    = indices[old_to_new[i]  ];
    int old_j_p1 = indices[old_to_new[i]+1];
    int n_i = old_j_p1-old_j;
    ELOG(old_j);
    ELOG(n_i);
    for (int k=0; k<n_i; ++k) {
      perm_local_array[indices_new[i]+k] = local_array[old_j+k];
    }
  }
  ELOG(perm_local_array);
  return perm_local_array;
};




template<class Distribution, class Int_range> auto
create_exchange_protocol(future<const Distribution&> distri, future<Int_range> ids) -> future<exchange_protocol> {
  return join(distri,ids) | then(create_exchange_protocol_fn);
}


template<class T> auto
gather(future<const exchange_protocol&> gp, future<dist_array<T>&> a) -> future<std::vector<T>> {
  auto a_open = a | then_comm(open_epoch); // TODO RENAME open_a
  auto result = gp| then(alloc_result_fn2<T>);
  auto result_filled = join(gp,a_open,result) | then_comm(get_protocol_indexed_fn2); // TODO then_comm -> split into computation (then) and comm (then_comm)
  auto a_close = join(a,result_filled) | then_comm(close_epoch); // TODO RENAME close_a
  auto final_res = join(gp,result,a_close) | then(apply_perm_fn2);
  return final_res | then(extract_result_fn);
}


constexpr  auto get_displacements_fn = []<class T>(dist_jagged_array<T>& arr) -> auto& {
  return arr.displacements_dist_array();
};
constexpr  auto get_strides_fn = []<class T>(dist_jagged_array<T>& arr) -> auto& {
  return arr.strides_dist_array();
};
constexpr  auto get_values_fn = []<class T>(dist_jagged_array<T>& arr) -> auto& {
  return arr.values_dist_array();
};

constexpr  auto indices_from_strides_fn = [](std::vector<int>& stris, auto&&...) -> interval_vector<int> {
  return indices_from_strides(stris);
};

template<class T>
constexpr auto alloc_res_val_fn = [](const interval_vector<int>& iv) {
  ELOG(iv);
  ELOG(iv.length());
  return std::vector<T>(iv.length());
};

constexpr auto extract_res_in_jagged_array_fn = []<class T>(interval_vector<int>& displs, std::vector<T>& values) {
  ELOG(displs);
  ELOG(values);
  return jagged_vector<T>(std::move(values),std::move(displs));
};

template<class T> auto
gather(future<const exchange_protocol&> gp, future<dist_jagged_array<T>&> a) { //-> future<jagged_vector<T>> {
  auto displs  = a | then(get_displacements_fn);
  auto strides = a | then(get_strides_fn);
  auto values  = a | then(get_values_fn);

  // gather strides TODO factorize with gather(gp,a)
  auto strides_open = strides | then_comm(open_epoch);
  auto strides_res = gp| then(alloc_result_fn2<int>);
  //return strides_res;
  auto strides_res_filled = join(gp,strides_open,strides_res) | then_comm(get_protocol_indexed_fn2);
  //return strides_res_filled;
  auto strides_close = join(strides,strides_res_filled) | then_comm(close_epoch);

  // gather displs TODO factorize with gather(gp,a)
  auto displs_open = displs | then_comm(open_epoch);
  auto displs_res = gp| then(alloc_result_fn2<int>);
  //return displs_res;
  auto displs_res_filled = join(gp,displs_open,displs_res) | then_comm(get_protocol_indexed_fn2);
  //return displs_res_filled;
  auto displs_close = join(displs,displs_res_filled) | then_comm(close_epoch);

  // allocate result values
  auto val_displs_res = join(strides_res,strides_close) | then(indices_from_strides_fn);
  auto val_res = val_displs_res | then(alloc_res_val_fn<T>);

  auto perm_strides_res = join(gp,strides_res,strides_close) | then(perm_fn2);
  auto final_displs_res = perm_strides_res | then(indices_from_strides_fn);
  //auto val_res = final_displs_res | then(alloc_res_val_fn<T>);

  // gather values
  auto values_open = values | then_comm(open_epoch);
  auto val_res_filled =
    join(gp,strides_res,displs_res,values_open,val_res,
    /*complete*/ strides_close,displs_close) | then_comm(get_protocol_indexed_var_len_fn);
  auto values_close = join(values,val_res_filled) | then_comm(close_epoch);
  //auto final_res = join(gp,result,values_close) | then(apply_perm_fn2);
  //return perm_strides_res;
  auto final_vals = join(gp,val_displs_res,final_displs_res,val_res, values_close) | then(perm_fn3);
  return join(final_displs_res,final_vals) | then(extract_res_in_jagged_array_fn);
}


} // std_e
