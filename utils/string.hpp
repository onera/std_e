#pragma once


#include <string>
#include <sstream>
#include <vector>


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
copy_without_whitespaces(const std::string& s) -> std::string {
  std::string res;
  auto not_space = [](char c){ return !isspace(c); };
  std::copy_if(begin(s), end(s), std::back_inserter(res), not_space);
  return res;
}


} // std_e
