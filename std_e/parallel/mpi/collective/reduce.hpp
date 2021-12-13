#pragma once


#include "std_e/parallel/mpi/base.hpp"


namespace std_e {


template<class T> auto
all_reduce(const T& local, MPI_Op op, MPI_Comm comm) -> T {
  T global;
  int err = MPI_Allreduce(&local, &global, 1, to_mpi_type<T>, op, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
  return global;
}

template<class T> auto
all_reduce(const std::vector<T>& v, MPI_Op op, MPI_Comm comm) -> std::vector<T> {
  std::vector<T> res(v.size());
  int err = MPI_Allreduce(v.data(), res.data(), v.size(), to_mpi_type<T>, op, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
  return res;
}
template<class T> auto
all_reduce_in_place(std::vector<T>& v, MPI_Op op, MPI_Comm comm) -> void {
  int err = MPI_Allreduce(MPI_IN_PLACE, v.data(), v.size(), to_mpi_type<T>, op, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
}


} // std_e
