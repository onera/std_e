#pragma once


#include "std_e/algorithm/iota.hpp"
#include <algorithm>


namespace std_e {


template<class Fwd_it, class T> constexpr auto
uniform_distribution(Fwd_it first, Fwd_it last, T elt0, T nb_elts) {
  T nb_intervals = std::distance(first,last)-1;

  STD_E_ASSERT(nb_intervals>=0);

  if (nb_intervals==0) {
    STD_E_ASSERT(nb_elts==0);
    *first = elt0;
    return;
  }

  T quotient  = nb_elts/nb_intervals;
  T remainder = nb_elts%nb_intervals;
  auto last_upper = exclusive_iota_n(first     ,remainder+1,elt0           ,quotient+1);
                    inclusive_iota  (last_upper,last       ,*(last_upper-1),quotient  );
}

template<class Fwd_it, class T> constexpr auto
uniform_distribution(Fwd_it first, Fwd_it last, T nb_elts) {
  return uniform_distribution(first,last,T(0),nb_elts);
}


} // std_e
