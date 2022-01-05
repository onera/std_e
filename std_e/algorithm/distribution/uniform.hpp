#pragma once


#include "std_e/algorithm/iota.hpp"


namespace std_e {


template<class Fwd_it, class T> constexpr auto
uniform_distribution(Fwd_it first, Fwd_it last, T elt0, T n_elt) {
  auto n_interval = std::distance(first,last)-1;

  STD_E_ASSERT(n_interval>=0);

  if (n_interval==0) {
    STD_E_ASSERT(n_elt==0);
    *first = elt0;
    return;
  }

  T quotient  = n_elt/n_interval;
  T remainder = n_elt%n_interval;
  auto last_upper = exclusive_iota_n(first     ,remainder+1,elt0           ,quotient+1);
                    inclusive_iota  (last_upper,last       ,*(last_upper-1),quotient  );
}


template<class Fwd_it> constexpr auto
uniform_distribution(Fwd_it first, Fwd_it last, double elt0, double n_elt) {
  auto n_interval = std::distance(first,last)-1;

  STD_E_ASSERT(n_interval>=0);

  double quotient  = n_elt/n_interval;
  for(int i = 0; i < n_interval+1; ++i) {
    *first++ = elt0 + i*quotient;
  }
}

template<class Fwd_it, class T> constexpr auto
uniform_distribution(Fwd_it first, Fwd_it last, T n_elt) {
  return uniform_distribution(first,last,T(0),n_elt);
}


} // std_e
