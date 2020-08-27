#pragma once

// Save typing: use for debugging purposes
#include "std_e/logging/log.hpp"

template<class... Ts> auto
std_e_log(const Ts&... xs) -> void {
  using std::to_string;
  using std_e::to_string;
  std::string msg = (to_string(xs) + ...) + "\n";
  std_e::log(msg);
}

#define LOG std_e_log
