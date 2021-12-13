#pragma once


#include <cmath>
#include "std_e/interval/interval.hpp"


namespace std_e {


// This function is meant to be used for tests only
// It is numerically inaccurate close to 0
inline auto
sinc(double x) -> double {
  if(x == 0.) return 1.;
  else return sin(x)/x;
}

inline auto
interval_portion(const interval<double>& in, int n, int i) -> interval<double> {
  double len = length(in);
  double sub_len = len/n;
  return {inf(in) + i*sub_len  ,  inf(in) + (i+1)*sub_len};
}

template<class F> auto
function_result_vector(F f, int n, const interval<double>& in) -> std::vector<double> {
  std::vector<double> y(n);
  double len = length(in);
  for (int i=0; i<n; ++i) {
    double x = inf(in) + len/n * i;
    y[i] = f(x);
  }
  return y;
}


} // std_e
