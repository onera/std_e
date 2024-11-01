#pragma once


#include <algorithm>
#include "std_e/algorithm/overlapping.hpp"
#include "std_e/algorithm/interval.hpp"


namespace std_e {


// Preconditions:
//   - `arr_off` is an index interval of `arr`
//   - function `f` has the following property:
//       if called with: `d_last = f(first,last,d_first)`
//       then d_last-d_first <= first-last
//     in other words, the output range is of smaller or equal size compared to the input range
// Implementation precondition (could be reworked)
//   - `arr.resize` must exist
//   - `arr_off` starts at 0
template<class Integer_rng, class Rng, class F> auto
for_each_block(Integer_rng& arr_off, Rng& arr, F&& f) -> void {
  using I = typename Integer_rng::value_type;
  using T = typename Rng::value_type;
  const I n_block = arr_off.size()-1;

  T* first   = arr.data();
  T* d_first = arr.data();
  for (I i=0; i<n_block; ++i) {
    T* next = arr.data()+arr_off[i+1];
    // loop invariants:
    //     d_first <= first
    //     first <= next
    //     arr_off[i]   = d_first - arr.data()
    //     arr_off[i+1] has not been yet modified

    auto d_next = f(first, next, d_first);
    I off = d_next - arr.data();
    
    first = next;
    d_first = d_next;
    arr_off[i+1] = off;
  }

  arr.resize(arr_off[n_block]);
}

template<class Integer_rng, class Rng> auto
sort_unique_by_block(Integer_rng& arr_off, Rng& arr) -> void {
  using T = typename Rng::value_type;
  auto f = [](T* first, T* last, T* d_first) {
    std::sort(first, last);
    return unique(first, last, d_first);
  };
  for_each_block(arr_off, arr, f);
}


template<class Integer_rng, class Rng, class Pred> auto
remove_if_by_block(Integer_rng& arr_off, Rng& arr, Pred&& p) -> void {
  using T = typename Rng::value_type;
  auto f = [&p](T* first, T* last, T* d_first) {
    return remove_if(first,last,d_first,p);
  };
  for_each_block(arr_off, arr, f);
}

template<class Integer_rng, class Rng, class T> auto
remove_by_block(Integer_rng& arr_off, Rng& arr, const T& value) -> void {
  auto p = [&value](const auto& x){ return x==value; };
  remove_if_by_block(arr_off, arr, p);
}


template<class Integer_rng_0, class Rng, class Integer_rng_1> [[nodiscard]] auto
permute_copy_by_block(const Integer_rng_0& arr_off, const Rng& arr, const Integer_rng_1& new_to_old)
{
  using I = typename Integer_rng_0::value_type;
  I n_node = arr_off.size()-1;

  std_e::dynarray<I> connectivity_bsz(n_node+1); // `n_node+1` because reused below for `_off`
  for (I i=0; i<n_node; ++i) {
    I bsz = arr_off[new_to_old[i] + 1] - arr_off[new_to_old[i]];
    connectivity_bsz[i] = bsz;
  }
  auto& new_arr_off = std_e::inplace_size_to_index(connectivity_bsz); // reuse the buffer

  std_e::dynarray<I> new_arr(arr.size());
  for (I i=0; i<n_node; ++i) {
    I bsz = arr_off[new_to_old[i] + 1] - arr_off[new_to_old[i]];
    for (I j=0; j<bsz; ++j) {
      new_arr[new_arr_off[i] + j] = arr[arr_off[new_to_old[i]] + j];
    }
  }

  return std::make_tuple(std::move(new_arr_off), std::move(new_arr));
}

template<class Integer_rng_0, class Rng, class Integer_rng_1> auto
permute_by_block(Integer_rng_0& arr_off, Rng& arr, const Integer_rng_1& new_to_old) -> void
{
  auto [new_arr_off, new_arr] = permute_copy_by_block(arr_off, arr, new_to_old);
  std::ranges::copy(new_arr_off, arr_off.data());
  std::ranges::copy(new_arr    , arr    .data());
}


} // std_e
