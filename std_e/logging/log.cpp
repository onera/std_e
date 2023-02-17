#include "std_e/logging/log.hpp"
#include "std_e/base/msg_exception.hpp"

#include <vector>


namespace std_e {


auto
all_loggers() -> std::vector<logger>& {
  // See C++ Magic Statics (N2660)
  static std::vector<logger> loggers;
  return loggers;
}

auto
has_logger(const std::string& name) -> bool {
  std::vector<logger>& loggers = all_loggers();
  auto it = find_if(begin(loggers),end(loggers),[&](const auto& logger){ return logger.name == name; });
  return it != end(loggers);
}
auto
get_logger(const std::string& name) -> logger& {
  std::vector<logger>& loggers = all_loggers();
  auto it = find_if(begin(loggers),end(loggers),[&](const auto& logger){ return logger.name == name; });
  if (it == end(loggers)) {
    throw msg_exception("std_e::get_logger: no logger of name "+name);
  }
  return *it;
}
auto
add_logger(std::string name, std::unique_ptr<log_printer> printer) -> logger& {
  std::vector<logger>& loggers = all_loggers();
  if (has_logger(name)) {
    throw msg_exception("std_e::add_logger: logger of name "+name+" already exists");
  }
  loggers.push_back( logger(std::move(name),std::move(printer)) );
  return loggers.back();
}
auto
add_printer_to_logger(const std::string& logger_name, std::unique_ptr<log_printer> printer) -> void {
  auto& logger = get_logger(logger_name);
  logger.printers.emplace_back(std::move(printer));
}

auto turn_on(const std::string& logger_name) {
  auto& logger = get_logger(logger_name);
  logger.active = true;
}
auto turn_off(const std::string& logger_name) {
  auto& logger = get_logger(logger_name);
  logger.active = false;
}

auto
log(const std::string& logger_name, const std::string& msg) -> void {
  auto& l = get_logger(logger_name);
  if (l.active) {
    for (auto& printer : l.printers) {
      printer->log(msg);
    }
  }
}

auto
add_stdout_printer(const std::string& logger_name) -> void {
  add_printer_to_logger(logger_name, std::make_unique<stdout_printer>());
}
auto
add_file_printer(const std::string& logger_name, const std::string& file_name) -> void {
  add_printer_to_logger(logger_name, std::make_unique<file_printer>(file_name));
}


} // std_e
