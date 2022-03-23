#pragma once


#include <string>
#include "std_e/utils/string.hpp"
#include "std_e/utils/vector.hpp"
#include "std_e/utils/deque.hpp"
#include "std_e/utils/tuple.hpp"
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


template<class Forward_iterator, class S, class F> auto
to_string(Forward_iterator first, S last, const std::string& inter, F to_string_fn) -> std::string {
  std::string s;
  if (first!=last) {
    s += to_string_fn(*first++);
  }
  while (first!=last) {
    s+= inter + to_string_fn(*first++);
  }
  return s;
}
template<class R, class F> auto
range_to_lite_string(const R& x, const std::string& inter, F to_string_fn) -> std::string {
  return to_string(begin(x),end(x),inter,to_string_fn);
}
template<class R, class F> auto
range_to_string(
  const R& x,
  const std::string& pre, const std::string& inter, const std::string& post,
  F to_string_fn
)
 -> std::string
{
  return pre+range_to_lite_string(x,inter,to_string_fn)+post;
}

template<class R, class F> auto
range_to_string(const R& x, F to_string_fn) -> std::string {
  return range_to_string(x,"{",",","}",to_string_fn);
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

template<typename... Ts, std::enable_if_t<(sizeof...(Ts)>1),int> =0> auto
to_string(const Ts&... xs) -> std::string {
  std::string s;
  using std::to_string;
  using std_e::to_string;
  ( (s += to_string(xs)) , ... );
  return s;
}


} // std_e
