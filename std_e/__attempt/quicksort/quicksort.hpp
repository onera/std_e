#include <random>
#include <iostream>
#include <limits>

#include <chrono>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>


namespace std_e {


// utility
auto random_vector(int size) {
  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

  auto imin = std::numeric_limits<int>::min();
  auto imax = std::numeric_limits<int>::max();
  std::uniform_int_distribution<> dist(imin,imax);

  std::vector<int> x(size);

  for (int n=0; n<size; ++n) {
    // Use dist to transform the random unsigned int generated by gen into an int in [imin,imax]
    x[n] = dist(gen);
  }
  return x;
}




// algorithms

template<class I, class Pred> inline auto
// requires I is bidirectional iterator
bidirectional_partition(I first, I last, Pred p) -> I {
  if (first == last) return last;

  auto from_begin = first;
  auto from_end = --last;

  while (from_begin != from_end) {
    while ( (from_begin != from_end) &&  p(*from_begin) ) { ++from_begin; }
    while ( (from_begin != from_end) && !p(*from_end)   ) { --from_end; }
    std::iter_swap(from_begin,from_end);
  }
  if (p(*from_begin)) { ++from_begin; }
  return from_begin;
}




template<class I> auto
// requires I is iterator
swap_pivot_with_first(I first, I) {
  return *first;
  // nothing to swap since the chosen pivot is first
}




template<class I> auto
// requires I is bidirectional iterator
quicksort(I first, I last) -> void {
  I next = std::next(first);

  if (first == last) return;
  if (next  == last) return;

  auto pivot = swap_pivot_with_first(first,last);
  auto less_than_pivot = [&pivot](const auto& value){ return value < pivot; };

  auto partition_point = bidirectional_partition(next,last,less_than_pivot);
  I one_before_partition_point = std::prev(partition_point);

  std::iter_swap(first,one_before_partition_point);
  quicksort(first, one_before_partition_point);
  quicksort(partition_point,last);
}


} // std_e
