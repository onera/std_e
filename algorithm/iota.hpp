#pragma once


#include <iterator>
#include "std_e/future/contract.hpp"


namespace std_e {


template<class Fwd_it, class T> constexpr auto
exclusive_iota(Fwd_it first, Fwd_it last, T value, const T& step) {
  while (first != last) {
    *first = value;
    value += step;
    ++first;
  }
}
template<class Fwd_it, class Integer, class T> constexpr auto
exclusive_iota_n(Fwd_it first, Integer n, T value, const T& step) -> Fwd_it {
  while (n--) {
    *first = value;
    value += step;
    ++first;
  }
  return first;
}


template<class Fwd_it, class T> constexpr auto
inclusive_iota(Fwd_it first, Fwd_it last, T value, const T& step) {
  exclusive_iota(first,last,value+step,step);
}
template<class Fwd_it, class Integer, class T> constexpr auto
inclusive_iota_n(Fwd_it first, Integer n, T value, const T& step) -> Fwd_it {
  return exclusive_iota_n(first,n,value+step,step);
}


template<class Fwd_it, class T> constexpr auto
uniform_distribution(Fwd_it first, Fwd_it last, T elt0, T nb_elts) {
  int nb_slots = std::distance(first,last)-1;

  STD_E_ASSERT(nb_slots>=0);
  if (nb_slots==0) STD_E_ASSERT(nb_elts==0);

  int quotient  = nb_elts/nb_slots;
  int remainder = nb_elts%nb_slots;

  auto last_upper = exclusive_iota_n(first     ,remainder+1,elt0           ,quotient+1);
                    inclusive_iota  (last_upper,last       ,*(last_upper-1),quotient  );
}


} // std_e
