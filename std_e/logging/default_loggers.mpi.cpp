#include "std_e/logging/mpi_log.hpp"

using namespace std_e;

namespace {

bool init_default_loggers() {
  add_logger("std_e"       ,std::make_unique<mpi_stdout_printer>());
  add_logger("std_e.rank_0",std::make_unique<mpi_rank_0_stdout_printer>());
  add_logger("std_e.file"  ,std::make_unique<mpi_file_printer>("std_e_log.txt"));
  return true;
}

// constant here just to trigger initialization
const bool _ = init_default_loggers();

}
