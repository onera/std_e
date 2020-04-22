#pragma once

#include <vector>
#include <algorithm>
#include "std_e/utils/to_string_fwd.hpp"
#include "std_e/algorithm/merge_twin_copy.hpp"


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


template<class T, class A> auto
append(std::vector<T,A>& v, const std::vector<T,A>& w) -> void {
  std::copy(begin(w),end(w),std::back_inserter(v));
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


template<class T, class A> auto
hash_vector(const std::vector<T,A>& v) -> std::vector<size_t> {
  std::vector<size_t> res(v.size());
  std::transform(begin(v),end(v),begin(res),[](const T& x){ return std::hash<T>{}(x); });
  return res;
}

template<class T, class A> constexpr auto
sort(std::vector<T,A>& v) -> void {
  std::sort(begin(v),end(v));
}
template<class T, class A, class Comp_pred> constexpr auto
sort(std::vector<T,A>& v, Comp_pred cmp) -> void {
  std::sort(begin(v),end(v),cmp);
}
template<class T, class A> constexpr auto
unique(std::vector<T,A>& v) -> void {
  auto new_end = std::unique(begin(v),end(v));
  v.erase(new_end,end(v));
}
template<class T, class A, class Equiv_pred> constexpr auto
sort_unique(std::vector<T,A>& v, Equiv_pred eq) -> void {
  auto new_end = std::unique(begin(v),end(v),eq);
  v.erase(new_end,end(v));
}
template<class T, class A> constexpr auto
sort_unique(std::vector<T,A>& v) -> void {
  sort(v);
  unique(v);
}
template<class T, class A, class Equiv_pred, class Comp_pred> constexpr auto
sort_unique(std::vector<T,A>& v, Equiv_pred eq, Comp_pred cmp) -> void {
  sort(v,cmp);
  unique(v,eq);
}
template<class T, class A, class F> constexpr auto
unique_compress(std::vector<T,A>& v, F compress_while_eq) -> void {
  auto new_end = unique_compress(begin(v),end(v),compress_while_eq);
  v.erase(new_end,end(v));
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


} // std_e
