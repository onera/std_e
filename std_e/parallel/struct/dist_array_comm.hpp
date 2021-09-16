#pragma once


#include "std_e/parallel/struct/dist_array.hpp"
#include "std_e/parallel/mpi/one_sided/communication.hpp"


namespace std_e {


template<class T> auto
load(const dist_array<T>& a, MPI_Aint i, T& out) {
  auto [rank,offset] = rank_offset(i,a.distribution());
  return get(a.win(),rank,offset,out);
}

template<
  class T, class Range,
  class = typename Range::value_type // constrain to enable only Ranges
> auto
load(const dist_array<T>& a, int rank, MPI_Aint offset, Range& out) {
  return get(a.win(),rank,offset,out);
}

template<class T, class Int_range, class Range,
  class = typename Int_range::value_type, // constrain to enable only Ranges
  class = typename Range::value_type // constrain to enable only Ranges
> auto
load(const dist_array<T>& a, int rank, const Int_range& ids, Range& out) {
  return get_indexed(a.win().underlying(),rank,ids,out.data());
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
