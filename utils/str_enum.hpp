#pragma once


#include <string>
#include <vector>
#include "std_e/utils/string.hpp"


namespace std_e {


template<class Enum_type>
constexpr auto to_int(Enum_type e) -> int {
  return static_cast<int>(e);
};

template<class Str_enum_type>
const std::vector<std::string> enum_to_strings;


} // std_e


#define STR_ENUM(enum_name, ... ) \
  enum class enum_name { __VA_ARGS__ }; \
  template<> const std::vector<std::string> std_e::enum_to_strings<enum_name> = \
    std_e::split( \
      std_e::copy_without_whitespaces(#__VA_ARGS__), \
      ',' \
    );


namespace std_e {


template<class Str_enum_type>
constexpr auto to_string(Str_enum_type e) -> const std::string& {
  int i = to_int(e);
  return enum_to_strings<Str_enum_type>[i];
};


} // std_e
