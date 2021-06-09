#pragma once

#include "std_e/data_structure/multi_range.hpp"
#include "std_e/meta/macro.hpp"

namespace std_e {

// spreadsheet-like class 
// "column" names may be used to create descriptive function names (through use of macros below)
template<class... Ts> using multi_vector2 = multi_vector<Ts...>;

} // std_e


#define STD_E_GEN_MULTI_VECTOR_ACCESSORS_4(multi_vector_type, type0,attr0, type1,attr1) \
  inline auto attr0(const multi_vector_type& x) -> const auto& { return std_e::range<0>(x); } \
  inline auto attr0(      multi_vector_type& x) ->       auto& { return std_e::range<0>(x); } \
  inline auto attr1(const multi_vector_type& x) -> const auto& { return std_e::range<1>(x); } \
  inline auto attr1(      multi_vector_type& x) ->       auto& { return std_e::range<1>(x); } \
  \
  template<class Comp = std::less<>, class sort_algo_type = decltype(std_e::std_sort_lambda)> auto \
    sort_##attr0(multi_vector_type& x, Comp comp = {}, sort_algo_type sort_algo = std_e::std_sort_lambda) \
      -> void { return std_e::sort_by<0>(x); } \

#define STD_E_GEN_MULTI_VECTOR_ACCESSORS_6(multi_vector_type, type0,attr0, type1,attr1, type2,attr2) \
  STD_E_GEN_MULTI_VECTOR_ACCESSORS_4(multi_vector_type, type0,attr0, type1,attr1) \
  \
  inline auto attr2(const multi_vector_type& x) -> const auto& { return std_e::range<2>(x); } \
  inline auto attr2(      multi_vector_type& x) ->       auto& { return std_e::range<2>(x); } \
  \

#define STD_E_GEN_MULTI_VECTOR_ACCESSORS_8(multi_vector_type, type0,attr0, type1,attr1, type2,attr2, type3,attr3) \
  STD_E_GEN_MULTI_VECTOR_ACCESSORS_6(multi_vector_type, type0,attr0, type1,attr1, type2,attr2) \
  \
  inline auto attr3(const multi_vector_type& x) -> const auto& { return std_e::range<3>(x); } \
  inline auto attr3(      multi_vector_type& x) ->       auto& { return std_e::range<3>(x); } \
  \

#define STD_E_MULTI_VECTOR_4(multi_vector_type, type0,attr0, type1,attr1) \
  using multi_vector_type = std_e::multi_vector<type0,type1>; \
  STD_E_GEN_MULTI_VECTOR_ACCESSORS_4(multi_vector_type, type0,attr0, type1,attr1)

#define STD_E_MULTI_VECTOR_6(multi_vector_type, type0,attr0, type1,attr1, type2,attr2) \
  using multi_vector_type = std_e::multi_vector<type0,type1,type2>; \
  STD_E_GEN_MULTI_VECTOR_ACCESSORS_6(multi_vector_type, type0,attr0, type1,attr1, type2,attr2)

#define STD_E_MULTI_VECTOR_8(multi_vector_type, type0,attr0, type1,attr1, type2,attr2, type3,attr3) \
  using multi_vector_type = std_e::multi_vector<type0,type1,type2>; \
  STD_E_GEN_MULTI_VECTOR_ACCESSORS_6(multi_vector_type, type0,attr0, type1,attr1, type2,attr2, type3,attr3)


#define STD_E_MULTI_VECTOR(multi_vector_type, ...) \
  static_assert(NB_VA_ARGS(__VA_ARGS__)%2 == 0,"STD_E_MULTI_VECTOR arguments must alternate types and names"); \
  static_assert(NB_VA_ARGS(__VA_ARGS__)/2 >= 2,"STD_E_MULTI_VECTOR implemented only for 2, 3, or 4 columns"); \
  static_assert(NB_VA_ARGS(__VA_ARGS__)/2 <= 4,"STD_E_MULTI_VECTOR implemented only for 2, 3, or 4 columns"); \
  MACRO_NAME_SUFFIXED_BY_NB_VA_ARGS(STD_E_MULTI_VECTOR_,__VA_ARGS__)(multi_vector_type, __VA_ARGS__)
