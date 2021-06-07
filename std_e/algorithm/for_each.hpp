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

template<class Forward_it, class S, class Predicate_generator, class Range_function> constexpr auto
// requires Range_function(Forward_it,Forward_it)
// requires Predicate_generator is a Function_generator
// requires Predicate_generator::function_type(Forward_it::value_type) -> bool
for_each_partition(Forward_it first, S last, Predicate_generator comparison_generator, Range_function f) {
  auto p_first = first;
  while (p_first!=last) {
    auto p_last = std_e::partition_point(p_first,last,*comparison_generator);
    f(p_first,p_last);

    ++comparison_generator;
    p_first = p_last;
  }
}


template<class Forward_it, class S, class Unary_pred, class F> constexpr auto
for_each_if(Forward_it first, S last, Unary_pred p, F f) -> void {
  auto f_cond = [p,f](auto& x){ if (p(x)) f(x); };
  for_each(first,last,f_cond);
}
template<class Forward_it, class S, class Unary_pred, class F> constexpr auto
for_each_until(Forward_it first, S last, Unary_pred p, F f) -> Forward_it {
  while (first!=last && p(*first)) {
    f(*first);
    ++first;
  }
  return first;
}


// TODO RENAME replace_by_first_equivalent
template<class Forward_it, class S, class Binary_pred> constexpr auto
// requires Binary_pred(Forward_it,Forward_it) -> bool
replace_by_first_equivalent(Forward_it first, S last, Binary_pred f) -> void {
  if (first==last) return;
  auto first_eq = first;
  while (++first != last) {
    bool equiv = f(first,first_eq);
    if (!equiv) first_eq = first;
  }
}
template<class Forward_it, class S, class Binary_pred> constexpr auto
// requires Binary_pred(Forward_it::value_type,Forward_it::value_type) -> bool
replace_by_first_equivalent_ref(Forward_it first, S last, Binary_pred f) -> void {
  auto f_iter = [f](auto it0, auto it1){ return f(*it0,*it1); };
  replace_by_first_equivalent(first,last,f_iter);
}


} // std_e
