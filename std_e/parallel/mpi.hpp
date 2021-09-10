#pragma once


#include <limits>
#include <vector>
#include <numeric>
#include "std_e/utils/tuple.hpp"
#include "std_e/future/make_array.hpp"
#include "std_e/future/contract.hpp"
#include "std_e/interval/interval_sequence.hpp"
#include "std_e/parallel/mpi/base.hpp"


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


template<class T> auto
all_gather(T value, T* rbuf, MPI_Comm comm) -> void {
  int err = MPI_Allgather(&value, 1, to_mpi_type<T>,
                          rbuf  , 1, to_mpi_type<T>, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
}

template<class T> auto
all_gather(T x, MPI_Comm comm) -> std::vector<T> {
  std::vector<T> data_received(n_rank(comm));
  std_e::all_gather(x,data_received.data(),comm);
  return data_received;
}

template<class T> auto
all_gather(const std::vector<T>& x,MPI_Comm comm) -> std::vector<T> {
  int sz = x.size();
  std::vector<int> szs = std_e::all_gather(sz,comm);
  std_e::interval_vector<int> displs = std_e::indices_from_strides(szs);

  int total_sz = std::accumulate(szs.begin(),szs.end(),0);
  std::vector<T> res(total_sz);

  int err = MPI_Allgatherv(x.data(), x.size(), to_mpi_type<T>,
                           res.data(), szs.data(), displs.data(), to_mpi_type<T>, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");

  return res;
}

template<class T> auto
all_reduce(const T& value, MPI_Op op, MPI_Comm comm) -> T {
  T res;
  int err = MPI_Allreduce(&value, &res, 1, to_mpi_type<T>, op, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
  return res;
}


} // std_e
