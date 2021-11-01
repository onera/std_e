#pragma once


#include "std_e/parallel/struct/dist_array.hpp"
#include "std_e/parallel/mpi/one_sided/communication.hpp"
#include "std_e/data_structure/jagged_range.hpp"
#include "std_e/algorithm/partition_sort.hpp"
#include "std_e/algorithm/step.hpp"
#include "std_e/execution/future/future.hpp"
#include "std_e/execution/task.hpp"
#include "std_e/parallel/struct/execution_utils.hpp"


namespace std_e {


template<class T, class Distribution> auto
load(const dist_array<T>& a, const Distribution& distri, MPI_Aint i, T& out) {
  auto [rank,offset] = rank_offset(i,distri);
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

    protocol_get_indexed(const protocol_get_indexed&) = delete;
    protocol_get_indexed& operator=(const protocol_get_indexed&) = delete;
    protocol_get_indexed(protocol_get_indexed&&) = default;
    protocol_get_indexed& operator=(protocol_get_indexed&&) = default;

    auto
    number_of_elements() const -> int {
      return p.number_of_elements();
    }

    template<class T, class Range> auto
    request(const dist_array<T>& a, int rank, Range& out) const -> void {
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

template<class T, class Range> auto
get_protocol_indexed(const dist_array<T>& a, const protocol_rank_get_indexed& protocols_by_rank, Range& out) -> Range& {
  int n_rank = protocols_by_rank.size();
  auto first = out.data();
  for (int i=0; i<n_rank; ++i) {
    int n_i = protocols_by_rank[i].number_of_elements();
    auto out_i = make_span(first,n_i);
    protocols_by_rank[i].request(a,i,out_i);
    first += n_i;
  }
  return out;
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
template<class T, class Distribution, class Int_range, class Range,
  class = typename Range::value_type // constrain to enable only Ranges
> auto
gather_sorted(const dist_array<T>& a, const Distribution& distri, Int_range ids, Range& out) {
  STD_E_ASSERT(ids.size() == out.size());
  STD_E_ASSERT(is_sorted(begin(ids),end(ids)));

  auto partition_is = partition_indices(ids,distri);

  apply_step(ids,partition_is,distri);
  jagged_span<int,2> ins_by_rank(ids,partition_is);

  gather_from_ranks(a,ins_by_rank,out);
}




struct gather_protocol {
  std::vector<int> new_to_old; // permutation
  protocol_rank_get_indexed protocols_by_rank;
};

template<class Distribution, class Int_range> auto
create_gather_protocol(const Distribution& distri, Int_range ids, int type_sz) {
  auto [partition_is,new_to_old] = apply_indirect_partition_sort(ids,distri);
  apply_step(ids,partition_is,distri);
  jagged_span<int,2> ins_by_rank(ids,partition_is);

  auto protocols_by_rank = gather_protocol_from_ranks(ins_by_rank,type_sz);
  return gather_protocol{new_to_old,std::move(protocols_by_rank)};
}

template<class T>
constexpr auto create_gather_protocol_fn = [](const auto& distri, const auto& ids) {
  int type_sz = sizeof(T);
  return create_gather_protocol(distri,ids,type_sz);
};


constexpr auto get_protocol_indexed_fn = [](const auto& a, const auto& protocol, auto& res) {
  return get_protocol_indexed(a,protocol.protocols_by_rank,res);
};

template<class T>
constexpr auto alloc_result_fn = [](const auto& ids) {
  return std::vector<T>(ids.size());
};

constexpr auto apply_perm_fn = [](const auto&, auto& local_array, const auto& protocol) -> auto& {
  auto& new_to_old = protocol.new_to_old;
  inv_permute(local_array,new_to_old);
  return local_array;
};

// TODO protocol to avoid repeated partition
template<class T, class Distribution, class Int_range> auto
gather(future<const dist_array<T>&> a, future<const Distribution&> distri, future<Int_range> ids) -> future<std::vector<T>> {
  auto open = a | then_comm(open_epoch);
  auto protocol = join(distri,ids) | then(create_gather_protocol_fn<T>);
  auto result = ids | then(alloc_result_fn<T>);
  auto launch_win_get_comm = join(open,protocol,result) | then_comm(get_protocol_indexed_fn);
  auto close = join(a,launch_win_get_comm) | then_comm(close_epoch);
  auto final_res = join(close,result,protocol) | then(apply_perm_fn);
  return final_res | then(extract_result_fn);
}

template<class T, class Distribution, class Int_range> auto
gather(const dist_array<T>& a, const Distribution& distri, Int_range ids) -> std::vector<T> {
  task_graph tg;
  future f0 = input_data(tg,a);
  future f1 = input_data(tg,distri);
  future f2 = input_data(tg,std::move(ids));

  future f_res = gather(f0,f1,f2);

  return execute_seq(f_res);
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
