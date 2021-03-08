#pragma once


#include <string>
#include <vector>
#include "std_e/utils/string.hpp"
#include "std_e/utils/frozen_flat_map.hpp"


namespace std_e {

template<class Enum_type> constexpr auto
to_int(Enum_type e) -> int {
  return static_cast<int>(e);
};

inline auto
enum_value_array(const std::string& enum_desc) -> std::vector<int> {
  auto e_fields = std_e::remove_spaces_and_split(enum_desc,',');
  auto n_fields = e_fields.size();
  std::vector<int> res(n_fields);
  // This algo mimics what does the language to determine the enumeration values
  // SEE https://en.cppreference.com/w/c/language/enum
  int current_e_value = 0;
  for (int i=0; i<n_fields; ++i) {
    const std::string& e_field = e_fields[i];
    auto e_field_decomp = std_e::remove_spaces_and_split(e_field,'=');
    if (e_field_decomp.size()==1) {
      res[i] = current_e_value;
      ++current_e_value;
    } else if (e_field_decomp.size()==2) {
      auto value_str = e_field_decomp[1];
      int value = std::stoi(value_str.c_str());
      res[i] = value;
      current_e_value = value+1;
    } else {
      STD_E_ASSERT(0); // The enum macro has not been specified by an "identifier[=value]" pattern
    }
  }
  return res;
}

inline auto
enum_identifiers(const std::string& enum_desc) -> std::vector<std::string> {
  auto e_fields = std_e::remove_spaces_and_split(enum_desc,',');
  auto n_fields = e_fields.size();
  std::vector<std::string> res(n_fields);
  auto extract_identifier = [](const auto& e_field){ return std_e::remove_spaces_and_split(e_field,'=')[0]; };
  std::transform(begin(e_fields),end(e_fields),begin(res),extract_identifier);
  return res;
}

} // std_e

#define STD_E_NUMBER_OF_VA_ARGS(...) \
  std_e::count_char(#__VA_ARGS__, ',') + 1

#define STD_E_ENUM__IMPL(enum_category,enum_name, ... ) \
  enum_category enum_name { __VA_ARGS__ }; \
  \
  constexpr auto enum_size__impl(enum_name) -> size_t { \
    return STD_E_NUMBER_OF_VA_ARGS(__VA_ARGS__); \
  } \
  \
  inline auto \
  enum_values(enum_name) { \
    return std_e::enum_value_array(#__VA_ARGS__); \
  } \
  \
  inline auto enum_to_strings__impl(enum_name) -> std::vector<std::string> { \
    return std_e::enum_identifiers(#__VA_ARGS__); \
  } \
  \
  inline auto to_string(enum_name e) -> std::string { \
    int e_value = std_e::to_int(e); \
    auto e_values = enum_values(e); \
    int i = std::find(begin(e_values),end(e_values),e_value)-begin(e_values); \
    return enum_to_strings__impl(enum_name{})[i]; \
  } \
  \
  inline auto strings_to_enum_index(enum_name) -> std_e::frozen_flat_map<std::string,int> { \
    return std_e::permutation_frozen_flat_map(std_e::enum_to_strings<enum_name>); \
  } \
  \
  inline auto to_enum__impl(const std::string& s, enum_name e) -> enum_name { \
    int index = strings_to_enum_index(e)[s]; \
    int enum_value = enum_values(e)[index]; \
    return static_cast<enum_name>(enum_value); \
  }

#define STD_E_ENUM_CLASS(enum_name, ... ) STD_E_ENUM__IMPL(enum class,enum_name,__VA_ARGS__)
#define STD_E_ENUM(enum_name, ... )       STD_E_ENUM__IMPL(enum      ,enum_name,__VA_ARGS__)


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

// to_enum {
/// primary definition, should be replaced by specific overload
template<class Str_enum_type> constexpr auto
to_enum(const std::string& , Str_enum_type) -> size_t {
  return 0;
}
template<class Str_enum_type> auto
to_enum(const std::string& s) -> Str_enum_type {
  return to_enum__impl(s,Str_enum_type{}); // use ADL
}
// to_enum }

} // std_e
