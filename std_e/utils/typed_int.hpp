#pragma once


#include <string>


namespace std_e {


template<auto tag>
struct typed_int {
  int value;

  friend constexpr auto
  operator==(const typed_int& x, const typed_int& y) { return x.value==y.value; }

  friend constexpr auto
  operator!=(const typed_int& x, const typed_int& y) { return !(x==y); }

  friend constexpr auto
  operator<(const typed_int& x, const typed_int& y) { return x.value<y.value; }
};

template<auto tag> auto
to_string(typed_int<tag> x) -> std::string {
  return std::to_string(x.value);
}


} // std_e
