#pragma once


#include "std_e/parallel/mpi.hpp"
#include "std_e/parallel/mpi_exception.hpp"
#include "std_e/parallel/serialize.hpp"
#include "std_e/parallel/compressed_array.hpp"


namespace std_e {


template<class T> auto
all_to_all(const T* sbuf, int n, T* rbuf, MPI_Comm comm) -> void {
  int err = MPI_Alltoall(sbuf, n, std_e::to_mpi_type<T>,
                         rbuf, n, std_e::to_mpi_type<T>, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
}
template<class Contiguous_range, class T = typename Contiguous_range::value_type> auto
all_to_all(const Contiguous_range& s_array, int n, MPI_Comm comm) -> std::vector<T> {
  int sz = s_array.size();
  STD_E_ASSERT(sz == std_e::nb_ranks(comm));
  std::vector<T> r_array(sz);
  all_to_all(s_array.data(),n,r_array.data(),comm);
  return r_array;
}
template<class Contiguous_range, class T = typename Contiguous_range::value_type> auto
all_to_all(const Contiguous_range& s_array, MPI_Comm comm) -> std::vector<T> {
  return all_to_all(s_array,1,comm);
}


template<class T> auto
all_to_all_v(const compressed_array<T>& sends, MPI_Comm comm) -> compressed_array<T> {
  const auto& [sbuf,scounts,soffsets] = sends;

  std::vector<int> rcounts = all_to_all(scounts,comm);

  knot_vector<int> roffsets = offsets_from_sizes(rcounts);

  std::vector<T> rbuf(roffsets.length());
  int err = MPI_Alltoallv(sbuf.data(), scounts.data(), soffsets.data(), std_e::to_mpi_type<T>,
                          rbuf.data(), rcounts.data(), roffsets.data(), std_e::to_mpi_type<T>, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");

  return {std::move(rbuf),std::move(rcounts),std::move(roffsets)};
}


// TODO with serialize_array->T instead of std::byte
template<class Random_access_range, class T = typename Random_access_range::value_type> auto
all_to_all_v(const Random_access_range& sends, MPI_Comm comm) -> std::vector<T> {
  auto [soffsets,sbuf] = serialize_array(sends);
  std::vector<int> scounts = interval_lengths(soffsets);

  auto [rbuf,_,roffsets] = all_to_all_v(compressed_array<std::byte>{std::move(sbuf),std::move(scounts),std::move(soffsets)},comm);

  return deserialize_array<T>(rbuf.data(),roffsets);
}


template<class T> auto
neighbor_all_to_all(const T* sbuf, int n, T* rbuf, MPI_Comm comm) -> void {
  int err = MPI_Neighbor_alltoall(sbuf, n, std_e::to_mpi_type<T>,
                         rbuf, n, std_e::to_mpi_type<T>, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
}
template<class Contiguous_range, class T = typename Contiguous_range::value_type> auto
neighbor_all_to_all(const Contiguous_range& s_array, int n, MPI_Comm comm) -> std::vector<T> {
  int sz = s_array.size();
  //STD_E_ASSERT(sz == std_e::nb_ranks(comm));
  std::vector<T> r_array(sz);
  neighbor_all_to_all(s_array.data(),n,r_array.data(),comm);
  return r_array;
}
template<class Contiguous_range, class T = typename Contiguous_range::value_type> auto
neighbor_all_to_all(const Contiguous_range& s_array, MPI_Comm comm) -> std::vector<T> {
  return neighbor_all_to_all(s_array,1,comm);
}


template<class T> auto
neighbor_all_to_all_v(const compressed_array<T>& sends, MPI_Comm comm) -> compressed_array<T> {
  const auto& [sbuf,scounts,soffsets] = sends;

  std::vector<int> rcounts = neighbor_all_to_all(scounts,comm);

  knot_vector<int> roffsets = offsets_from_sizes(rcounts);

  std::vector<T> rbuf(roffsets.length());
  int err = MPI_Neighbor_alltoallv(sbuf.data(), scounts.data(), soffsets.data(), std_e::to_mpi_type<T>,
                          rbuf.data(), rcounts.data(), roffsets.data(), std_e::to_mpi_type<T>, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");

  return {std::move(rbuf),std::move(rcounts),std::move(roffsets)};
}


// TODO with serialize_array->T instead of std::byte
template<class Random_access_range, class T = typename Random_access_range::value_type> auto
neighbor_all_to_all_v(const Random_access_range& sends, MPI_Comm comm) -> std::vector<T> {
  auto [soffsets,sbuf] = serialize_array(sends);
  std::vector<int> scounts = interval_lengths(soffsets);

  auto [rbuf,_,roffsets] = neighbor_all_to_all_v(compressed_array<std::byte>{std::move(sbuf),std::move(scounts),std::move(soffsets)},comm);

  return deserialize_array<T>(rbuf.data(),roffsets);
}


//// TODO stride protocol
//template<class T, int N> auto
//neighbor_all_to_all_v(const jagged_array<T,N>& sends, MPI_Comm comm) -> compressed_array<T> {
//  auto sbuf = 
//
//  std::vector<int> rcounts = neighbor_all_to_all(scounts,comm);
//
//  knot_vector<int> roffsets = offsets_from_sizes(rcounts);
//
//  std::vector<T> rbuf(roffsets.length());
//  int err = MPI_Neighbor_alltoallv(sbuf.data(), scounts.data(), soffsets.data(), std_e::to_mpi_type<T>,
//                          rbuf.data(), rcounts.data(), roffsets.data(), std_e::to_mpi_type<T>, comm);
//  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
//
//  return {std::move(rbuf),std::move(rcounts),std::move(roffsets)};
//}



} // std_e
