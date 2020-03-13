#pragma once


#include <vector>
#include "doctest/doctest.h"
#include "std_e/utils/array.hpp"
#include "std_e/utils/vector.hpp"
#include "std_e/utils/to_string.hpp"


namespace doctest {


template<class T, class A> struct StringMaker<std::vector<T,A>> {
  static String convert(const std::vector<T,A>& v) {
    std::string s = std_e::to_string(v);
    return s.c_str();
  }
};
template<class T, size_t N> struct StringMaker<std::array<T,N>> {
  static String convert(const std::array<T,N>& v) {
    std::string s = std_e::to_string(v);
    return s.c_str();
  }
};


} // doctest
