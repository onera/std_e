#include "std_e/logging/log.hpp"

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
  STD_E_ASSERT(it != end(loggers));
  return *it;
}
auto
add_logger(logger l) -> logger& {
  std::vector<logger>& loggers = all_loggers();
  STD_E_ASSERT(!has_logger(l.name));
  loggers.push_back(std::move(l));
  return loggers.back();
}

auto
log(logger& l, const std::string& msg) -> void {
  if (l.lvl != off) {
    l.printer->log(msg);
  }
}

auto
log(const std::string& log_name, const std::string& msg) -> void {
  auto& logger = get_logger(log_name);
  log(logger,msg);
}

auto
log(logging_level log_lvl, const std::string& msg) -> void {
  for (auto& logger : all_loggers()) {
    if (logger.lvl >= log_lvl) {
      log(logger,msg);
    }
  }
}
auto
log(const std::string& msg) -> void {
  log(info,msg);
}


} // std_e
