#include "std_e/logging/mpi_log.hpp"

using namespace std_e;

namespace {

bool init_default_loggers() {
  add_logger({"terminal"       ,full,std::make_unique<mpi_stdout_printer>       ()});
  add_logger({"file"           ,full,std::make_unique<mpi_file_printer>         ()});
  //add_logger({"terminal_0_only",full,std::make_unique<mpi_rank_0_stdout_printer>()});
  return true;
}

// constant here just to trigger initialization
const bool _ = init_default_loggers();

}
