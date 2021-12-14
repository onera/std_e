#pragma once


#include "std_e/parallel/mpi/base.hpp"


namespace std_e {


template<class T> auto
scan(const T& local, MPI_Op op, MPI_Comm comm) -> T {
  T global;
  int err = MPI_Scan(&local, &global, 1, to_mpi_type<T>, op, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
  return global;
}
template<class T> auto
ex_scan(const T& local, MPI_Op op, MPI_Comm comm) -> T {
  T global;
  int err = MPI_Exscan(&local, &global, 1, to_mpi_type<T>, op, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
  return global;
}
template<class T, class T0> auto
ex_scan(T local, MPI_Op op, T0 init, MPI_Comm comm) -> T {
  T global;
  int err = MPI_Exscan(&local, &global, 1, to_mpi_type<T>, op, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
  if (rank(comm)==0) return init;
  return global;
}


} // std_e
