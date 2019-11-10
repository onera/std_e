#pragma once


#include <string>


namespace std_e {


inline auto
remove_back_n(std::string& s, int n) -> std::string& {
  s.resize( s.size()-n );
  return s;
}


} // std_e
