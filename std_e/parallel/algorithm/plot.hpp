#pragma once


#include "std_e/utils/to_string.hpp"
#include <fstream>

namespace std_e {


template<class Rng0, class Rng1> auto
to_matplotlib_script(const std::string& fig_name, const Rng0& x, const Rng1& y) -> std::string {
  auto x_str = range_to_lite_string(x);
  auto y_str = range_to_lite_string(y);
  std::string s =
    "import matplotlib as mpl\n"
    "import matplotlib.pyplot as plt\n"
    "import numpy as np\n"

    "fig, ax = plt.subplots()\n"
    "ax.plot(["+x_str+"], ["+y_str+"])\n"
    "fig.savefig('"+fig_name+".svg')\n";
  return s;
}


template<class Rng> auto
to_matplotlib_file(const Rng& y, const std::string& file_name) -> void {
  std::ofstream f(file_name+".py");

  using T = typename Rng::value_type;
  std::vector<T> x(y.size());
  std::iota(begin(x),end(x),T(0));

  auto s = to_matplotlib_script(file_name,x,y);
  f.write(s.c_str(), s.size());
}


} // std_e
