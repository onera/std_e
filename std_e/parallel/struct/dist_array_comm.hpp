#pragma once


#include "std_e/parallel/struct/dist_array.hpp"
#include "std_e/parallel/mpi/one_sided/communication.hpp"


namespace std_e {


template<class T> auto
read(const dist_array<T>& a, MPI_Aint i, T& out) {
  auto [rank,offset] = rank_offset(i,a.distribution());
  return get(a.win(),rank,offset,out);
}

template<
  class T, class Range,
  class = typename Range::value_type // constrain to enable only Ranges
> auto
read(const dist_array<T>& a, MPI_Aint i, Range& out) {
  auto [rank    ,offset    ] = rank_offset(i           ,a.distribution());
  auto [rank_end,offset_end] = rank_offset(i+out.size(),a.distribution());
  STD_E_ASSERT(
       (rank_end == rank)
    || (rank_end == rank+1 && offset_end==0)
  ); // read only works for one rank (else: use gather)
  return get(a.win(),rank,offset,out);
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
