#pragma once


#include "std_e/parallel/struct/dist_array.hpp"
#include "std_e/parallel/mpi/one_sided/communication.hpp"
#include "std_e/data_structure/jagged_range.hpp"
#include "std_e/algorithm/partition_sort.hpp"
#include "std_e/algorithm/step.hpp"


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

template<class T, class Int_range, class Range,
  class = typename Int_range::value_type, // constrain to enable only Ranges
  class = typename Range::value_type // constrain to enable only Ranges
> auto
get_indexed(const dist_array<T>& a, int rank, const Int_range& ins, Range& out) {
  return get_indexed(a.win(),rank,ins,out.data());
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
  ELOG(distri);
  auto [partition_is,new_to_old] = apply_indirect_partition_sort(ids,distri);
  ELOG(ids);
  ELOG(partition_is);
  ELOG(new_to_old);

  apply_step(ids,partition_is,distri);
  ELOG(ids);
  jagged_span<int,2> ins_by_rank(ids,partition_is);

  gather_from_ranks(a,ins_by_rank,out);
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
