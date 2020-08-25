#include "std_e/logging/console_color.hpp"


namespace std_e {

auto
color_string(console_color color) -> std::string {
  std::string c = "\033";
  switch (color) {
    case console_color::red:          return c + "[0;31m";
    case console_color::green:        return c + "[0;32m";
    case console_color::blue:         return c + "[0;34m";
    case console_color::cyan:         return c + "[0;36m";
    case console_color::yellow:       return c + "[0;33m";
    case console_color::grey:         return c + "[1;30m";
    case console_color::light_grey:   return c + "[0;37m";
    case console_color::bright_red:   return c + "[1;31m";
    case console_color::bright_green: return c + "[1;32m";
    case console_color::bright_white: return c + "[1;37m";
    case console_color::none:
    case console_color::white:
    default:                          return c + "[0m";
  }
}

} // std_e
