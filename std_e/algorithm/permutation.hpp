#pragma once


#include <vector>
#include <algorithm>
#include <numeric>


namespace std_e {


template<class Random_access_container> constexpr auto
inverse_permutation(const Random_access_container& p) -> Random_access_container {
  using index_type = typename Random_access_container::difference_type;
  index_type sz = p.size();
  Random_access_container inv_p(sz);
  for (index_type i=0; i<sz; ++i) {
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


template<class Rand_it0, class Output_it, class Rand_it1, class I> auto
// requires Rand_it0,Rand_it1 are random access iterators
// requires Output_it is an output iterator
permute_copy_n(Rand_it0 first, Output_it d_first, Rand_it1 perm_first, I n) -> Output_it {
  // Write permutation of range [first,n) at [d_first,n)
  // Preconditions:
  //     - [first,first+n) is a valid range
  //     - [d_first,d_first+n) is a valid range
  //     - permutation_indices = [perm_first,n) is an index permutation (i.e. sort(permutation_indices) == iota(n))
  return std::transform(perm_first, perm_first+n, d_first, [&](I i){ return *(first+i); });
}
template<class T, class I> auto
permute_copy(const std::vector<T>& v, const std::vector<I>& p) -> std::vector<T> {
  using index_type = typename std::vector<I>::difference_type;
  index_type n = p.size();
  std::vector<T> w(n);
  permute_copy_n(begin(v),begin(w),begin(p),n);
  return w;
}


// TODO Bruno: reference?
template<class Rand_it, class I> auto
permute(Rand_it it, const std::vector<I>& p) -> void {
  int sz = p.size();
  std::vector<bool> done(sz);
  for (I i = 0; i < sz; ++i){
    if (done[i]){
      continue;
    }
    done[i] = true;
    I prev_j = i;
    I j = p[i];
    while (i != j){
      std::swap(*(it+prev_j),*(it+j));
      done[j] = true;
      prev_j = j;
      j = p[j];
    }
  }
}
template<class T, class I> auto
permute(std::vector<T>& vec, const std::vector<I>& p) -> void {
  return permute(vec.begin(),p);
}


} // std_e
