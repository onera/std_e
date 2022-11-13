#pragma once

/// Future (C++20)
/// This file re-implements standard algorithms with "constexpr"
/// Many of the implementations come from cppreference.com

#include <utility>
#include <iterator>
#include <functional>


namespace std_e {


template<class InputIt, class S, class OutputIt> constexpr auto
copy(InputIt first, S last, OutputIt d_first) -> OutputIt
{
    while (first != last) {
        *d_first++ = *first++;
    }
    return d_first;
}

template< class InputIt, class Size, class OutputIt>
constexpr OutputIt
copy_n(InputIt first, Size count, OutputIt result)
{
    if (count > 0) {
        *result++ = *first;
        for (Size i = 1; i < count; ++i) {
            *result++ = *++first;
        }
    }
    return result;
}

template<class InputIt, class OutputIt, class UnaryPredicate>
constexpr OutputIt
copy_if(InputIt first, InputIt last, OutputIt d_first, UnaryPredicate pred)
{
    while (first != last) {
        if (pred(*first))
            *d_first++ = *first;
        first++;
    }
    return d_first;
}


template< class ForwardIt, class T >
constexpr void
fill(ForwardIt first, ForwardIt last, const T& value)
{
    for (; first != last; ++first) {
        *first = value;
    }
}


template<class InputIt1, class InputIt2>
constexpr std::pair<InputIt1, InputIt2>
    mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2)
{
    while (first1 != last1 && *first1 == *first2) {
        ++first1, ++first2;
    }
    return std::make_pair(first1, first2);
}


template<class InputIt1, class InputIt2, class BinaryPredicate>
constexpr std::pair<InputIt1, InputIt2>
    mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p)
{
    while (first1 != last1 && p(*first1, *first2)) {
        ++first1, ++first2;
    }
    return std::make_pair(first1, first2);
}


template<class InputIt1, class InputIt2>
constexpr std::pair<InputIt1, InputIt2>
    mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
{
    while (first1 != last1 && first2 != last2 && *first1 == *first2) {
        ++first1, ++first2;
    }
    return std::make_pair(first1, first2);
}


template<class InputIt1, class InputIt2, class BinaryPredicate>
constexpr std::pair<InputIt1, InputIt2>
    mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, BinaryPredicate p)
{
    while (first1 != last1 && first2 != last2 && p(*first1, *first2)) {
        ++first1, ++first2;
    }
    return std::make_pair(first1, first2);
}


template<class InputIt, class Sentinel, class OutputIt, class UnaryOperation>
constexpr OutputIt 
    transform(InputIt first1, Sentinel last1, OutputIt d_first, 
                   UnaryOperation unary_op)
{
    while (first1 != last1) {
        *d_first = unary_op(*first1);
        ++d_first;
        ++first1;
    }
    return d_first;
}

template<class InputIt1, class InputIt2, 
         class OutputIt, class BinaryOperation>
constexpr OutputIt
    transform(InputIt1 first1, InputIt1 last1, InputIt2 first2, 
                   OutputIt d_first, BinaryOperation binary_op)
{
    while (first1 != last1) {
        *d_first++ = binary_op(*first1++, *first2++);
    }
    return d_first;
}

/// lexicographical_compare {
template<class InputIt1, class InputIt2>
constexpr bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                             InputIt2 first2, InputIt2 last2)
{
    for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 ) {
        if (*first1 < *first2) return true;
        if (*first2 < *first1) return false;
    }
    return (first1 == last1) && (first2 != last2);
}

template<class InputIt1, class InputIt2, class Compare>
constexpr bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                             InputIt2 first2, InputIt2 last2,
                             Compare comp)
{
    for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2 ) {
        if (comp(*first1, *first2)) return true;
        if (comp(*first2, *first1)) return false;
    }
    return (first1 == last1) && (first2 != last2);
}
/// lexicographical_compare }


/// unique {
template<class ForwardIt>
constexpr ForwardIt unique(ForwardIt first, ForwardIt last)
{
    if (first == last)
        return last;
 
    ForwardIt result = first;
    while (++first != last) {
        if (!(*result == *first) && ++result != first) {
            *result = std::move(*first);
        }
    }
    return ++result;
}


template<class ForwardIt, class BinaryPredicate>
constexpr ForwardIt unique(ForwardIt first, ForwardIt last, BinaryPredicate p)
{
    if (first == last)
        return last;
 
    ForwardIt result = first;
    while (++first != last) {
        if (!p(*result, *first) && ++result != first) {
            *result = std::move(*first);
        }
    }
    return ++result;
}
/// unique }

template<class ForwardIt, class T> constexpr void
iota(ForwardIt first, ForwardIt last, T value) {
    while(first != last) {
        *first = value;
        ++first;
        ++value;
    }
}

template<class InputIt, class T> constexpr InputIt
find(InputIt first, InputIt last, const T& value) {
    for (; first != last; ++first) {
        if (*first == value) {
            return first;
        }
    }
    return last;
}

template<class InputIt, class S, class UnaryPredicate> constexpr InputIt
find_if(InputIt first, S last, UnaryPredicate p) {
    for (; first != last; ++first) {
        if (p(*first)) {
            return first;
        }
    }
    return first;
}


template<class InputIt1, class InputIt2> constexpr bool
equal(InputIt1 first1, InputIt1 last1, InputIt2 first2) {
    for (; first1 != last1; ++first1, ++first2) {
        if (!(*first1 == *first2)) {
            return false;
        }
    }
    return true;
}
template<class InputIt1, class InputIt2, class BinaryPredicate> constexpr bool
equal(InputIt1 first1, InputIt1 last1, InputIt2 first2, BinaryPredicate p) {
    for (; first1 != last1; ++first1, ++first2) {
        if (!p(*first1, *first2)) {
            return false;
        }
    }
    return true;
}


template<class InputIt, class T> constexpr auto
accumulate(InputIt first, InputIt last, T init) -> T {
    for (; first != last; ++first) {
        init = std::move(init) + *first; // std::move since C++20
    }
    return init;
}


template<class InputIt, class T, class BinaryOperation> constexpr auto
accumulate(InputIt first, InputIt last, T init, BinaryOperation op) -> T {
    for (; first != last; ++first) {
        init = op(std::move(init), *first); // std::move since C++20
    }
    return init;
}

template<class ForwardIt, class UnaryPredicate> constexpr auto
partition_point(ForwardIt first, ForwardIt last, UnaryPredicate pred) -> ForwardIt {
  auto n = std::distance(first, last);
  while (n>0) {
    ForwardIt it = first;
    auto step = n/2;
    std::advance (it, step);
    if (pred(*it)) { first=++it; n-=step+1; }
    else n=step;
  }
  return first;
}

template<class ForwardIt, class T> constexpr auto
lower_bound(ForwardIt first, ForwardIt last, const T& value) -> ForwardIt {
    ForwardIt it = first;
    typename std::iterator_traits<ForwardIt>::difference_type step=0;
    auto count = std::distance(first, last);
 
    while (count > 0) {
        it = first; 
        step = count / 2; 
        std::advance(it, step);
        if (*it < value) {
            first = ++it; 
            count -= step + 1; 
        }
        else
            count = step;
    }
    return first;
}


template<class ForwardIt, class T, class Compare> constexpr auto
lower_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp) -> ForwardIt {
    ForwardIt it = first;
    typename std::iterator_traits<ForwardIt>::difference_type step=0;
    auto count = std::distance(first, last);
 
    while (count > 0) {
        it = first;
        step = count / 2;
        std::advance(it, step);
        if (comp(*it, value)) {
            first = ++it;
            count -= step + 1;
        }
        else
            count = step;
    }
    return first;
}


template<class InputIt, class T> constexpr auto
count(InputIt first, InputIt last, const T& value)
 -> typename std::iterator_traits<InputIt>::difference_type
{
  typename std::iterator_traits<InputIt>::difference_type ret = 0;
  for (; first != last; ++first) {
    if (*first == value) {
      ret++;
    }
  }
  return ret;
}

template<class InputIt, class UnaryPredicate> constexpr auto
count_if(InputIt first, InputIt last, UnaryPredicate p)
 -> typename std::iterator_traits<InputIt>::difference_type
{
  typename std::iterator_traits<InputIt>::difference_type ret = 0;
  for (; first != last; ++first) {
    if (p(*first)) {
      ret++;
    }
  }
  return ret;
}


template<class InputIt, class OutputIt, class BinaryOperation> constexpr auto
inclusive_scan(InputIt first, InputIt last, OutputIt d_first, BinaryOperation op) -> OutputIt {
  if (first == last) return d_first;

  auto sum = *first;
  *d_first = sum;

  while (++first != last) {
   sum = op(std::move(sum), *first);
   *++d_first = sum;
  }
  return ++d_first;
}
template<class InputIt, class OutputIt> constexpr auto
inclusive_scan(InputIt first, InputIt last, OutputIt d_first) -> OutputIt {
  return std_e::inclusive_scan(first, last, d_first, std::plus<>());
}

template<class InputIt, class OutputIt, class T, class BinaryOperation> constexpr auto
exclusive_scan(InputIt first, InputIt last, OutputIt d_first, T init, BinaryOperation op) -> OutputIt {
  if (first == last) return d_first;

  --last;
  *d_first = init;

  while (first != last) {
   init = op(std::move(init), *first++);
   *++d_first = init;
  }
  return ++d_first;
}
template<class InputIt, class OutputIt, class T> constexpr auto
exclusive_scan(InputIt first, InputIt last, OutputIt d_first, T init) -> OutputIt {
  return std_e::exclusive_scan(first, last, d_first, init, std::plus<>());
}


// Not present in libc++ <= 14
template< class I1, class I2, class Cmp >
constexpr auto lexicographical_compare_three_way( I1 f1, I1 l1, I2 f2, I2 l2, Cmp comp )
    -> decltype(comp(*f1, *f2))
{
    using ret_t = decltype(comp(*f1, *f2));
    static_assert(std::disjunction_v<
                      std::is_same<ret_t, std::strong_ordering>,
                      std::is_same<ret_t, std::weak_ordering>,
                      std::is_same<ret_t, std::partial_ordering>>,
                  "The return type must be a comparison category type.");
 
    bool exhaust1 = (f1 == l1);
    bool exhaust2 = (f2 == l2);
    for (; !exhaust1 && !exhaust2; exhaust1 = (++f1 == l1), exhaust2 = (++f2 == l2))
        if (auto c = comp(*f1, *f2); c != 0)
            return c;
 
    return !exhaust1 ? std::strong_ordering::greater :
           !exhaust2 ? std::strong_ordering::less :
                       std::strong_ordering::equal;
}

} // std_e
