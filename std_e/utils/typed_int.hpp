#pragma once


#include <string>


namespace std_e {


template<auto tag>
struct typed_int {
  int value;

  constexpr auto operator<=>(const typed_int& x) const = default;
};

template<auto tag> auto
to_string(typed_int<tag> x) -> std::string {
  return std::to_string(x.value);
}


} // std_e
