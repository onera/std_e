#pragma once


#include "std_e/parallel/mpi/one_sided/window.hpp"
#include "std_e/parallel/mpi/datatype/datatype.hpp"


namespace std_e {


template<class T, class I> auto
_get_contiguous(MPI_Win win, int rank, MPI_Aint disp, I size, T* out) -> void {
  int err = MPI_Get(
    out      ,size,to_mpi_type<T>, // origin args
    rank,disp,size,to_mpi_type<T>, // target args
    win
  );
  STD_E_ASSERT(!err);
}

template<class T, class I> auto
_get_indexed(MPI_Win win, int rank, MPI_Datatype target_type, I size, T* out) -> void {
  int err = MPI_Get(
    out    ,size, to_mpi_type<T>, // origin args
    rank, 0,   1, target_type   , // target args
    win
  );
  STD_E_ASSERT(!err);
}


template<class T> auto
get_scalar(const window<T>& win, int rank, MPI_Aint disp, T& out) -> void {
  _get_contiguous(win.underlying(),rank,disp,1,&out);
}

template<class T, class Range> auto
get_contiguous(const window<T>& win, int rank, MPI_Aint disp, Range& out) -> void {
  _get_contiguous(win.underlying(),rank,disp,out.size(),out.data());
}

template<class T, class Int_range> auto
get_indexed(const window<T>& win, int rank, const Int_range& ids, T* out) -> void {
  auto n = ids.size();
  auto target_type = indexed_block<T>(ids);

  _get_indexed(win.underlying(),rank,target_type.underlying(),n,out);
}

template<class T, class Int_range> auto
get_indexed_v(const window<T>& win, int rank, const Int_range& ids, T* out) -> void {
  throw "not implemented";
  // auto n = ids.size();
  // auto target_type = indexed_block<T>(ids);

  // _get_indexed(win.underlying(),rank,target_type.underlying(),n,out);
}


//template<class T> auto
//rget(const window<T>& win, int rank, MPI_Aint disp, T& out) -> MPI_Request {
//  MPI_Request req;
//  MPI_Rget(
//    &out     ,1,to_mpi_type<T>, // origin args
//    rank,disp,1,to_mpi_type<T>, // target args
//    win.underlying(),&req
//  );
//  return req;
//}



} // std_e
