#if __cplusplus > 201703L
#pragma once


#include <cstddef>
#include <algorithm>
#include <ranges>
#include "std_e/future/concept.hpp"
#include "std_e/future/ranges/concept.hpp"


namespace std_e {

template<class I, class Comp> auto
swap_median_of_3_pivot_first(I a, I b, I c, Comp comp) -> void {
  if (comp(*a,*b))
    if (comp(*b,*c)) return std::ranges::iter_swap(a,b);
    else if (comp(*a,*c)) return std::ranges::iter_swap(a,c);
    else return;
  else if (comp(*a,*c)) return;
  else if (comp(*b,*c)) return std::ranges::iter_swap(a,c);
  else return std::ranges::iter_swap(a,b);
}

template<class I, class Comp> auto
swap_median_of_5_pivot_first(I a, I b, I c, I d, I e, Comp comp) -> void {
  auto comp_deref = [comp](I x, I y){ return comp(*x,*y); };
  I f = max( min(b,c,comp_deref) , min(d,e,comp_deref) , comp_deref ); // discards lowest from last 4
  I g = min( max(b,c,comp_deref) , max(d,e,comp_deref) , comp_deref ); // discards biggest from last 4
  return swap_median_of_3_pivot_first(a,f,g,comp);
}


// ========== From Stepanov ==========
template<class I, class Comp> auto
unguarded_linear_insert(I last, std::iter_value_t<I> value, Comp comp) -> void {
  I previous = last-1;
  while (comp(value,*previous)) {
    *last = std::ranges::iter_move(previous);
    last = previous;
    --previous;
  }
  *last = std::move(value);
}

template<class I, class Comp> auto
unguarded_insertion_sort(I first, I last, Comp comp) -> void {
  for (I i = first; i != last; ++i) {
    unguarded_linear_insert(i, std::ranges::iter_move(i), comp);
  }
}

template<class I, class Comp> auto
linear_insert(I first, I last, std::iter_value_t<I> value, Comp comp) -> void {
  if (comp(value,*first)) {
    while (first != last--) {
      *(last+1) = std::ranges::iter_move(last);
    }
    *first = std::move(value);
  }
  else unguarded_linear_insert(last, value, comp);
}

template<class I, class Comp> auto
insertion_sort(I first, I last, Comp comp) -> void {
  if (first == last) return;
  for (I i = first+1; i != last; ++i) {
    linear_insert(first, i, std::ranges::iter_move(i), comp);
  }
}

//template<class T, class Comp> auto
//median_of_3(const T& a, const T& b, const T& c, Comp comp) -> const T& {
//  if (comp(a,b))
//    if (comp(b,c)) return b;
//    else if (comp(a,c)) return c;
//    else return a;
//  else if (comp(a,c)) return a;
//  else if (comp(b,c)) return c;
//  else return b;
//}

template<class I, class Comp> auto
unguarded_partition(I first, I last, std::iter_value_t<I> pivot, Comp comp) -> I {
  --last;
  while (comp(*first,pivot)) ++first;
  while (comp(pivot,*last)) --last;
  while (first < last) {
    std::ranges::iter_swap(first,last);
    ++first; --last;
    while (comp(*first,pivot)) ++first;
    while (comp(pivot,*last)) --last;
  }
  return first;
}

template<class I, class Comp> auto
quicksort_loop(I first, I last, Comp comp, ptrdiff_t threshold) -> int {
  ptrdiff_t len = last - first;
  int k=0;
  while (len > threshold && k<300) {
    I middle = first + (last-first)/2;
    swap_median_of_3_pivot_first(first, middle, last-1, comp);
    //swap_median_of_3_pivot_first(first + (last-first)/6, middle, first + 5*(last-first)/6, comp);
    //swap_median_of_5_pivot_first(first,first + (last-first)/4,first + (last-first)/2,first + 3*(last-first)/4,last-1,comp);
    std::iter_value_t<I> pivot = *first;
    I cut = unguarded_partition(first, last, pivot, comp);
    if (last - cut < cut - first) {
      quicksort_loop(cut+1, last, comp, threshold);
      last = cut;
    } else {
      quicksort_loop(first, cut, comp, threshold);
      first = cut+1;
    }
    len = last - first;
    ++k;
  }
  return k;
}

template<class I, class Comp = std::less<>> auto
quicksort(I first, I last, Comp comp = {}) -> void {
  constexpr ptrdiff_t threshold = 16;
  ptrdiff_t len = last - first;
  if (len <= threshold) {
    insertion_sort(first, last, comp);
  } else {
    quicksort_loop(first, last, comp, threshold);
    I middle = first + threshold;
    insertion_sort(first, middle, comp);
    unguarded_insertion_sort(middle, last, comp);
  }
}
// ========== From Stepanov - end ==========

//
//
//
////template<class T, class Comp = std::less<>> auto
////median_of_3(const T& a, const T& b, const T& c, Comp comp = {}) -> const T& {
////  if (a < b)
////    if (b < c) return b;
////    else if (a < c) return c;
////    else return a;
////  else if (a < c) return a;
////  else if (b < c) return c;
////  else return b;
////}
////template<class I, class Comp = std::less<>> auto
////median_of_3(I a, I b, I c, Comp comp = {}) -> I {
////  if (*a,*b)
////    if (*b,*c) return b;
////    else if (*a,*c) return c;
////    else return a;
////  else if (*a,*c) return a;
////  else if (*b,*c) return c;
////  else return b;
////}

//
//
//template<class I, class Comp = std::less<>> auto
//quicksort_impl(I first, I last, Comp comp = {}) -> int {
//  I next = std::next(first);
//
//  if (first == last) return 0;
//  if (next  == last) return 0;
//
//  swap_median_of_3_pivot_first(first,first + (last-first)/2,last-1,comp);
//  //swap_median_of_5_pivot_first(first,first + (last-first)/4,first + (last-first)/2,first + 3*(last-first)/4,last-1,comp);
//  decltype(auto) pivot = *first;
//  auto less_than_pivot = [&pivot,comp](const auto& value){ return comp(value,pivot); };
//
//  I partition_pt = std::ranges::partition(next,last,less_than_pivot).begin();
//  I one_before_partition_pt = std::prev(partition_pt);
//
//  std::ranges::iter_swap(first,one_before_partition_pt);
//  auto l = quicksort_impl(first,one_before_partition_pt);
//  auto r = quicksort_impl(partition_pt,last);
//  return std::max(l,r)+1;
//}
//
//template<class I, class Comp = std::less<>> auto
//quicksort(I first, I last, Comp comp = {}) -> void {
//  int n_iter = quicksort_impl(first,last,comp);
//}

template<typename Comp, typename Proj> constexpr auto
make_comp_proj(Comp& comp, Proj& proj) {
  return [&] (auto&& x, auto&& y) -> bool {
    using T0 = decltype(x);
    using T1 = decltype(y);
    return std::invoke(comp,
                       std::invoke(proj, std::forward<T0>(x)),
                       std::invoke(proj, std::forward<T1>(y)));
  };
}


namespace ranges {

struct _sort_fn {
  template<std::random_access_iterator It, std::sentinel_for<It> S, class Comp = std_e::ranges::less, class Proj = std::identity>
    requires std_e::sortable<It, Comp, Proj>
      constexpr auto
  operator()(It first, S last, Comp comp = {}, Proj proj = {}) const -> It {
    auto last_it = std_e::ranges::next(first, last);
    quicksort(first, last_it, make_comp_proj(comp, proj));
    return last_it;
  }

  template<std_e::ranges::random_access_range Range, class Comp = std_e::ranges::less, class Proj = std::identity>
    requires std_e::sortable<std::ranges::iterator_t<Range>, Comp, Proj>
      constexpr auto
  operator()(Range&& r, Comp comp = {}, Proj proj = {}) const -> std::ranges::borrowed_iterator_t<Range> {
    return (*this)(std::ranges::begin(r), std::ranges::end(r), std::move(comp), std::move(proj));
  }
};

inline constexpr _sort_fn sort{};

} // ranges

} // std_e
#endif // C++>17
