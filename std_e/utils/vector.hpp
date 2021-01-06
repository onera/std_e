#pragma once

#include <vector>
#include <algorithm>
#include <numeric>
#include "std_e/utils/to_string_fwd.hpp"
#include "std_e/algorithm/permutation.hpp"
#include "std_e/algorithm/unique_compress.hpp"
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
    std::back_inserter(v_true),
    std::back_inserter(v_false),
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

template<class T, class A, class Comp_pred = std::less<>> constexpr auto
sort(std::vector<T,A>& v, Comp_pred cmp = {}) -> void {
  std::sort(begin(v),end(v),cmp);
}

template<class T, class A, class Equiv_pred = std::equal_to<>> constexpr auto
unique(std::vector<T,A>& v, Equiv_pred eq = {}) -> void {
  auto new_end = std::unique(begin(v),end(v), eq);
  v.erase(new_end,end(v));
}

// TODO DEL? (wrong: no sort)
//template<class T, class A, class Equiv_pred> constexpr auto
//sort_unique(std::vector<T,A>& v, Equiv_pred eq) -> void {
//  auto new_end = std::unique(begin(v),end(v),eq);
//  v.erase(new_end,end(v));
//}

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

template<class T, class A, class F> constexpr auto
unique_compress(std::vector<T,A>& v, F compress_while_eq) -> void {
  auto new_end = unique_compress(begin(v),end(v),compress_while_eq);
  v.erase(new_end,end(v));
}
template<class T, class A, class F> constexpr auto
unique_compress_copy(std::vector<T,A>& v, F compress_while_eq) -> std::vector<T> {
  std::vector<T> res;
  unique_compress_copy(begin(v),end(v),std::back_inserter(res),compress_while_eq);
  return res;
}
template<class I, class T, class A, class F> constexpr auto
unique_compress_copy_with_index_position(std::vector<T,A>& v, F compress_while_eq) -> std::pair<std::vector<T>,std::vector<I>> {
  std::vector<T> compress_res;
  std::vector<I> position_res;
  I position_init = 0;
  unique_compress_copy_with_index_position(
    begin(v),end(v),
    std::back_inserter(compress_res),
    position_init,std::back_inserter(position_res),
    compress_while_eq
  );
  return std::make_pair(compress_res,position_res);
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


} // std_e
