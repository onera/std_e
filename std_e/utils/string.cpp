#include "std_e/utils/string.hpp"


namespace std_e {


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
balanced_parens(const std::string& s, char open_paren, char close_paren) -> bool {
  int paren_count = 0;
  for (char c : s) {
    if (c == open_paren) {
      ++paren_count;
    }
    if (c == close_paren) {
      --paren_count;
      if (paren_count < 0) return false;
    }
  }
  return paren_count == 0;
}

auto
split_with_parens(const std::string& s, char sep, char open_paren, char close_paren) -> std::vector<std::string> {
  std::vector<std::string> ss;
  std::string token;
  int paren_count = 0;
  for (char c : s) {
    if (c == sep && paren_count == 0) {
      ss.emplace_back(token);
      token = "";
    }
    else { // c!=sep || paren_count!=0 
      token += c;
      if (c == open_paren) {
        ++paren_count;
      }
      else if (c == close_paren) {
        --paren_count;
      }
    }
  }
  ss.emplace_back(token);
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
trim(const std::string& s) -> std::string {
  auto first = begin(s);
  auto last  = end(s);
  while ((first != last) && isspace(* first  )) { ++first; }
  while ((first != last) && isspace(*(last-1))) { --last; }
  return std::string(first,last);
}
auto
trim(const std::vector<std::string>& ss) -> std::vector<std::string> {
  std::vector<std::string> res(ss.size());
  std::transform(begin(ss),end(ss),begin(res),[](const auto& s){ return trim(s); });
  return res;
}


} // std_e
