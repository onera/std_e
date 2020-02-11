#pragma once


#include "std_e/multi_index/concept.hpp"
#include <algorithm>


namespace std_e {


template<class Multi_index> constexpr auto
is_one_dimensional(const Multi_index& x) -> bool {
  int nb_of_extents_greater_than_1 = std::count_if(begin(x),end(x),[](auto dim){return dim>1;});
  return nb_of_extents_greater_than_1<=1;
}


} // std_e
