#pragma once


#include <string>
#include "std_e/utils/to_string.hpp"


namespace std_e {

// from https://github.com/onqtam/doctest
enum class console_color {
  none = 0,
  white,
  red,
  green,
  blue,
  cyan,
  yellow,
  grey,
  bright_red,
  bright_green,
  light_grey,
  bright_white
};

auto color_string(console_color color) -> std::string;

template<class... Ts> auto 
to_color_string(console_color color, const Ts&... xs) {
  using std::to_string;
  using std_e::to_string;
  return color_string(color) + (to_string(xs) + ...) + color_string(console_color::none);
}

} // std_e
