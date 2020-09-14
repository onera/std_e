#pragma once


#include "std_e/parallel/mpi.hpp"
#include "std_e/parallel/mpi_exception.hpp"
#include "std_e/parallel/serialize.hpp"
#include "std_e/parallel/compressed_array.hpp"
#include "std_e/data_structure/jagged_range.hpp"


namespace std_e {


// paraphernalia to handle MPI_Neighbor_ versions with minimal repetition
struct dense_algo_family {
  static constexpr auto all_to_all   = [](auto... xs){ return MPI_Alltoall (xs...); };
  static constexpr auto all_to_all_v = [](auto... xs){ return MPI_Alltoallv(xs...); };
};
struct neighbor_algo_family {
  static constexpr auto all_to_all   = [](auto... xs){ return MPI_Neighbor_alltoall (xs...); };
  static constexpr auto all_to_all_v = [](auto... xs){ return MPI_Neighbor_alltoallv(xs...); };
};

constexpr auto& default_all_to_all   = dense_algo_family::all_to_all  ;
constexpr auto& default_all_to_all_v = dense_algo_family::all_to_all_v;
using default_all_to_all_t = decltype(default_all_to_all  );
using default_all_to_all_v_t = decltype(default_all_to_all_v);


/// All algorithms in the sections below take an algo_family as their last argument
/// It is defaulted by dense_algo_family, so usable directly from the user
/// If the user wants the MPI_Neighbor_ versions, it is also possible with these two wrappers
template<class... Args> auto
neighbor_all_to_all(Args&&... args) {
  return all_to_all(FWD(args)...,neighbor_algo_family::all_to_all);
}
template<class... Args> auto
neighbor_all_to_all_v(Args&&... args) {
  return all_to_all_v(FWD(args)...,neighbor_algo_family{});
}
/// All algorithms... }

// paraphernalia... }


// all_to_all {
template<class T, class F = default_all_to_all_t> auto
all_to_all(const T* sbuf, int n, T* rbuf, MPI_Comm comm, F alltoall_algo = default_all_to_all) -> void {
  int err = alltoall_algo(sbuf, n, to_mpi_type<T>,
                          rbuf, n, to_mpi_type<T>, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
}

template<class Contiguous_range, class F = default_all_to_all_t, class T = typename Contiguous_range::value_type> auto
all_to_all(const Contiguous_range& s_array, int n, MPI_Comm comm, F alltoall_algo = default_all_to_all) -> std::vector<T> {
  int sz = s_array.size();
  //STD_E_ASSERT(sz == nb_ranks(comm)); // TODO this one is false if MPI_Neighbor_ version
  std::vector<T> r_array(sz);
  all_to_all(s_array.data(),n,r_array.data(),comm,alltoall_algo);
  return r_array;
}
template<class Contiguous_range, class F = default_all_to_all_t, class T = typename Contiguous_range::value_type> auto
all_to_all(const Contiguous_range& s_array, MPI_Comm comm, F alltoall_algo = default_all_to_all) -> std::vector<T> {
  return all_to_all(s_array,1,comm,alltoall_algo);
}
// all_to_all }


// DEL{
template<class T, class F = dense_algo_family> auto
all_to_all_v(const compressed_array<T>& sends, MPI_Comm comm, F algo_family = {}) -> compressed_array<T> {
  const auto& [sbuf,sstrides,soffsets] = sends;

  std::vector<int> rstrides = all_to_all(sstrides,comm,algo_family.all_to_all);

  knot_vector<int> roffsets = indices_from_sizes(rstrides);

  std::vector<T> rbuf(roffsets.length());
  all_to_all_v(sbuf.data(), sstrides.data(), soffsets.data(),
               rbuf.data(), rstrides.data(), roffsets.data(), comm, algo_family.all_to_all_v);

  return {std::move(rbuf),std::move(rstrides),std::move(roffsets)};
}

// TODO with serialize_array->T instead of std::byte
template<class Random_access_range, class F = dense_algo_family, class T = typename Random_access_range::value_type> auto
all_to_all_v(const Random_access_range& sends, MPI_Comm comm, F algo_family = {}) -> std::vector<T> {
  auto [soffsets,sbuf] = serialize_array(sends);
  std::vector<int> sstrides = interval_lengths(soffsets);

  auto [rbuf,_,roffsets] = all_to_all_v(compressed_array<std::byte>{std::move(sbuf),std::move(sstrides),std::move(soffsets)},comm,algo_family);

  return deserialize_array<T>(rbuf.data(),roffsets);
}
// DEL}

template<class T, class F = default_all_to_all_v_t> auto
all_to_all_v(
  const T* sbuf, const int* sstrides, const int* sindices,
        T* rbuf,       int* rstrides,       int* rindices,
  MPI_Comm comm,
  F alltoallv_algo = default_all_to_all_v
) -> void
{
  int err = alltoallv_algo(sbuf, sstrides, sindices, to_mpi_type<T>,
                           rbuf, rstrides, rindices, to_mpi_type<T>, comm);
  if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
}


template<class Range, class Int_range, class F> auto
all_to_all_v__from_indices(const Range& sbuf, const Int_range& sindices, MPI_Comm comm, F algo_family) {
  using T = typename Range::value_type;
  std::vector<int> sstrides = interval_lengths(sindices);
  std::vector<int> rstrides = all_to_all(sstrides,comm,algo_family.all_to_all);
  knot_vector<int> rindices = indices_from_sizes(rstrides);
  std::vector<T> rbuf(rindices.length());

  all_to_all_v(
    sbuf.data(), sstrides.data(), sindices.data(),
    rbuf.data(), rstrides.data(), rindices.data(),
    comm,algo_family.all_to_all_v
  );

  return std::make_pair(std::move(rbuf),std::move(rindices));
}
template<class Range, class Int_range, class F> auto
all_to_all_v__from_strides(const Range& sbuf, const Int_range& sstrides, MPI_Comm comm, F algo_family) {
  using T = typename Range::value_type;
  knot_vector<int> sindices = indices_from_sizes(sindices);
  std::vector<int> rstrides = algo_family.all_to_all(sstrides,comm);
  knot_vector<int> rindices = indices_from_sizes(rstrides);
  std::vector<T> rbuf(rstrides.size());

  all_to_all_v(
    sbuf.data(), sstrides.data(), sindices.data(),
    rbuf.data(), rstrides.data(), rindices.data(),
    comm,algo_family.all_to_all_v
  );

  return std::make_pair(std::move(rbuf),std::move(rindices));
}

template<class DR, class IR, class F = dense_algo_family, class T = typename DR::value_type> auto
all_to_all_v(const jagged_range<DR,IR,2>& sends, MPI_Comm comm, F algo_family = {}) -> jagged_vector<T> {
  auto [rbuf,rindices] = all_to_all_v__from_indices(sends.flat_view(), sends.indices(), comm, algo_family);
  return {std::move(rbuf),std::move(rindices)};
}
template<class DR, class IR, class T = typename DR::value_type> auto
neighbor_all_to_all_v(const jagged_range<DR,IR,2>& sends, MPI_Comm comm) -> jagged_vector<T> {
  return all_to_all_v(sends,comm,neighbor_algo_family{});
}


} // std_e
