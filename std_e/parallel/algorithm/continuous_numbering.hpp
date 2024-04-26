#include <vector>
#include <cstdint>
#include "mpi.h"
#include "std_e/future/span.hpp"

namespace std_e {

auto continuous_numbering(std_e::span<int64_t> x, MPI_Comm comm) -> std::vector<int64_t>;

} // std_e
