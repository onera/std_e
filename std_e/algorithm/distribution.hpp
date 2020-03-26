#pragma once


#include "std_e/algorithm/iota.hpp"
#include <algorithm>


namespace std_e {


/*
concept Distribution : Range of Integers
  elts are sorted and begin at 0
*/


template<class Integer, class Distribution> auto 
search_rank(Integer global_id, const Distribution& dist) {
  auto it = std::lower_bound(begin(dist),end(dist),global_id);
  return it-begin(dist);
}


template<class Distribution, class Integer> constexpr auto
distribution_length(const Distribution& dist, Integer i) {
  return dist[i+1]-dist[i];
}


template<class Fwd_it, class T> constexpr auto
uniform_distribution(Fwd_it first, Fwd_it last, T elt0, T nb_elts) {
  T nb_slots = std::distance(first,last)-1;

  STD_E_ASSERT(nb_slots>=0);

  if (nb_slots==0) {
    STD_E_ASSERT(nb_elts==0);
    *first = elt0;
    return;
  }

  T quotient  = nb_elts/nb_slots;
  T remainder = nb_elts%nb_slots;
  auto last_upper = exclusive_iota_n(first     ,remainder+1,elt0           ,quotient+1);
                    inclusive_iota  (last_upper,last       ,*(last_upper-1),quotient  );
}


} // std_e
