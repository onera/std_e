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
find_logger(const std::string& name) -> std::vector<logger>::iterator {
  std::vector<logger>& loggers = all_loggers();

  return 
    find_if(
      begin(loggers),end(loggers),
      [&](const auto& l){ return l.name == name; }
    );
}

auto
has_logger(const std::string& name) -> bool {
  std::vector<logger>& loggers = all_loggers();
  auto it = find_logger(name);
  return it == end(loggers);
}

auto
get_logger(const std::string& name) -> logger& {
  std::vector<logger>& loggers = all_loggers();

  auto it = find_logger(name);
  if (it == end(loggers)) {
    throw msg_exception("std_e::get_logger: no logger of name " + name);
  }
  return *it;
}

auto
add_logger(logger l, bool replace) -> logger& {
  std::vector<logger>& loggers = all_loggers();

  auto it = find_logger(l.name);

  if (it != end(loggers)) {
    if (replace) {
      *it = std::move(l);
    }
    return *it;
  } else {
    loggers.emplace_back(std::move(l));
    return loggers.back();
  }
}
auto
add_or_replace_logger(logger l) -> logger& {
  return add_logger(std::move(l),true);
}
auto
add_logger_if_absent(logger l) -> logger& {
  return add_logger(std::move(l),false);
}


auto turn_on(const std::string& logger_name) -> void {
  auto& l = get_logger(logger_name);
  l.active = true;
}
auto turn_off(const std::string& logger_name) -> void {
  auto& l = get_logger(logger_name);
  l.active = false;
}

auto
log(const std::string& logger_name, const std::string& msg) -> void {
  auto& l = get_logger(logger_name);
  if (l.active) {
    for (auto& p: l.printers) {
      p->log(msg);
    }
  }
}


} // std_e
