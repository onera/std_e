#pragma once

#include <iostream>
#include <string>

namespace std_e {

// ------------------------------------------------------------------
// Procedure: startswith
inline bool startswith(std::string const & value, std::string const & starting)
{
  if (starting.size() > value.size())
    return false;
  return std::equal(starting.begin(), starting.end(), value.begin());
}

// ------------------------------------------------------------------
// Procedure: endswith
inline bool endswith(std::string const & value, std::string const & ending)
{
  if (ending.size() > value.size())
    return false;
  return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

// ------------------------------------------------------------------
// Procedure: stringify
template <typename T>
void ostreamize(std::ostringstream& oss, T&& token) {
  oss << std::forward<T>(token);
}

// Procedure: stringify
template <typename T, typename... Rest>
void ostreamize(std::ostringstream& oss, T&& token, Rest&&... rest) {
  oss << std::forward<T>(token);
  ostreamize(oss, std::forward<Rest>(rest)...);
}

}  // end of namespace -----------------------------------------------------
