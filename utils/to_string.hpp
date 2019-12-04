#pragma once


#include <string>


namespace std_e {


inline auto
to_string(const std::string& s) -> const std::string& {
  return s;
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
range_to_string(const R& x, const std::string& pre="{", const std::string& inter=";", const std::string& post="}") -> std::string {
  return to_string(begin(x),end(x),pre,inter,post);
}


} // std_e
