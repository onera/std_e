#pragma once


#include "std_e/parallel/mpi/collective/all_to_all.hpp"
#include "std_e/parallel/mpi/collective/gather.hpp"
#include "std_e/future/array.hpp"
#include "std_e/data_structure/block_range/block_range.hpp"
#include "std_e/data_structure/multi_range/multi_range.hpp"
#include "std_e/algorithm/algorithm.hpp"


// TODO serialize!


namespace std_e {


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


template<class C, int N, class Int_range, class F = dense_algo_family> auto
all_to_all(const block_range<C,N>& x, const Int_range& indices, MPI_Comm comm, F algo_family = {}) {
  using T = typename block_range<C,N>::scalar_type;
  auto sbuf = x.underlying_range();

  auto sindices = indices;
  scale(sindices,N);

  auto [rbuf,rindices] = all_to_all(sbuf,sindices,comm);

  block_range<std::vector<T>,N> res(std::move(rbuf));

  scale_inv(rindices,N);
  return std::make_pair(std::move(res),std::move(rindices));
}


template<class Multi_range, class Int_range, class F, size_t... Is> auto
all_to_all_tail_impl(const Multi_range& x, const Int_range& indices, MPI_Comm comm, F algo_family, auto&& rbuf0, std::index_sequence<Is...>) {
  return multi_range2(
    std::move(rbuf0),
    all_to_all(range<Is+1>(x),indices,comm,algo_family).first...
  );
}

template<class R, class... Rs, class Int_range, class F = dense_algo_family> auto
all_to_all(const multi_range2<R,Rs...>& x, const Int_range& indices, MPI_Comm comm, F algo_family = {}) {
  // We call `all_to_all` range by range
  // the received indices will be the same for all ranges, so we only return the first one

  // exchange the first range
  auto [rbuf0,rindices] = all_to_all(range<0>(x),indices,comm,algo_family);

  // exchange the rest
  auto res = all_to_all_tail_impl(x,indices,comm,algo_family,std::move(rbuf0),std::make_index_sequence<sizeof...(Rs)>{});

  return std::make_pair(std::move(res),std::move(rindices));
}


} // std_e
