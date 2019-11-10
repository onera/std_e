#pragma once


#include <string>


namespace std_e {


template<class Forward_iterator, class S> auto
to_string(Forward_iterator first, S last) -> std::string {
  using std::to_string;

  std::string s = "{";
  if (first!=last) {
    s += to_string(*first++);
  }
  while (first!=last) {
    s+= ";" + to_string(*first++);
  }
  s += "}";
  return s;
}
template<class R> auto
range_to_string(const R& x) -> std::string {
  return to_string(begin(x),end(x));
}


} // std_e
