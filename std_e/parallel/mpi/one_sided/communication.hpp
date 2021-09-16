#pragma once


#include "std_e/parallel/mpi/one_sided/window.hpp"


namespace std_e {


template<class T, class I> auto
get(MPI_Win win, int rank, MPI_Aint disp, T* out, I n) -> void {
  int err = MPI_Get(
    out      ,n,to_mpi_type<T>, // origin args
    rank,disp,n,to_mpi_type<T>, // target args
    win
  );
  STD_E_ASSERT(err == MPI_SUCCESS);
}

template<class T> auto
get(const window<T>& win, int rank, MPI_Aint disp, T& out) -> void {
  return get(win.underlying(),rank,disp,&out,1);
}
template<class T, class Range> auto
get(const window<T>& win, int rank, MPI_Aint disp, Range& out) -> void {
  return get(win.underlying(),rank,disp,out.data(),out.size());
}


template<class T, class Int_range> auto
get_indexed(MPI_Win win, int rank, const Int_range& ids, T* out) -> void {
  using I = typename Int_range::value_type;
  int err;
  I n = ids.size();

  MPI_Datatype target_type;
  err = MPI_Type_create_indexed_block(n, 1, ids.data(), to_mpi_type<T>, &target_type);
  STD_E_ASSERT(err == MPI_SUCCESS);

  err = MPI_Type_commit(&target_type);
  STD_E_ASSERT(err == MPI_SUCCESS);

  err = MPI_Get(
    out   ,n,to_mpi_type<T>, // origin args
    rank,0,1,target_type   , // target args
    win
  );
  STD_E_ASSERT(err == MPI_SUCCESS);
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
