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

template<class Range> auto
_join(const Range& ss, std::string_view sep) -> std::string {
  if (ss.size()==0) return "";
  std::string res;
  for (std::string_view s : ss) {
   res += std::string(s) + std::string(sep);
  }
  remove_back_n(res,sep.size());
  return res;
}

template<class Range> auto
join(const Range& ss, std::string_view sep) -> std::string {
  return _join(ss, sep);
}

// this non-template overload allows to give an initializer list e.g. `join({"ab","c"},"|")`
inline auto
join(const std::vector<std::string>& ss, std::string_view sep) -> std::string {
  return _join(ss, sep);
}

auto split(std::string_view s, char sep) -> std::vector<std::string>;

auto balanced_parens(std::string_view s, char open_paren, char close_paren) -> bool;
auto split_with_parens(std::string_view s, char sep, char open_paren, char close_paren) -> std::vector<std::string>;

auto copy_without_spaces(std::string_view s) -> std::string;

auto trim(std::string_view s) -> std::string;
auto trim(const std::vector<std::string>& ss) -> std::vector<std::string>;

inline auto
remove_spaces_and_split(std::string_view s, char sep) -> std::vector<std::string> {
  return split(copy_without_spaces(s),sep);
}

constexpr auto
contains(std::string_view s, char c) -> bool {
  auto it = std_e::find(begin(s),end(s),c);
  return it!=end(s);
}
inline auto
begins_with(std::string_view s, std::string_view prefix) -> bool {
  if (prefix.size() > s.size()) return false;
  return std::equal(prefix.begin(), prefix.end(), s.begin());
}
inline auto
ends_with(std::string_view s, std::string_view suffix) -> bool {
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
