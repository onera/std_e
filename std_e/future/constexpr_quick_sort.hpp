#pragma once


// from https://tristanbrindle.com/posts/a-more-useful-compile-time-quicksort
#include <iterator>


namespace std_e {


template <typename RAIt>
constexpr RAIt next(RAIt it,
                    typename std::iterator_traits<RAIt>::difference_type n = 1)
{
    return it + n;
}

template <typename RAIt>
constexpr auto distance(RAIt first, RAIt last)
{
    return last - first;
}

template<class ForwardIt1, class ForwardIt2>
constexpr void iter_swap(ForwardIt1 a, ForwardIt2 b)
{
    auto temp = std::move(*a);
    *a = std::move(*b);
    *b = std::move(temp);
}

template<class InputIt, class UnaryPredicate>
constexpr InputIt find_if_not(InputIt first, InputIt last, UnaryPredicate q)
{
    for (; first != last; ++first) {
        if (!q(*first)) {
            return first;
        }
    }
    return last;
}

template<class ForwardIt, class UnaryPredicate>
constexpr ForwardIt partition(ForwardIt first, ForwardIt last, UnaryPredicate p)
{
    first = std_e::find_if_not(first, last, p);
    if (first == last) return first;

    for(ForwardIt i = std_e::next(first); i != last; ++i){
        if(p(*i)){
            std_e::iter_swap(i, first);
            ++first;
        }
    }
    return first;
}

template<class RAIt, class Compare = std::less<>> constexpr auto
quick_sort(RAIt first, RAIt last, Compare cmp = Compare{}) -> void {
    auto const N = std_e::distance(first, last);
    if (N <= 1) return;
    auto const pivot = *std_e::next(first, N / 2);
    auto const middle1 = std_e::partition(first, last, [=](auto const& elem){
        return cmp(elem, pivot);
    });
    auto const middle2 = std_e::partition(middle1, last, [=](auto const& elem){
        return !cmp(pivot, elem);
    });
    quick_sort(first, middle1, cmp); // assert(std::is_sorted(first, middle1, cmp));
    quick_sort(middle2, last, cmp);  // assert(std::is_sorted(middle2, last, cmp));
}


}
