#pragma once


#include "std_e/parallel/mpi/base.hpp"
#include "std_e/serialize/serialize.hpp"


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

template<class T, class Array> auto
_all_gather_array(const Array& x, MPI_Comm comm) -> Array {
  int sz = x.size();
  std::vector<int> szs = std_e::all_gather(sz,comm);
  std_e::interval_vector<int> displs = std_e::indices_from_strides(szs);

  int total_sz = std::accumulate(szs.begin(),szs.end(),0);
  Array res(total_sz);

  int err = MPI_Allgatherv(x.data(), x.size(), to_mpi_type<T>,
                           res.data(), szs.data(), displs.data(), to_mpi_type<T>, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");

  return res;
}
template<class T> auto
all_gather(const std::vector<T>& x, MPI_Comm comm) -> std::vector<T> {
  return _all_gather_array<T>(x,comm);
}
//auto
//all_gather(const std::string& x, MPI_Comm comm) -> std::string {
//  return _all_gather_array<char>(x,comm);
//}

//template<class T>
//  requires std::is_trivially_copyable_v<T>
//    auto
//area_of(const T& x) -> size_t {
//  return sizeof(T);
//}
//area_of(const T& x) -> size_t {
//  return sizeof(T);
//}




inline auto
all_gather(const std::vector<std::string>& x, MPI_Comm comm) -> std::vector<std::string> {
  auto [x_szs, x_serial] = serialize_2(x);
  auto x_szs_all    = all_gather(x_szs,comm);
  auto x_serial_all = all_gather(x_serial,comm);
  return deserialize_2<std::vector<std::string>>(x_szs_all,x_serial_all);
}


} // std_e
