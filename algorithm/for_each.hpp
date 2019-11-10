#pragma once


#include "std_e/future/algorithm.hpp"


namespace std_e {
/* 
  concept Range_function:
    has operator()(Iterator,Iterator)

  concept Function_generator:
    has Function function_type
    has operator++
    has operator*() -> function_type
*/

template<class Forward_it, class Predicate_generator, class Range_function> constexpr auto
// requires Range_function(Forward_it,Forward_it)
// requires Predicate_generator is a Function_generator
// requires Predicate_generator::function_type(Forward_it::value_type) -> bool
for_each_partition(Forward_it first, Forward_it last, Predicate_generator comparison_generator, Range_function f) {
  auto p_first = first;
  while (p_first!=last) {
    auto p_last = std_e::partition_point(p_first,last,*comparison_generator); // TODO equal_level
    f(p_first,p_last);

    ++comparison_generator;
    p_first = p_last;
  }
}


template<class Forward_it, class Binary_pred> constexpr auto
// requires Binary_pred(Forward_it,Forward_it) -> bool
for_each_equivalent(Forward_it first, Forward_it last, Binary_pred f) -> void {
  if (first==last) return;
  auto first_eq = first;
  while (++first != last) {
    bool equiv = f(first,first_eq);
    if (!equiv) first_eq = first;
  }
}
template<class Forward_it, class Binary_pred> constexpr auto
// requires Binary_pred(Forward_it::value_type,Forward_it::value_type) -> bool
for_each_equivalent_ref(Forward_it first, Forward_it last, Binary_pred f) -> void {
  auto f_iter = [f](auto it0, auto it1){ return f(*it0,*it1); };
  for_each_equivalent(first,last,f_iter);
}


} // std_e
