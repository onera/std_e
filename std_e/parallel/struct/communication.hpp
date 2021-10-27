#pragma once


#include "std_e/parallel/struct/dist_array.hpp"
#include "std_e/parallel/mpi/one_sided/communication.hpp"
#include "std_e/data_structure/jagged_range.hpp"
#include "std_e/algorithm/partition_sort.hpp"
#include "std_e/algorithm/step.hpp"
#include "std_e/execution/task_graph/task_graph_handle.hpp"
#include "std_e/execution/task.hpp"


namespace std_e {


template<class T> auto
load(const dist_array<T>& a, MPI_Aint i, T& out) {
  auto [rank,offset] = rank_offset(i,a.distribution());
  return get_scalar(a.win(),rank,offset,out);
}

template<class T> auto
get_scalar(const dist_array<T>& a, int rank, MPI_Aint offset, T& out) {
  return get_scalar(a.win(),rank,offset,out);
}

template<class T, class Range> auto
get_contiguous(const dist_array<T>& a, int rank, MPI_Aint offset, Range& out) {
  return get_contiguous(a.win(),rank,offset,out);
}

class protocol_get_indexed {
  private:
    protocol_win_get_indexed p;
  public:
    protocol_get_indexed() = default;

    template<class Int_range>
    protocol_get_indexed(const Int_range& ins, int type_sz)
      : p(ins,type_sz)
    {}

    auto
    number_of_elements() const -> int {
      return p.number_of_elements();
    }

    template<class T, class Range> auto
    request(const dist_array<T>& a, int rank, Range& out) -> void {
      p.request(a.win(),rank,out);
    }
};
template<class T, class Int_range, class Range> auto
get_indexed(const dist_array<T>& a, int rank, const Int_range& ins, Range& out) {
  return protocol_get_indexed(ins,sizeof(T)).request(a,rank,out);
}


using protocol_rank_get_indexed = std::vector<protocol_get_indexed>;

template<class TR, class IR> auto
gather_protocol_from_ranks(const jagged_range<TR,IR,2>& ins_by_rank, int type_sz) {
  int n_rank = ins_by_rank.n_interval();
  protocol_rank_get_indexed protocols_by_rank(n_rank);
  for (int i=0; i<n_rank; ++i) {
    const auto& ins = ins_by_rank[i];
    protocols_by_rank[i] = protocol_get_indexed(ins,type_sz);
  }
  return protocols_by_rank;
}

template<class T, class Int_range, class Range> auto
get_protocol_indexed(const dist_array<T>& a, const protocol_rank_get_indexed& protocols_by_rank, Range& out) {
  int n_rank = protocols_by_rank.size();
  auto first = out.data();
  for (int i=0; i<n_rank; ++i) {
    int n_i = protocols_by_rank[i].number_of_elements();
    auto out_i = make_span(first,n_i);
    protocols_by_rank[i].request(a,rank,out_i);
    first += n_i;
  }
}

// TODO protocol to avoid creation of MPI indexed types
template<class T, class TR, class IR, class Range,
  class = typename Range::value_type // constrain to enable only Ranges
> auto
gather_from_ranks(const dist_array<T>& a, const jagged_range<TR,IR,2>& ins_by_rank, Range& out) {
  STD_E_ASSERT(ins_by_rank.n_elt() == (int)out.size());
  STD_E_ASSERT(ins_by_rank.n_interval() == a.n_rank());
  int n_rank = a.n_rank();
  auto first = out.data();
  for (int i=0; i<n_rank; ++i) {
    const auto& ins = ins_by_rank[i];
    int n_i = ins.size();
    auto out_i = make_span(first,n_i);
    get_indexed(a,i,ins,out_i);
    first += n_i;
  }
}

// TODO protocol to avoid repeated copy and shift
template<class T, class Int_range, class Range,
  class = typename Range::value_type // constrain to enable only Ranges
> auto
gather_sorted(const dist_array<T>& a, Int_range ids, Range& out) {
  STD_E_ASSERT(ids.size() == out.size());
  STD_E_ASSERT(is_sorted(begin(ids),end(ids)));

  const auto& distri = a.distribution();
  auto partition_is = partition_indices(ids,distri);

  apply_step(ids,partition_is,distri);
  jagged_span<int,2> ins_by_rank(ids,partition_is);

  gather_from_ranks(a,ins_by_rank,out);
}


// TODO protocol to avoid repeated partition
template<class T, class Int_range, class Range,
  class = typename Range::value_type // constrain to enable only Ranges
> auto
gather(const dist_array<T>& a, Int_range ids, Range& out) {
  STD_E_ASSERT(ids.size() == out.size());

  const auto& distri = a.distribution();
  //ELOG(distri);
  auto [partition_is,new_to_old] = apply_indirect_partition_sort(ids,distri);
  //ELOG(ids);
  //ELOG(partition_is);
  //ELOG(new_to_old);

  apply_step(ids,partition_is,distri);
  //ELOG(ids);
  jagged_span<int,2> ins_by_rank(ids,partition_is);

  gather_from_ranks(a,ins_by_rank,out);
}


struct gather_protocol {
  std::vector<int> new_to_old; // permutation
  protocol_rank_get_indexed protocols_by_rank;
};

template<class dist_type, class Int_range> auto // TODO replace dist_type by ~Distribution
create_gather_protocol(const dist_type& distri, Int_range ids, int type_sz) {
  auto [partition_is,new_to_old] = apply_indirect_partition_sort(ids,distri);
  apply_step(ids,partition_is,distri);
  jagged_span<int,2> ins_by_rank(ids,partition_is);

  auto protocols_by_rank = gather_protocol_from_ranks(ins_by_rank,type_sz);
  return gather_protocol{new_to_old,protocols_by_rank};
}

constexpr auto create_gather_protocol_fn = [](auto t) { // t is tuple<dist_array<T>,Int_range>
  using dist_array_type = std::tuple_element_t<0,decltype(t)>;
  using T = typename dist_array_type::value_type;
  int type_sz = sizeof(T);
  return create_gather_protocol<T>(std::get<0>(t).distribution(),std::get<1>(t),type_sz);
};

constexpr auto open_epoch = []<class T>(dist_array<T>& a) -> dist_array<T>& {
  int assertion = 0;
  int err = MPI_Win_lock_all(assertion,a.win());
  STD_E_ASSERT(!err);
  return a;
};
constexpr auto close_epoch = []<class T>(dist_array<T>& a) -> dist_array<T>& {
  int err = MPI_Win_unlock_all(a.win());
  STD_E_ASSERT(!err);
  return a;
};

template<class T> using future = task_graph_handle<T>;


constexpr auto get_protocol_indexed_fn = [](auto t) {
  return get_protocol_indexed(std::get<0>(t),std::get<1>(t).protocols_by_rank,std::get<2>(t));
};

constexpr auto alloc_result_fn = []<class T>(auto ids) {
  return std::vector<T>(ids.size());
};

constexpr auto apply_perm_fn = [](auto t) {
  auto& local_array = std::get<0>(t);
  auto& new_to_old = std::get<1>(t).new_to_old;
  return inv_permute(local_array,new_to_old);
};

constexpr auto extract_result_fn = [](auto t) {
  return std::move(std::get<1>(t));
};

template<class T, class Int_range> auto
gather(const future<dist_array<T>>& a, const future<Int_range>& ids) -> future<std::vector<T>> {
  auto open = a | then_comm(open_epoch);
  auto protocol = join(a,ids) | then(create_gather_protocol_fn);
  auto result = ids | then(alloc_result_fn);
  auto launch_win_get_comm = join(open,protocol,result) | then_comm(get_protocol_indexed_fn);
  auto close = join(a,launch_win_get_comm) | then_comm(close_epoch);
  auto reorder = join(close,protocol) | then(apply_perm_fn);
  return join(reorder,result) | then(extract_result_fn);
}

//template<class T> auto
//get_now(const dist_array<T>& a, MPI_Aint i) -> T {
//  auto [rank,offset] = rank_offset(i,a.distribution());
//  T res;
//  MPI_Request req = rget(a.win(),rank,offset,res);
//  MPI_Status status;
//  MPI_Wait(&req,&status);
//  return res;
//}


} // std_e
