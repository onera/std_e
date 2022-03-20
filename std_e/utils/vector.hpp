#pragma once

#include <utility>
#include <vector>
#include <algorithm>
#include <numeric>
#include "std_e/utils/to_string_fwd.hpp"
#include "std_e/algorithm/permutation.hpp"
#include "std_e/utils/functional.hpp"


namespace std_e {


template<class T, class A> constexpr auto
begin_ptr(std::vector<T,A>& v) -> T* {
  return v.data();
}
template<class T, class A> constexpr auto
begin_ptr(const std::vector<T,A>& v) -> const T* {
  return v.data();
}
template<class T, class A> constexpr auto
end_ptr(std::vector<T,A>& v) -> T* {
  return v.data()+v.size();
}
template<class T, class A> constexpr auto
end_ptr(const std::vector<T,A>& v) -> const T* {
  return v.data()+v.size();
}


template<class T, class A, class Unary_predicate> constexpr auto
partition_copy(const std::vector<T,A>& v, Unary_predicate p) -> std::pair<std::vector<T,A>,std::vector<T,A>> {
  std::vector<T,A> v_true;
  std::vector<T,A> v_false;
  std::partition_copy(
    begin(v),end(v),
    back_inserter(v_true),
    back_inserter(v_false),
    p
  );
  return make_pair(std::move(v_true),std::move(v_false));
}


template<class T, class A, class F> constexpr auto
for_each(std::vector<T,A>& x, F f) -> void {
  std::for_each(begin(x),end(x),f);
}
template<class T, class A, class F> constexpr auto
for_each(const std::vector<T,A>& x, F f) -> void {
  std::for_each(begin(x),end(x),f);
}
template<class Range, class F, class T = typename Range::value_type, class RT = std::invoke_result_t<F,T> > constexpr auto
transform(const Range& x, F f) -> std::vector<RT> {
  std::vector<RT> res(x.size());
  std::transform(begin(x),end(x),begin(res),f);
  return res;
}
template<class Range> constexpr auto
transform(const Range& x, identity_closure) -> const Range& {
  return x;
}


template<class T, class A> auto
hash_vector(const std::vector<T,A>& v) -> std::vector<size_t> {
  std::vector<size_t> res(v.size());
  std::transform(begin(v),end(v),begin(res),[](const T& x){ return std::hash<T>{}(x); });
  return res;
}

template<class Rng, class Comp_pred = std::less<>> constexpr auto
sort(Rng& v, Comp_pred cmp = {}) -> void {
  std::sort(begin(v),end(v),cmp);
}
template<class T, class A, class Comp_pred = std::less<>> constexpr auto
stable_sort(std::vector<T,A>& v, Comp_pred cmp = {}) -> void {
  std::stable_sort(begin(v),end(v),cmp);
}

template<class T, class A, class Equiv_pred = std::equal_to<>> constexpr auto
unique(std::vector<T,A>& v, Equiv_pred eq = {}) -> void {
  auto new_end = std::unique(begin(v),end(v), eq);
  v.erase(new_end,end(v));
}

template<class T, class A, class Equiv_pred = std::equal_to<>, class Comp_pred = std::less<>> constexpr auto
sort_unique(std::vector<T,A>& v, Equiv_pred eq = {}, Comp_pred cmp = {}) -> void {
  sort(v,cmp);
  unique(v,eq);
}
template<class T, class A, class Equiv_pred = std::equal_to<>, class Comp_pred = std::less<>> constexpr auto
sort_unique_permutation(std::vector<T,A>& x, Equiv_pred eq = {}, Comp_pred cmp = {}) {
  auto p = sort_permutation(x,cmp);
  std_e::unique(p,[&](int i, int j){ return eq(x[i], x[j]); });
  return p;
}

template<class T, class A, class I> constexpr auto
make_sub_vector(const std::vector<T,A>& x, I start, I sub_size) {
  std::vector<T,A> sub(sub_size);
  std::copy_n(begin(x)+start,sub_size,begin(sub));
  return sub;
}


template<class T, class A> auto
to_string(const std::vector<T,A>& x) -> std::string {
  return range_to_string(x);
}

template<class I> auto
iota(I n, I init = I()) -> std::vector<I> {
  std::vector<I> res(n);
  std::iota(begin(res), end(res), init);
  return res;
}

template<class It> auto
move_to_vector(It first, It last) {
  using T = typename std::iterator_traits<It>::value_type;
  auto n = std::distance(first,last);
  std::vector<T> res(n);
  std::move(first,last,res.begin());
  return res;
}
template<class T, class... Ts> auto
make_vector(T&& x, Ts&&... xs) -> std::vector<std::remove_const_t<std::remove_reference_t<T>>> {
  std::vector<std::remove_const_t<std::remove_reference_t<T>>> res;
  res.emplace_back(std::forward<T>(x));
  ( res.emplace_back(std::forward<Ts>(xs)) , ...);
  return res;
}


} // std_e
