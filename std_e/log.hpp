#pragma once

// Use these features to save typing when print-debugging

#include "std_e/logging/log.hpp"
#include "std_e/utils/to_string.hpp"

namespace std_e {
  template<class... Ts> auto
  mlog(const Ts&... xs) -> void {
    using std::to_string;
    using std_e::to_string;
    std::string msg = (to_string(xs) + ...) + "\n";
    std_e::log("terminal",msg);
  }

  template<class T, class... Ts, size_t... Is> auto
  log_with_var_names__impl(std::index_sequence<Is...>, const std::vector<std::string>& var_names, const T& x, const Ts&... xs) -> void {
    using std::to_string;
    using std_e::to_string;
    std::string msg = var_names[0] + " = " + to_string(x);
    ( (msg += "    |    " + var_names[Is+1] + " = " + to_string(xs)) , ...);
    msg += "\n";
    std_e::log("terminal",msg);
  }
  template<class... Ts> auto
  log_with_var_names(const std::string& var_names_str, const Ts&... xs) -> void {
    auto var_names = std_e::remove_spaces_and_split(var_names_str,',');
    constexpr size_t n_var = sizeof...(Ts);
    STD_E_ASSERT(var_names.size() == n_var);
    log_with_var_names__impl(std::make_index_sequence<n_var-1>{},var_names,xs...);
  }
}

#define LOG(...) std_e::mlog(__VA_ARGS__)

#define ELOG(x) std_e::mlog(#x," = ",x)

#define MELOG(...) std_e::log_with_var_names(#__VA_ARGS__,__VA_ARGS__)
