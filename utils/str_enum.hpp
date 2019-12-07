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

template<class Str_enum_type>
const size_t enum_size;


} // std_e


// WARNING: STR_ENUM must be declared at global scope, not in a namespace
// If you want a namespace enum, use STR_ENUM_NSPACE (and also call the macro at global scope)
#define STR_ENUM(enum_name, ... ) \
  enum class enum_name { __VA_ARGS__ }; \
  template<> const std::vector<std::string> std_e::enum_to_strings<enum_name> = \
    std_e::remove_spaces_split( \
      #__VA_ARGS__, \
      ',' \
    ); \
  template<> const size_t std_e::enum_size<enum_name> = std_e::enum_to_strings<enum_name>.size();


#define STR_ENUM_NSPACE(nspace, enum_name, ... ) \
  namespace nspace { enum class enum_name { __VA_ARGS__ }; } \
  template<> const std::vector<std::string> std_e::enum_to_strings<nspace::enum_name> = \
    std_e::remove_spaces_split( \
      #__VA_ARGS__, \
      ',' \
    ); \
  template<> const size_t std_e::enum_size<nspace::enum_name> = std_e::enum_to_strings<nspace::enum_name>.size();


namespace std_e {


template<class Str_enum_type>
constexpr auto to_string(Str_enum_type e) -> const std::string& {
  int i = to_int(e);
  return enum_to_strings<Str_enum_type>[i];
};


} // std_e
