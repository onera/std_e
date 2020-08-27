#include "std_e/logging/mpi_log.hpp"

namespace std_e {


auto
log_0(logger& l, const std::string& msg) -> void {
  if (rank(MPI_COMM_WORLD)==0) {
    log(l,msg);
  }
}
auto
log_0(const std::string& log_name, const std::string& msg) -> void {
  if (rank(MPI_COMM_WORLD)==0) {
    log(log_name,msg);
  }
}
auto log_0(logging_level log_lvl, const std::string& msg) -> void {
  if (rank(MPI_COMM_WORLD)==0) {
    log(log_lvl,msg);
  }
}
auto
log_0(const std::string& msg) -> void {
  if (rank(MPI_COMM_WORLD)==0) {
    log(info,msg);
  }
}


} // std_e
