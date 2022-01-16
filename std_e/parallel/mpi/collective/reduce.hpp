#pragma once


#include "std_e/parallel/mpi/base.hpp"
#include <ranges>


namespace std_e {


template<class T> auto
all_reduce(const T& local, MPI_Op op, MPI_Comm comm) -> T {
  T global;
  int err = MPI_Allreduce(&local, &global, 1, to_mpi_type<T>, op, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
  return global;
}

template<class Rng0, class Rng1> auto
_all_reduce_rng_impl(const Rng0& in, Rng1& out, MPI_Op op, MPI_Comm comm) -> void {
  using T = typename Rng0::value_type;
  int err = MPI_Allreduce(in.data(), out.data(), in.size(), to_mpi_type<T>, op, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
}
template<class T> auto
all_reduce(const std_e::span<T>& v, MPI_Op op, MPI_Comm comm) -> std::vector<T> {
  std::vector<T> res(v.size());
  _all_reduce_rng_impl(v,res,op,comm);
  return res;
}
// any contiguous range (other than span)
template<std::ranges::contiguous_range Rng> auto
all_reduce(const Rng& v, MPI_Op op, MPI_Comm comm) -> Rng {
  Rng res(v.size());
  _all_reduce_rng_impl(v,res,op,comm);
  return res;
}

template<class Rng> auto
all_reduce_in_place(Rng& x, MPI_Op op, MPI_Comm comm) -> void {
  using T = typename Rng::value_type;
  int err = MPI_Allreduce(MPI_IN_PLACE, x.data(), x.size(), to_mpi_type<T>, op, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
}


} // std_e
