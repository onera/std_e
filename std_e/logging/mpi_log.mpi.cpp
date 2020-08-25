#include "std_e/logging/mpi_log.hpp"

namespace std_e {


auto
log_0(const std::string& msg) -> void {
  if (rank(MPI_COMM_WORLD)==0) {
    log(info,msg);
  }
}


} // std_e
