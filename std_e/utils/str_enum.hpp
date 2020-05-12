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

} // std_e

#define NUMBER_OF_VA_ARGS(...) \
  std_e::count_char(#__VA_ARGS__, ',') + 1


#define STR_ENUM__IMPL(enum_category,enum_name, ... ) \
  enum_category enum_name { __VA_ARGS__ }; \
  \
  constexpr auto enum_size__impl(enum_name) -> size_t { \
    return NUMBER_OF_VA_ARGS(__VA_ARGS__); \
  } \
  \
  inline auto enum_to_strings__impl(enum_name) -> std::vector<std::string> { \
    return std_e::remove_spaces_and_split( \
      #__VA_ARGS__, \
      ',' \
    ); \
  } \
  \
  inline auto to_string(enum_name e) -> std::string { \
    int i = std_e::to_int(e); \
    return enum_to_strings__impl(enum_name{})[i]; \
  } \
  \
  inline auto strings_to_enum_index(enum_name) -> std_e::frozen_flat_map<std::string,int> { \
    return std_e::permutation_frozen_flat_map(std_e::enum_to_strings<enum_name>); \
  } \
  \
  template<class Str_enum_type> auto to_enum(const std::string& s) -> Str_enum_type; \
  \
  template<> inline auto to_enum<enum_name>(const std::string& s) -> enum_name { \
    int index = strings_to_enum_index(enum_name{})[s]; \
    return static_cast<enum_name>(index); \
  }

// DEPRECATED NAME (use STR_ENUM_CLASS)
#define STR_ENUM(enum_name, ... ) STR_ENUM__IMPL(enum class,enum_name,__VA_ARGS__)

#define STD_E_ENUM_CLASS(enum_name, ... ) STR_ENUM__IMPL(enum class,enum_name,__VA_ARGS__)
#define STD_E_ENUM(enum_name, ... ) STR_ENUM__IMPL(enum,enum_name,__VA_ARGS__)

namespace std_e {

// enum_size {
/// primary definition, should be replaced by specific overload
template<class Str_enum_type> constexpr auto
enum_size__impl(Str_enum_type) -> size_t {
  return 0;
}
template<class Str_enum_type>
constexpr size_t enum_size = enum_size__impl(Str_enum_type{}); // use ADL
// enum_size }

// enum_to_strings {
/// primary definition, should be replaced by specific overload
template<class Str_enum_type> auto
enum_to_strings__impl(Str_enum_type) -> std::vector<std::string> {
  return {};
}
template<class Str_enum_type>
const std::vector<std::string> enum_to_strings = enum_to_strings__impl(Str_enum_type{}); // use ADL
// enum_to_strings }

} // std_e

