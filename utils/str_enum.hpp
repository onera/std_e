#pragma once


#include <string>
#include <vector>
#include "std_e/utils/string.hpp"
#include "std_e/utils/frozen_flat_map.hpp"


namespace std_e {


template<class Enum_type>
constexpr auto to_int(Enum_type e) -> int {
  return static_cast<int>(e);
};


template<class Str_enum_type>
const std::vector<std::string> enum_to_strings;

template<class Str_enum_type>
constexpr size_t enum_size;

template<class Str_enum_type>
const std_e::frozen_flat_map<std::string,int> strings_to_enum_index;

} // std_e

#define NUMBER_OF_VA_ARGS(...) \
  std_e::count_char(#__VA_ARGS__, ',') + 1


// WARNING: STR_ENUM must be declared at global scope, not in a namespace
// If you want a namespace enum, use STR_ENUM_NSPACE (and also call the macro at global scope)
#define STR_ENUM(enum_name, ... ) \
  enum class enum_name { __VA_ARGS__ }; \
  \
  template<> const std::vector<std::string> std_e::enum_to_strings<enum_name> = \
    std_e::remove_spaces_and_split( \
      #__VA_ARGS__, \
      ',' \
    ); \
  \
  template<> constexpr size_t std_e::enum_size<enum_name> = NUMBER_OF_VA_ARGS(__VA_ARGS__); \
  \
  inline auto to_string(enum_name e) -> const std::string& { \
    int i = std_e::to_int(e); \
    return std_e::enum_to_strings<enum_name>[i]; \
  } \
  \
  template<> const std_e::frozen_flat_map<std::string,int> \
    std_e::strings_to_enum_index<enum_name> = \
      std_e::permutation_frozen_flat_map(std_e::enum_to_strings<enum_name>); \
  \
  template<class Str_enum_type> constexpr auto to_enum(const std::string& s) -> Str_enum_type; \
  \
  template<> constexpr auto to_enum<enum_name>(const std::string& s) -> enum_name { \
    int index = std_e::strings_to_enum_index<enum_name>[s]; \
    return static_cast<enum_name>(index); \
  }


#define STR_ENUM_NSPACE(nspace, enum_name, ... ) \
  namespace nspace { enum class enum_name { __VA_ARGS__ }; } \
  \
  template<> const std::vector<std::string> std_e::enum_to_strings<nspace::enum_name> = \
    std_e::remove_spaces_and_split( \
      #__VA_ARGS__, \
      ',' \
    ); \
  \
  template<> constexpr size_t std_e::enum_size<nspace::enum_name> = NUMBER_OF_VA_ARGS(__VA_ARGS__); \
  \
  namespace nspace { \
    inline auto to_string(enum_name e) -> const std::string& { \
      int i = std_e::to_int(e); \
      return std_e::enum_to_strings<enum_name>[i]; \
    } \
  } \
  \
  template<> const std_e::frozen_flat_map<std::string,int> \
    std_e::strings_to_enum_index<nspace::enum_name> = \
      std_e::permutation_frozen_flat_map(std_e::enum_to_strings<nspace::enum_name>); \
  \
  namespace nspace { \
    template<class Str_enum_type> constexpr auto to_enum(const std::string& s) -> Str_enum_type; \
    \
    template<> constexpr auto to_enum<enum_name>(const std::string& s) -> enum_name { \
      int index = std_e::strings_to_enum_index<enum_name>[s]; \
      return static_cast<enum_name>(index); \
    } \
  }
