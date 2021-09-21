#pragma once


#include "std_e/parallel/mpi/base.hpp"


namespace std_e {

template<class T, class I> auto
_send(const T* buf, I size, int target_rank, int tag, MPI_Comm comm) -> void {
  int err = MPI_Send(buf, size, to_mpi_type<T>, target_rank, tag, comm);
  STD_E_ASSERT(!err);
}

template<class T, class I> auto
_recv(T* buf, I size, int source_rank, int tag, MPI_Comm comm) -> void {
  int err = MPI_Recv(buf, 20, to_mpi_type<T>, source_rank, tag, comm, MPI_STATUS_IGNORE);
  STD_E_ASSERT(!err);
}


template<class Range> auto
send(const Range& buf, int target_rank, int tag, MPI_Comm comm) -> void {
  _send(buf.data(),buf.size(),target_rank,tag,comm);
}

template<class Range> auto
recv(Range&& buf, int source_rank, int tag, MPI_Comm comm) -> void {
  _recv(buf.data(),buf.size(),source_rank,tag,comm);
}

} // std_e
