#pragma once


#include <algorithm>
#include <vector>


namespace std_e {


template<class Input_it, class Output_it, class Random_it> constexpr auto
// requires Input_it::value_type == Random_it
// requires Output_it::value_type == Integer
iterators_to_indices(Input_it first, Input_it last, Output_it d_first, Random_it start) -> void {
  std::transform(first,last,d_first,[=](const auto& it){
    return it - start;
  });
}

template<class Input_it, class Output_it, class Random_it> constexpr auto
// requires Input_it::value_type == Integer
// requires Output_it::value_type == Random_it
indices_to_iterators(Input_it first, Input_it last, Output_it d_first, Random_it start) -> void {
  std::transform(first,last,d_first,[=](const auto& it){
    return start + it;
  });
}

template<class Input_it, class Output_it, class Random_it0, class Random_it1> constexpr auto
// requires Input_it::value_type == Random_it0
// requires Output_it::value_type == Random_it1
iterators_to_iterators(Input_it first, Input_it last, Output_it d_first, Random_it0 start, Random_it1 d_start) -> void {
  std::transform(first,last,d_first,[=](const auto& it){
    int index = it - start;
    return d_start + index;
  });
}


template<class Fwd_it0, class Fwd_it1, class Random_it0, class Random_it1> constexpr auto
// requires Fwd_it0::value_type == Random_it0
// requires Fwd_it1::value_type == Random_it1
equal_iterator_shifts(Fwd_it0 first0, Fwd_it0 last0, Fwd_it1 first1, Random_it0 start0, Random_it1 start1) -> bool {
  while (first0 != last0) {
    auto index0 = *first0 - start0;
    auto index1 = *first1 - start1;
    if (index0 != index1) return false;
    ++first0;
    ++first1;
  }
  return true;
}
template<class Fwd_it0, class Fwd_it1, class Random_it0, class Random_it1> constexpr auto
// requires Fwd_it0::value_type == Random_it0
// requires Fwd_it1::value_type == Random_it1
equal_iterator_shifts(Fwd_it0 first0, Fwd_it0 last0, Fwd_it1 first1, Fwd_it1 last1, Random_it0 start0, Random_it1 start1) -> bool {
  auto n0 = std::distance(first0,last0);
  auto n1 = std::distance(first1,last1);
  if (n0!=n1) return false;
  return equal_iterator_shifts(first0,last0,first1,start0,start1);
}




template<class Random_it, class Random_const_it, class Integer=int> constexpr auto
// requires const Random_it == Random_const_it
iterators_to_indices(const std::vector<Random_it>& v, Random_const_it start) -> std::vector<Integer> {
  std::vector<Integer> indices(v.size());
  iterators_to_indices(begin(v),end(v),begin(indices),start);
  return indices;
}

template<class Integer, class Random_const_it, class Random_it=std::remove_const_t<Random_const_it>> constexpr auto
indices_to_iterators(const std::vector<Integer>& indices, Random_const_it start) -> std::vector<Random_it> {
  std::vector<Random_it> res(indices.size());
  indices_to_iterators(begin(indices),end(indices),begin(res),start);
  return res;
}

template<class Random_it0, class Random_const_it0, class Random_const_it1, class Random_it1=std::remove_const_t<Random_const_it1>> constexpr auto
// requires const Random_it0 == Random_const_it0
// requires const Random_it1 == Random_const_it1
iterators_to_iterators(const std::vector<Random_it0>& v, Random_const_it0 start, Random_const_it1 d_start) -> std::vector<Random_it1> {
  std::vector<Random_it1> res(v.size());
  iterators_to_iterators(begin(v),end(v),begin(res),start,d_start);
  return res;
}


template<class Random_it0, class Random_it1, class Random_const_it0, class Random_const_it1> constexpr auto
// requires const Random_it0 == Random_const_it0
// requires const Random_it1 == Random_const_it1
equal_iterator_shifts(const std::vector<Random_it0>& v0, const std::vector<Random_it1>& v1, Random_const_it0 start0, Random_const_it1 start1) -> bool {
  return equal_iterator_shifts(begin(v0),end(v0),begin(v1),end(v1),start0,start1);
}


} // std_e
