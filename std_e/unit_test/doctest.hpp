#pragma once


#include "doctest/doctest.h"
#include "std_e/utils/vector.hpp"
#include "std_e/utils/array.hpp"
#include "std_e/future/span.hpp"
#include "std_e/future/dynarray.hpp"


namespace doctest {


template<class T, class A>
struct StringMaker<std::vector<T,A>> {
  static String convert(const std::vector<T,A>& v) {
    using std::to_string;
    using std_e::to_string;
    std::string s = to_string(v);
    return s.c_str();
  }
};
template<class T>
struct StringMaker<std_e::dynarray<T>> {
  static String convert(const std_e::dynarray<T>& v) {
    using std::to_string;
    using std_e::to_string;
    std::string s = to_string(v);
    return s.c_str();
  }
};
template<class T, ptrdiff_t N>
struct StringMaker<std_e::span<T,N>> {
  static String convert(const std_e::span<T,N>& x) {
    using std::to_string;
    using std_e::to_string;
    std::string s = to_string(x);
    return s.c_str();
  }
};
template<class T, size_t N>
struct StringMaker<std::array<T,N>> {
  static String convert(const std::array<T,N>& v) {
    using std::to_string;
    using std_e::to_string;
    std::string s = to_string(v);
    return s.c_str();
  }
};


} // doctest
