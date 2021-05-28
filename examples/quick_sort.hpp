#pragma once


#include <algorithm>


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

template<class I, class P> auto
// requires I is bidirectional iterator
quick_sort(I first, I last, P pred) -> void {
  I next = std::next(first);

  if (first == last) return;
  if (next  == last) return;

  auto pivot = swap_pivot_with_first(first,last);
  auto less_than_pivot = [pred,&pivot](const auto& value){ return pred(value , pivot); };

  auto partition_point = bidirectional_partition(next,last,less_than_pivot);
  I one_before_partition_point = std::prev(partition_point);

  std::iter_swap(first,one_before_partition_point);
  quick_sort(first, one_before_partition_point,pred);
  quick_sort(partition_point,last,pred);
}
