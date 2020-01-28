#pragma once

#include <vector>
#include <algorithm>


namespace std_e {


template<class T> constexpr auto
begin_ptr(std::vector<T>& v) -> T* {
  return v.data();
}
template<class T> constexpr auto
begin_ptr(const std::vector<T>& v) -> const T* {
  return v.data();
}
template<class T> constexpr auto
end_ptr(std::vector<T>& v) -> T* {
  return v.data()+v.size();
}
template<class T> constexpr auto
end_ptr(const std::vector<T>& v) -> const T* {
  return v.data()+v.size();
}


template<class T> auto
append(std::vector<T>& v, const std::vector<T>& w) -> void {
  std::copy(begin(w),end(w),std::back_inserter(v));
}


template<class T, class Unary_predicate> constexpr auto
partition_copy(const std::vector<T>& v, Unary_predicate p) -> std::pair<std::vector<T>,std::vector<T>> {
  std::vector<T> v_true;
  std::vector<T> v_false;
  std::partition_copy(
    begin(v),end(v),
    std::back_inserter(v_true),
    std::back_inserter(v_false),
    p
  );
  return make_pair(std::move(v_true),std::move(v_false));
}


template<class T, class F> constexpr auto
for_each(std::vector<T>& x, F f) -> void {
  std::for_each(begin(x),end(x),f);
}
template<class T, class F> constexpr auto
for_each(const std::vector<T>& x, F f) -> void {
  std::for_each(begin(x),end(x),f);
}


template<class T> auto
hash_vector(const std::vector<T>& v) -> std::vector<size_t> {
  std::vector<size_t> res(v.size());
  std::transform(begin(v),end(v),begin(res),[](const T& x){ return std::hash<T>{}(x); });
  return res;
}

template<class T> constexpr auto
sort_unique(std::vector<T>& v) -> void {
  std::sort(begin(v),end(v));
  auto new_end = std::unique(begin(v),end(v));
  v.erase(new_end,end(v));
}
template<class T, class Equiv_pred, class Comp_pred> constexpr auto
sort_unique(std::vector<T>& v, Equiv_pred eq, Comp_pred cmp) -> void {
  std::sort(begin(v),end(v),cmp);
  auto new_end = std::unique(begin(v),end(v),eq);
  v.erase(new_end,end(v));
}




} // std_e
