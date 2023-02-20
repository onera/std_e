#include "std_e/logging/log.hpp"

#include <vector>
#include <cstdlib>
#include "std_e/base/msg_exception.hpp"
#include "std_e/utils/string.hpp"


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
    throw msg_exception("std_e::get_logger: no logger of name " + name);
  }
  return *it;
}
auto
add_logger(std::string name) -> logger& {
  std::vector<logger>& loggers = all_loggers();
  if (has_logger(name)) {
    throw msg_exception("std_e::add_logger: logger of name " + name + " already exists");
  } else {
    loggers.emplace_back(logger(name));
    return loggers.back();
  }
}
auto
add_logger(std::string name, std::unique_ptr<printer> printer) -> logger& {
  auto& l = add_logger(name);
  l.printers.emplace_back(std::move(printer));
  return l;
}
auto
add_logger_if_absent(std::string name, std::unique_ptr<printer> printer) -> logger& {
  std::vector<logger>& loggers = all_loggers();
  if (has_logger(name)) {
    return get_logger(name);
  } else {
    loggers.emplace_back(logger(name,std::move(printer)));
    return loggers.back();
  }
}
auto
add_or_replace_logger(logger l) -> logger& {
  std::vector<logger>& loggers = all_loggers();
  auto it = find_if(begin(loggers),end(loggers),[&](const auto& logger){ return logger.name == l.name; });
  if (it != end(loggers)) {
    loggers.erase(it);
  }
  loggers.emplace_back(std::move(l));
  return loggers.back();
}
auto
add_printer_to_logger(const std::string& logger_name, std::unique_ptr<printer> printer) -> void {
  auto& logger = get_logger(logger_name);
  logger.printers.emplace_back(std::move(printer));
}

auto turn_on(const std::string& logger_name) -> void {
  auto& logger = get_logger(logger_name);
  logger.active = true;
}
auto turn_off(const std::string& logger_name) -> void {
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


} // std_e
