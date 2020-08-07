#pragma once


#include <string>
#include "std_e/utils/string.hpp"
#include "std_e/utils/vector.hpp"
#include "std_e/utils/to_string_fwd.hpp"
#include <iomanip>
#include <sstream>


namespace std_e {


inline auto
to_string(const std::string& s) -> const std::string& {
  return s;
}

template<class T> auto
to_string(T* ptr) -> std::string {
  std::stringstream ss;
  ss << ptr;
  return ss.str();
}


template<class Forward_iterator, class S> auto
to_string(Forward_iterator first, S last, const std::string& pre, const std::string& inter, const std::string& post) -> std::string {
  using std::to_string;
  using std_e::to_string;

  std::string s = pre;
  if (first!=last) {
    s += to_string(*first++);
  }
  while (first!=last) {
    s+= inter + to_string(*first++);
  }
  s += post;
  return s;
}
template<class R> auto
range_to_string(const R& x, const std::string& pre, const std::string& inter, const std::string& post) -> std::string {
  return to_string(begin(x),end(x),pre,inter,post);
}


template<class floating_point_type> auto
to_string_almost_exact(floating_point_type x) -> std::string {
  std::ostringstream ss;
  ss << std::setprecision(15) << std::defaultfloat << x;
  std::string x_as_str = ss.str();
  if (!contains(x_as_str,'.')) {
    x_as_str.push_back('.'); // "123" -> "123."
  }
  return x_as_str;
}


} // std_e
