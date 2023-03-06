#include "std_e/logging/log.hpp"
#include "std_e/logging/printer_common.hpp"

using namespace std_e;

namespace {

bool init_std_e_default_loggers() {
  // ensures that these loggers are present for std_e to run even if no logging configuration file was found
  add_logger_if_absent(logger{"std_e.debug", std::make_unique<stdout_printer>{}});
  add_logger_if_absent(logger{"std_e.file" , std::make_unique<mpi_file_printer>{}});
  return true;
}

// constant here just to trigger initialization
const bool _ = init_std_e_default_loggers();

}
