#pragma once


#include <string>


namespace std_e {


struct printer {
  virtual auto log(const std::string& msg) -> void = 0;
  virtual ~printer() = default;
};


} // std_e

