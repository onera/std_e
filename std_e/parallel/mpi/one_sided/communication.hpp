#pragma once


#include "std_e/parallel/mpi/base.hpp"


namespace std_e {


template<class T> auto
get(const window<T>& win, int rank, MPI_Aint disp) {
  T res;
  MPI_Get(
    &res     ,1,to_mpi_type<T>, // origin args
    rank,disp,1,to_mpi_type<T>, // target args
    win.underlying()
  );
  return res;
}
template<class T> auto
rget(const window<T>& win, int rank, MPI_Aint disp, T& res) -> MPI_Request {
  MPI_Request req;
  MPI_Rget(
    &res     ,1,to_mpi_type<T>, // origin args
    rank,disp,1,to_mpi_type<T>, // target args
    win.underlying(),&req
  );
  return req;
}

//template<class T, class Contiguous_range> auto
//get(const window<T>& win, int rank, MPI_Aint disp, Contiguous_range& rng) -> void {
//  MPI_Get(
//    rng.data(),rng.size(),to_mpi_type<T>, // origin args
//    rank,disp ,rng.size(),to_mpi_type<T>, // target args
//    win.underlying()
//  );
//}
//
//template<class T> auto
//get(const window<T>& win, int rank, MPI_Aint disp, int n) -> std::vector<T> {
//  std::vector<T> res(n);
//  get(win,rank,disp,res);
//  return res;
//}


} // std_e
