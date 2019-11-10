#pragma once


namespace std_e {


template<class InputIt1, class InputIt2, class Integer> constexpr bool
equal_n(InputIt1 first1, InputIt2 first2, Integer n) {
    for (Integer i=0; i<n; ++first1, ++first2, ++i) {
        if (!(*first1 == *first2)) {
            return false;
        }
    }
    return true;
}
template<class InputIt1, class InputIt2, class Integer, class BinaryPredicate> constexpr bool
equal_n(InputIt1 first1, InputIt2 first2, Integer n, BinaryPredicate p) {
    for (Integer i=0; i<n; ++first1, ++first2, ++i) {
        if (!p(*first1, *first2)) {
            return false;
        }
    }
    return true;
}


} // std_e
