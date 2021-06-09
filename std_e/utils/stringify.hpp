#pragma once

#include "std_e/utils/string.hpp"
#include <iostream>

namespace std_e {

// ------------------------------------------------------------------
// Procedure: startswith
[[deprecated("use begins_with")]] inline bool startswith(std::string const & s, std::string const & prefix)
{
  return begins_with(s,prefix);
}

// ------------------------------------------------------------------
// Procedure: endswith
[[deprecated("use ends_with")]] inline bool endswith(std::string const & s, std::string const & suffix)
{
  return ends_with(s,suffix);
}

// ------------------------------------------------------------------
// Procedure: stringify
template <typename T> [[deprecated("use to_string")]]
void ostreamize(std::ostringstream& oss, T&& token) {
  oss << std::forward<T>(token);
}

// Procedure: stringify
template <typename T, typename... Rest> [[deprecated("use to_string")]]
void ostreamize(std::ostringstream& oss, T&& token, Rest&&... rest) {
  oss << std::forward<T>(token);
  ostreamize(oss, std::forward<Rest>(rest)...);
}

}  // end of namespace -----------------------------------------------------
