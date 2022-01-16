#pragma once


#include <limits>
#include <vector>
#include <numeric>
#include "std_e/utils/tuple.hpp"
#include "std_e/future/make_array.hpp"
#include "std_e/future/contract.hpp"
#include "std_e/interval/interval_sequence.hpp"
#include "std_e/parallel/mpi/base.hpp"
#include "std_e/parallel/mpi/collective/reduce.hpp"
#include "std_e/parallel/mpi/collective/gather.hpp"
#include "std_e/parallel/mpi/collective/scan.hpp"


namespace std_e {


template<class T> auto
min_global(T local_min, MPI_Comm comm) -> T {
  T global_min = std::numeric_limits<T>::max();

  int err = MPI_Allreduce(&local_min, &global_min, 1, to_mpi_type<T>, MPI_MIN, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");

  return global_min;
}
template<class T> auto
max_global(T local_max, MPI_Comm comm) -> T {
  T global_max = std::numeric_limits<T>::min();

  int err = MPI_Allreduce(&local_max, &global_max, 1, to_mpi_type<T>, MPI_MAX, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");

  return global_max;
}
template<class T> auto
minmax_global(T local_min, T local_max, MPI_Comm comm) -> std::pair<T,T> {
  return {min_global(local_min,comm),max_global(local_max,comm)};
}


} // std_e
