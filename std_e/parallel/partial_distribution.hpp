#pragma once


#include "std_e/parallel/mpi/collective/scan.hpp"
#include "std_e/parallel/mpi/collective/reduce.hpp"
#include <string>


namespace std_e {


template<class I>
struct partial_distri {
  I start;
  I finish;
  I size_tot;

  #if __cplusplus > 201703L
    constexpr auto operator<=>(const partial_distri&) const = default;
  #else
    friend constexpr auto operator==(const partial_distri& x, const partial_distri& y) {
      return std::tie(x.start,x.finish,x.size_tot) == std::tie(y.start,y.finish,y.size_tot);
    }
    friend constexpr auto operator!=(const partial_distri& x, const partial_distri& y) {
      return !(x==y);
    }
    friend constexpr auto operator<(const partial_distri& x, const partial_distri& y) {
      return std::tie(x.start,x.finish,x.size_tot) < std::tie(y.start,y.finish,y.size_tot);
    }
  #endif
};

template<class I> auto
to_string(const partial_distri<I>& x) {
  return '(' + std::to_string(x.start) + ',' + std::to_string(x.finish) + ',' + std::to_string(x.size_tot) + ')';
}




template<class I> auto
partial_distribution(I size, MPI_Comm comm) {

  I start = ex_scan(size,MPI_SUM,0,comm);
  I finish = start + size;
  I size_tot = all_reduce(size,MPI_SUM,comm);

  #if __cplusplus > 201703L && defined(REAL_GCC)
    return partial_distri{start,finish,size_tot};
  #else
    return partial_distri<I>{start,finish,size_tot};
  #endif
}


} // std_e
