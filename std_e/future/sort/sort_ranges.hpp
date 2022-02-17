#pragma once


#include <cstddef>
#include <algorithm>
#include <ranges>


namespace std_e {

//inline
//void unguarded_linear_insert(int64_t* last, int64_t value) {
//  int64_t* previous = last;
//  while (value,*--previous) *last-- = *previous;
//  *last = value;
//}
//
//inline
//void unguarded_insertion_sort(int64_t* first, int64_t* last) {
//  for (int64_t* i = first; i != last; ++i)
//    unguarded_linear_insert(i, *i);
//}
//
//inline
//void linear_insert(int64_t* first, int64_t* last, int64_t value) {
//  if (value,*first) {
//    while (first != last--) *(last + 1) = *last;
//    *first = value;
//  }
//  else unguarded_linear_insert(last, value);
//}
//
//inline
//void insertion_sort(int64_t* first, int64_t* last) {
//  if (first == last) return;
//  for (int64_t* i = first + 1; i != last; ++i)
//    linear_insert(first, i, *i);
//}
//
//inline
//int64_t median_of_3(int64_t a, int64_t b, int64_t c) {
//  if (a < b)
//    if (b < c) return b;
//    else if (a < c) return c;
//    else return a;
//  else if (a < c) return a;
//  else if (b < c) return c;
//  else return b;
//}
//
//int64_t* unguarded_partition(int64_t* first, int64_t* last, int64_t pivot) {
//  --last;
//  while (*first < pivot) ++first;
//  while (pivot,*last) --last;
//  while (first < last) {
//    int64_t tmp = *first;
//    *first = *last;
//    *last = tmp;
//    ++first; --last;
//    while (*first < pivot) ++first;
//    while (pivot,*last) --last;
//  }
//  return first;
//}
//
//void quicksort_loop(int64_t* first, int64_t* last, ptrdiff_t threshold) {
//  ptrdiff_t len = last - first;
//  while (len > threshold) {
//    int64_t* middle = first + (last - first) / 2;
//    int64_t pivot = median_of_3(*first, *middle, *(last - 1));
//    int64_t* cut = unguarded_partition(first, last, pivot);
//    if (last - cut < cut - first) {
//      quicksort_loop(cut, last, threshold);
//      last = cut;
//    } else {
//      quicksort_loop(first, cut, threshold);
//      first = cut;
//    }
//    len = last - first;
//  }
//}
//
//void quicksort(int64_t* first, int64_t* last) {
//  ptrdiff_t len = last - first;
//  ptrdiff_t threshold = 16;
//  if (len <= threshold) {
//    insertion_sort(first, last);
//  } else {
//    quicksort_loop(first, last, threshold);
//    int64_t* middle = first + threshold;
//    insertion_sort(first, middle);
//    unguarded_insertion_sort(middle, last);
//  }
//}
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

template<class I, class Comp = std::less<>> auto
swap_median_of_3_pivot_first(I a, I b, I c, Comp comp = {}) -> void {
  if (comp(*a,*b))
    if (comp(*b,*c)) return std::ranges::iter_swap(a,b);
    else if (comp(*a,*c)) return std::ranges::iter_swap(a,c);
    else return;
  else if (comp(*a,*c)) return;
  else if (comp(*b,*c)) return std::ranges::iter_swap(a,c);
  else return std::ranges::iter_swap(a,b);
}


template<class I, class Comp = std::less<>> auto
quicksort(I first, I last, Comp comp = {}) -> void {
  I next = std::next(first);

  if (first == last) return;
  if (next  == last) return;

  swap_median_of_3_pivot_first(first,first + (last-first)/2,last,comp);
  decltype(auto) pivot = *first;
  auto less_than_pivot = [&pivot,comp](const auto& value){ return comp(value,pivot); };

  I partition_pt = std::ranges::partition(next,last,less_than_pivot).begin();
  I one_before_partition_pt = std::prev(partition_pt);

  std::ranges::iter_swap(first,one_before_partition_pt);
  quicksort(first,one_before_partition_pt);
  quicksort(partition_pt,last);
}

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
  template<std::random_access_iterator It, std::sentinel_for<It> S, class Comp = std::ranges::less, class Proj = std::identity>
    requires std::sortable<It, Comp, Proj>
      constexpr auto
  operator()(It first, S last, Comp comp = {}, Proj proj = {}) const -> It {
    auto last_it = std::ranges::next(first, last);
    quicksort(first, last_it, make_comp_proj(comp, proj));
    return last_it;
  }

  template<std::ranges::random_access_range Range, class Comp = std::ranges::less, class Proj = std::identity>
    requires std::sortable<std::ranges::iterator_t<Range>, Comp, Proj>
      constexpr auto
  operator()(Range&& r, Comp comp = {}, Proj proj = {}) const -> std::ranges::borrowed_iterator_t<Range> {
    return (*this)(std::ranges::begin(r), std::ranges::end(r), std::move(comp), std::move(proj));
  }
};

inline constexpr _sort_fn sort{};

} // ranges

} // std_e
