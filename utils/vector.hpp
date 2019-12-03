#pragma once

#include <vector>
#include <algorithm>


namespace std_e {


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


template<class T> constexpr auto
hash_vector(const std::vector<T>& v) -> std::vector<size_t> {
  std::vector<size_t> res(v.size());
  std::transform(begin(v),end(v),begin(res),[](const T& x){ return std::hash<T>{}(x); });
  return res;
}


} // std_e
