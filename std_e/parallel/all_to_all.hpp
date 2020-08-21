#pragma once


#include "std_e/parallel/mpi.hpp"
#include "std_e/parallel/mpi_exception.hpp"
#include "std_e/parallel/serialize.hpp"


namespace std_e {


template<class Random_access_range, class T = typename Random_access_range::value_type> auto
all_to_all_v(const Random_access_range& sends, MPI_Comm comm) -> std::vector<T> {
  auto [soffsets,sbuf] = serialize_array(sends);
  std::vector<int> scounts = interval_lengths(soffsets);

  std::vector<int> rcounts(std_e::nb_ranks(comm));
  int err0 = MPI_Alltoall(scounts.data(), 1, std_e::to_mpi_type<int>,
                          rcounts.data(), 1, std_e::to_mpi_type<int>, comm);
  if (err0!=0) throw mpi_exception(err0,"in function \"__func__\"");

  knot_vector<int> roffsets = offsets_from_sizes(rcounts);

  std::vector<std::byte> rbuf(roffsets.length());
  int err1 = MPI_Alltoallv(sbuf.data(), scounts.data(), soffsets.data(), MPI_BYTE,
                           rbuf.data(), rcounts.data(), roffsets.data(), MPI_BYTE, comm);
  if (err1!=0) throw mpi_exception(err1,"in function \"__func__\"");

  return deserialize_array<T>(rbuf.data(),roffsets);
}


} // std_e
