#pragma once


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

} // std_e
