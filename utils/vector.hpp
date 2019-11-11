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


} // std_e
