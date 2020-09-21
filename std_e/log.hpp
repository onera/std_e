#pragma once

// Use these features to save typing when print-debugging

#include "std_e/logging/log.hpp"
#include "std_e/utils/to_string.hpp"

template<class... Ts> auto
std_e_log(const Ts&... xs) -> void {
  using std::to_string;
  using std_e::to_string;
  std::string msg = (to_string(xs) + ...) + "\n";
  std_e::log(msg);
}

#define LOG std_e_log

#define ELOG(x) std_e_log(#x," = ",x);
