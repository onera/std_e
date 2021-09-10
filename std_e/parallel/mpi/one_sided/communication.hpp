#pragma once


#include "std_e/parallel/mpi/one_sided/window.hpp"


namespace std_e {


template<class T, class I> auto
get(MPI_Win win, int rank, MPI_Aint disp, T* out, I n) -> void {
  MPI_Get(
    out      ,n,to_mpi_type<T>, // origin args
    rank,disp,n,to_mpi_type<T>, // target args
    win
  );
}

template<class T> auto
get(const window<T>& win, int rank, MPI_Aint disp, T& out) -> void {
  return get(win.underlying(),rank,disp,&out,1);
}
template<class T, class Range> auto
get(const window<T>& win, int rank, MPI_Aint disp, Range& out) -> void {
  return get(win.underlying(),rank,disp,out.data(),out.size());
}



template<class T> auto
rget(const window<T>& win, int rank, MPI_Aint disp, T& out) -> MPI_Request {
  MPI_Request req;
  MPI_Rget(
    &out     ,1,to_mpi_type<T>, // origin args
    rank,disp,1,to_mpi_type<T>, // target args
    win.underlying(),&req
  );
  return req;
}



} // std_e
