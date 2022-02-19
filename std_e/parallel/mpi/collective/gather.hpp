#pragma once


#include "std_e/parallel/mpi/base.hpp"
#include "std_e/future/array.hpp"


namespace std_e {


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
all_gather(const std::vector<T>& x, MPI_Comm comm) -> std::vector<T> {
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

// TODO serialize!
template<class T, ptrdiff_t N> auto
all_gather(const std::vector<std_e::array<T,N>>& x, MPI_Comm comm) -> std::vector<std_e::array<T,N>> {
  int sz = x.size()*N;
  std::vector<int> szs = std_e::all_gather(sz,comm);
  std_e::interval_vector<int> displs = std_e::indices_from_strides(szs);

  int total_sz = std::accumulate(szs.begin(),szs.end(),0);
  std::vector<std_e::array<T,N>> res(total_sz/N);

  int err = MPI_Allgatherv(x.data(), sz, to_mpi_type<T>,
                           res.data(), szs.data(), displs.data(), to_mpi_type<T>, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");

  return res;
}


} // std_e
