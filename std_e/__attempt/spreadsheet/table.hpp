#pragma once

#include "std_e/data_structure/multi_range.hpp"
#include "std_e/meta/macro.hpp"

namespace std_e {

// spreadsheet-like class
// "column" names may be used to create descriptive function names (through use of macros below)
template<class... Ts> using table = multi_vector<Ts...>;

} // std_e

#define PLURAL(x) x##s // No way to define irregular plurals!

#define STD_E_GEN_TABLE_FUN_UTILS_4(table_type, type0,attr0, type1,attr1) \
  auto PLURAL(attr0)(const table_type& x) -> const auto& { return std_e::range<0>(x); } \
  auto PLURAL(attr0)(      table_type& x) ->       auto& { return std_e::range<0>(x); } \
  auto PLURAL(attr1)(const table_type& x) -> const auto& { return std_e::range<1>(x); } \
  auto PLURAL(attr1)(      table_type& x) ->       auto& { return std_e::range<1>(x); } \
  \
  auto find_row_from_##attr0(const table_type& x, const type0& value) { return std_e::find<0>(x,value); } \
  auto find_row_from_##attr0(      table_type& x, const type0& value) { return std_e::find<0>(x,value); } \
  auto find_row_from_##attr1(const table_type& x, const type1& value) { return std_e::find<1>(x,value); } \
  auto find_row_from_##attr1(      table_type& x, const type1& value) { return std_e::find<1>(x,value); } \
  \
  auto find_##attr0##_from_##attr1(const table_type& x, const type1& value) -> const auto& { return std_e::find_element<1,0>(x,value); } \
  auto find_##attr0##_from_##attr1(      table_type& x, const type1& value) ->       auto& { return std_e::find_element<1,0>(x,value); } \
  \
  auto find_##attr1##_from_##attr0(const table_type& x, const type0& value) -> const auto& { return std_e::find_element<0,1>(x,value); } \
  auto find_##attr1##_from_##attr0(      table_type& x, const type0& value) ->       auto& { return std_e::find_element<0,1>(x,value); } \
  \
  auto sort_by_##attr0(table_type& x) -> void { return std_e::sort_by<0>(x); } \
  auto sort_by_##attr1(table_type& x) -> void { return std_e::sort_by<1>(x); } \
  \

#define STD_E_GEN_TABLE_FUN_UTILS_6(table_type, type0,attr0, type1,attr1, type2,attr2) \
  STD_E_GEN_TABLE_FUN_UTILS_4(table_type, type0,attr0, type1,attr1) \
  \
  auto PLURAL(attr2)(const table_type& x) -> const auto& { return std_e::range<2>(x); } \
  auto PLURAL(attr2)(      table_type& x) ->       auto& { return std_e::range<2>(x); } \
  \
  auto find_row_from_##attr2(const table_type& x, const type2& value) { return std_e::find<2>(x,value); } \
  auto find_row_from_##attr2(      table_type& x, const type2& value) { return std_e::find<2>(x,value); } \
  \
  auto find_##attr0##_from_##attr2(const table_type& x, const type2& value) -> const auto& { return std_e::find_element<2,0>(x,value); } \
  auto find_##attr0##_from_##attr2(      table_type& x, const type2& value) ->       auto& { return std_e::find_element<2,0>(x,value); } \
  \
  auto find_##attr1##_from_##attr2(const table_type& x, const type2& value) -> const auto& { return std_e::find_element<2,1>(x,value); } \
  auto find_##attr1##_from_##attr2(      table_type& x, const type2& value) ->       auto& { return std_e::find_element<2,1>(x,value); } \
  \
  auto find_##attr2##_from_##attr0(const table_type& x, const type0& value) -> const auto& { return std_e::find_element<0,2>(x,value); } \
  auto find_##attr2##_from_##attr0(      table_type& x, const type0& value) ->       auto& { return std_e::find_element<0,2>(x,value); } \
  auto find_##attr2##_from_##attr1(const table_type& x, const type1& value) -> const auto& { return std_e::find_element<1,2>(x,value); } \
  auto find_##attr2##_from_##attr1(      table_type& x, const type1& value) ->       auto& { return std_e::find_element<1,2>(x,value); } \
  \
  auto sort_by_##attr2(table_type& x) -> void { return std_e::sort_by<2>(x); } \
  \

#define STD_E_GEN_TABLE_FUN_UTILS_8(table_type, type0,attr0, type1,attr1, type2,attr2, type3,attr3) \
  STD_E_GEN_TABLE_FUN_UTILS_6(table_type, type0,attr0, type1,attr1, type2,attr2) \
  \
  auto PLURAL(attr3)(const table_type& x) -> const auto& { return std_e::range<3>(x); } \
  auto PLURAL(attr3)(      table_type& x) ->       auto& { return std_e::range<3>(x); } \
  \
  auto find_row_from_##attr3(const table_type& x, const type3& value) { return std_e::find<3>(x,value); } \
  auto find_row_from_##attr3(      table_type& x, const type3& value) { return std_e::find<3>(x,value); } \
  \
  auto find_##attr0##_from_##attr3(const table_type& x, const type3& value) -> const auto& { return std_e::find_element<3,0>(x,value); } \
  auto find_##attr0##_from_##attr3(      table_type& x, const type3& value) ->       auto& { return std_e::find_element<3,0>(x,value); } \
  \
  auto find_##attr1##_from_##attr3(const table_type& x, const type3& value) -> const auto& { return std_e::find_element<3,1>(x,value); } \
  auto find_##attr1##_from_##attr3(      table_type& x, const type3& value) ->       auto& { return std_e::find_element<3,1>(x,value); } \
  \
  auto find_##attr2##_from_##attr3(const table_type& x, const type3& value) -> const auto& { return std_e::find_element<3,2>(x,value); } \
  auto find_##attr2##_from_##attr3(      table_type& x, const type3& value) ->       auto& { return std_e::find_element<3,2>(x,value); } \
  \
  auto find_##attr3##_from_##attr0(const table_type& x, const type0& value) -> const auto& { return std_e::find_element<0,3>(x,value); } \
  auto find_##attr3##_from_##attr0(      table_type& x, const type0& value) ->       auto& { return std_e::find_element<0,3>(x,value); } \
  auto find_##attr3##_from_##attr1(const table_type& x, const type1& value) -> const auto& { return std_e::find_element<1,3>(x,value); } \
  auto find_##attr3##_from_##attr1(      table_type& x, const type1& value) ->       auto& { return std_e::find_element<1,3>(x,value); } \
  auto find_##attr3##_from_##attr2(const table_type& x, const type2& value) -> const auto& { return std_e::find_element<2,3>(x,value); } \
  auto find_##attr3##_from_##attr2(      table_type& x, const type2& value) ->       auto& { return std_e::find_element<2,3>(x,value); } \
  \
  auto sort_by_##attr3(table_type& x) -> void { return std_e::sort_by<3>(x); } \
  \

#define STD_E_TABLE_4(table_type, type0,attr0, type1,attr1) \
  using table_type = std_e::table<type0,type1>; \
  STD_E_GEN_TABLE_FUN_UTILS_4(table_type, type0,attr0, type1,attr1)

#define STD_E_TABLE_6(table_type, type0,attr0, type1,attr1, type2,attr2) \
  using table_type = std_e::table<type0,type1,type2>; \
  STD_E_GEN_TABLE_FUN_UTILS_6(table_type, type0,attr0, type1,attr1, type2,attr2)

#define STD_E_TABLE_8(table_type, type0,attr0, type1,attr1, type2,attr2, type3,attr3) \
  using table_type = std_e::table<type0,type1,type2>; \
  STD_E_GEN_TABLE_FUN_UTILS_6(table_type, type0,attr0, type1,attr1, type2,attr2, type3,attr3)


#define STD_E_TABLE(table_type, ...) \
  static_assert(NB_VA_ARGS(__VA_ARGS__)%2 == 0,"STD_E_TABLE arguments must alternate types and names"); \
  static_assert(NB_VA_ARGS(__VA_ARGS__)/2 >= 2,"STD_E_TABLE implemented only for 2, 3, or 4 columns"); \
  static_assert(NB_VA_ARGS(__VA_ARGS__)/2 <= 4,"STD_E_TABLE implemented only for 2, 3, or 4 columns"); \
  MACRO_NAME_SUFFIXED_BY_NB_VA_ARGS(STD_E_TABLE_,__VA_ARGS__)(table_type, __VA_ARGS__)
