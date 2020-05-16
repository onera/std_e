#pragma once


namespace std_e {


constexpr auto
implies(bool x, bool y) -> bool {
  return !x || y;
}

constexpr auto
equiv(bool x, bool y) -> bool {
  return x==y;
}

constexpr auto
exclusive_or(bool x, bool y) -> bool {
  return x!=y; // or x^y
}


} // std_e
