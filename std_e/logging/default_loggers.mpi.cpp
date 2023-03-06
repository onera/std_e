#include "std_e/logging/log.hpp"
#include "std_e/logging/printer_common.hpp"
#include "std_e/logging/printer_mpi.hpp"

using namespace std_e;

namespace {

bool init_std_e_default_loggers() {
  // ensures that these loggers are present for std_e to run even if no logging configuration file was found
  add_logger_if_absent(logger{"std_e.debug" ,mpi_stdout_printer{}});
  add_logger_if_absent(logger{"std_e.rank_0",mpi_rank_0_stdout_printer{}});
  add_logger_if_absent(logger{"std_e.file"  ,mpi_file_printer{"std_e_log.txt"}});
  return true;
}

// constant here just to trigger initialization
const bool _ = init_std_e_default_loggers();

}
