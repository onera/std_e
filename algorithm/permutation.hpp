#pragma once


#include <vector>
#include <algorithm>
#include <numeric>


namespace std_e {


template<class Array> constexpr auto
invert_permutation(const Array& p) -> Array {
  using indexing_type = typename Array::difference_type;
  indexing_type sz = p.size();
  Array inv_p(sz);
  for (indexing_type i=0; i<sz; ++i) {
    inv_p[p[i]] = i;
  }
  return inv_p;
}




template<class T> auto
sort_permutation(const std::vector<T>& v) -> std::vector<int> {
  std::vector<int> p(v.size());
  std::iota(p.begin(), p.end(), 0);
  std::sort(p.begin(), p.end(), [&](int i, int j){ return v[i] < v[j]; });
  return p;
}

template<class T, class Comp> auto
sort_permutation(const std::vector<T>& v, Comp comp) -> std::vector<int> {
  std::vector<int> p(v.size());
  std::iota(p.begin(), p.end(), 0);
  std::sort(p.begin(), p.end(), [&](int i, int j){ return comp(v[i], v[j]); });
  return p;
}


template<class T> auto
permute_copy(const std::vector<T>& v, const std::vector<int>& p) -> std::vector<T> {
  std::vector<T> w(v.size());
  std::transform(p.begin(), p.end(), w.begin(), [&](int i){ return v[i]; });
  return w;
}

// TODO Bruno: reference?
template<class T> auto
permute(std::vector<T>& vec, const std::vector<int>& p) -> void {
  int sz = vec.size();
  std::vector<bool> done(sz);
  for (int i = 0; i < sz; ++i){
    if (done[i]){
      continue;
    }
    done[i] = true;
    int prev_j = i;
    int j = p[i];
    while (i != j){
      std::swap(vec[prev_j], vec[j]);
      done[j] = true;
      prev_j = j;
      j = p[j];
    }
  }
}



} // std_e
