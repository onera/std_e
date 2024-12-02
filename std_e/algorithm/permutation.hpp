#pragma once


#include <vector>
#include <algorithm>
#include <numeric>
#include <tuple>
#include "std_e/algorithm/sort_base.hpp"
#include "std_e/utils/dynarray.hpp"


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
template<class Random_access_container, class index_type> constexpr auto
inverse_partial_permutation(const Random_access_container& p, index_type sz, index_type invalid_value) -> Random_access_container {
  index_type p_sz = p.size();
  Random_access_container inv_p(sz,invalid_value);
  for (index_type i=0; i<p_sz; ++i) {
    inv_p[p[i]] = i;
  }
  return inv_p;
}

template<class array_type> constexpr auto
compose_permutations(const array_type& p0, const array_type& p1) {
  // Precond p0 and p1 are compatible permutations ( i.e. image(p0) in domain(p1)=[0,size(p1)[ )
  array_type res(p0.size());
  for (size_t i=0; i<p0.size(); ++i) {
    res[i] = p1[p0[i]];
  }
  return res;
}


template<class Rng, class Comp = std::equal_to<>> auto
unique_permutation(const Rng& x, Comp comp = {}) -> std::vector<int> {
  std::vector<int> p(x.size());
  std::iota(begin(p), end(p), 0);
  auto new_end = std::unique(begin(p),end(p),[&](int i, int j){ return comp(x[i], x[j]); });
  std::fill(new_end,end(p),-1); // TODO -1 -> invalid_value
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
template<class Rand_it0, class S, class Output_it, class Rand_it1> auto
// requires Rand_it0,Rand_it1 are random access iterators
// requires Output_it is an output iterator
permute_copy(Rand_it0 first, S last, Output_it d_first, Rand_it1 perm_first) -> Output_it {
  return permute_copy_n(first,d_first,perm_first,last-first);
}
template<class RA_rng, class Out_rng, class integer_RA_rng> auto
// requires Rand_it0,Rand_it1 are random access iterators
// requires Output_it is an output iterator
permute_copy(const RA_rng& x, Out_rng& dest, const integer_RA_rng& perm) {
  return permute_copy(x.begin(),x.end(),dest.begin(),perm.begin());
}
template<class Rng, class Int_rng> auto
permute_copy(const Rng& x, const Int_rng& p) -> Rng {
  using I = typename Int_rng::value_type;
  I n = p.size();
  Rng res(n);
  permute_copy_n(begin(x),begin(res),begin(p),n);
  return res;
}

template<class Rng, class Int_rng> auto
_permute_cp_algo(Rng&& x, const Int_rng& p) -> void {
  using T = typename Rng::value_type;
  using I = typename Int_rng::value_type;
  I n = p.size();
  std_e::dynarray<T> old_x(x.begin(), x.end());
  permute_copy_n(old_x.begin(), x.begin(), p.begin(), n);
}



// REF: https://stackoverflow.com/a/17074810/1583122
// REF: https://blog.merovius.de/2014/08/12/applying-permutation-in-constant.html
// ALSO: with range-v3 https://stackoverflow.com/a/53785022/1583122
// ALSO: (in place?) https://stackoverflow.com/a/44030600/1583122
// TODO look in e.g. TAOCP for inplace
template<class Rand_it, class Rand_int_it, class I=std::iter_value_t<Rand_int_it>> auto
_permute_inplace(Rand_it it, Rand_int_it p, I sz) -> void {
  std::vector<int8_t> done(sz,0);
  for (I i = 0; i < sz; ++i){
    if (done[i]){
      continue;
    }
    done[i] = 1;
    I prev_j = i;
    I j = p[i];
    while (i != j){
      using std::swap;
      swap(*(it+prev_j),*(it+j));
      done[j] = 1;
      prev_j = j;
      j = p[j];
    }
  }
}
template<class Rng, class Int_rng> auto
_permute_inplace(Rng&& x, const Int_rng& p) -> void {
  return _permute_inplace(x.begin(),p.begin(),p.size());
}

template<class Rng, class Int_rng> auto
permute(Rng&& x, const Int_rng& p) -> void {
  _permute_inplace(x, p);
  // not sure the `permute` algo is really fast
  // here we use the simple algorithm that uses a copy
  //_permute_cp_algo(x, p);
}


// TODO there should be a more efficient implementation
template<class Range, class I> auto
inv_permute(Range& rng, const std::vector<I>& p) -> void {
  std::vector<I> p_inv = inverse_permutation(p);
  return permute(rng,p_inv);
}


template<class Int_range, class Tuple, size_t... Is> auto
apply_permutation__impl(const Int_range& perm, Tuple& rngs, std::index_sequence<Is...>) -> void {
  ( permute(std::get<Is>(rngs),perm) , ... ); // TODO inefficient (instead, permute(zip(rngs)))
}
template<class Int_range, class Tuple> auto
apply_permutation(const Int_range& perm, Tuple& rngs) -> void {
  constexpr auto n_range = std::tuple_size_v<Tuple>;
  apply_permutation__impl(perm,rngs,std::make_index_sequence<n_range>());
}


auto
is_permutation(const auto& x) -> bool {
  using I = std::decay_t<decltype(x)>::value_type;
  const I n = x.size();
  return sorted(x) == iota_dynarray(n);
}

} // std_e
