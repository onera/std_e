#pragma once


#include <string>
#include <string_view>
#include <vector>
#include <sstream>
#include <algorithm>
#include "std_e/future/algorithm.hpp"


namespace std_e {


inline auto
remove_back_n(std::string& s, int n) -> std::string& {
  s.resize( s.size()-n );
  return s;
}

inline auto
join(const std::vector<std::string>& ss, const std::string& sep) -> std::string {
  if (ss.size()==0) return "";
  std::string res;
  for (const std::string& s : ss) {
    res += s + sep;
  }
  remove_back_n(res,sep.size());
  return res;
}
inline auto
split(const std::string& s, char sep) -> std::vector<std::string> {
   std::vector<std::string> ss;
   std::string token;
   std::istringstream token_stream(s);
   while (std::getline(token_stream, token, sep)) {
      ss.push_back(token);
   }
   return ss;
}

inline auto
copy_without_spaces(const std::string& s) -> std::string {
  std::string res;
  auto not_space = [](char c){ return !isspace(c); };
  std::copy_if(begin(s), end(s), std::back_inserter(res), not_space);
  return res;
}

inline auto
remove_spaces_and_split(const std::string& s, char sep) -> std::vector<std::string> {
  return split(copy_without_spaces(s),sep);
}

constexpr auto
contains(std::string_view s, char c) -> bool {
  auto it = std_e::find(begin(s),end(s),c);
  return it!=end(s);
}
inline auto
begins_with(const std::string& s, const std::string& prefix) -> bool {
  if (prefix.size() > s.size()) return false;
  return std::equal(prefix.begin(), prefix.end(), s.begin());
}
inline auto
ends_with(const std::string& s, const std::string& suffix) -> bool {
  if (suffix.size() > s.size()) return false;
  return std::equal(suffix.rbegin(), suffix.rend(), s.rbegin());
}

constexpr auto
contains(std::string_view s, std::string_view sub) -> bool {
  return s.find(sub) != std::string::npos;
}

constexpr auto
count_char(std::string_view s, char c) -> int {
  return std_e::count(begin(s),end(s),c);
}


} // std_e
