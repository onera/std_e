#include "std_e/logging/configure_loggers.hpp"
#include "std_e/logging/create_loggers.hpp"
#include "std_e/utils/file.hpp"
#include <cstdlib>


namespace std_e {


auto  
configure_loggers(const std::string& logging_conf_file) -> void {
  auto error_msg_context = "File \"" + logging_conf_file + "\"";
  auto conf = file_to_string(logging_conf_file);
  auto loggers = create_loggers(conf,error_msg_context);

  for (auto& l : loggers) {
    add_or_replace_logger(std::move(l));
  }
}


} // std_e


namespace {

bool init_loggers_from_conf_file() {
  const char* logging_conf_file = std::getenv("LOGGING_CONF_FILE");
  std_e::configure_loggers(logging_conf_file);
  return true;
}

// constant here just to trigger initialization
const bool _ = init_loggers_from_conf_file();

}
