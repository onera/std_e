#include "std_e/logging/log.hpp"

using namespace std_e;

namespace {

bool init_default_loggers() {
  add_logger("std_e"     , std::make_unique<stdout_printer>());
  add_logger("std_e.file", std::make_unique<mpi_file_printer>());
  return true;
}

// constant here just to trigger initialization
const bool _ = init_default_loggers();

}
