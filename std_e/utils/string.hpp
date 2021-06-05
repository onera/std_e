#pragma once


#include <string>
#include <string_view>
#include <vector>
#include "std_e/future/algorithm.hpp"


namespace std_e {


auto remove_back_n(std::string& s, int n) -> std::string&;

auto join(const std::vector<std::string>& ss, const std::string& sep) -> std::string;
auto split(const std::string& s, char sep) -> std::vector<std::string>;

auto copy_without_spaces(const std::string& s) -> std::string;

auto remove_spaces_and_split(const std::string& s, char sep) -> std::vector<std::string>;

auto contains(std::string_view s, char c) -> bool;
auto begins_with(const std::string& s, const std::string& prefix) -> bool;
auto ends_with(const std::string& s, const std::string& prefix) -> bool;

constexpr auto
count_char(std::string_view s, char c) -> int {
  return count(begin(s),end(s),c);
}


} // std_e
