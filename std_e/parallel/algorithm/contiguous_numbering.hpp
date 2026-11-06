#include <vector>
#include <string>
#include <cstdint>
#include "mpi.h"
#include "std_e/future/span.hpp"
#include "std_e/debug.hpp"

namespace std_e {

auto contiguous_numbering(std_e::span<int64_t> x, MPI_Comm comm, const std::string& strategy) -> std::vector<int64_t>;

template<class Contiguous_rng, class T = typename Contiguous_rng::value> auto
binary_search(const Contiguous_rng& x, const T& value) -> int
{
  int sz = x.size();

  if (sz == 0) return -1;
  if (value < x[0]) return -1;
  if (value > x[sz-1]) return -1;

  int left = 0;
  int mid = sz / 2;
  int right = sz;
  while (sz > 1) {
    if (value < x[mid]) {
      sz = mid - left;
      right = mid;
      mid = left + sz/2;
    } else if (value == x[mid]) {
      return mid;
    } else { // value > x[mid]
      sz = right - (mid+1);
      left = mid+1;
      mid = left + sz/2;
    }
  }
  return left;
}


} // std_e
