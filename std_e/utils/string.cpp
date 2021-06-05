#include "std_e/utils/string.hpp"

#include <sstream>
#include <algorithm>


namespace std_e {


auto
remove_back_n(std::string& s, int n) -> std::string& {
  s.resize( s.size()-n );
  return s;
}

auto
join(const std::vector<std::string>& ss, const std::string& sep) -> std::string {
  if (ss.size()==0) return "";
  std::string res;
  for (const std::string& s : ss) {
    res += s + sep;
  }
  remove_back_n(res,sep.size());
  return res;
}
auto
split(const std::string& s, char sep) -> std::vector<std::string> {
   std::vector<std::string> ss;
   std::string token;
   std::istringstream token_stream(s);
   while (std::getline(token_stream, token, sep)) {
      ss.push_back(token);
   }
   return ss;
}

auto
copy_without_spaces(const std::string& s) -> std::string {
  std::string res;
  auto not_space = [](char c){ return !isspace(c); };
  std::copy_if(begin(s), end(s), std::back_inserter(res), not_space);
  return res;
}

auto
remove_spaces_and_split(const std::string& s, char sep) -> std::vector<std::string> {
  return split(copy_without_spaces(s),sep);
}

auto
contains(std::string_view s, char c) -> bool {
  auto it = std::find(begin(s),end(s),c);
  return it!=end(s);
}
auto
begins_with(const std::string& s, const std::string& prefix) -> bool {
  if (prefix.size() > s.size()) return false;
  return std::equal(prefix.begin(), prefix.end(), s.begin());
}
auto
ends_with(const std::string& s, const std::string& suffix) -> bool {
  if (suffix.size() > s.size()) return false;
  return std::equal(suffix.rbegin(), suffix.rend(), s.rbegin());
}


} // std_e
