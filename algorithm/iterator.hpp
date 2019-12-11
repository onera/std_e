#pragma once


#include <algorithm>


namespace std_e {


template<class Input_it, class Output_it, class Random_it0, class Random_it1> constexpr auto
// requires Input_it::value_type == Random_it0
// requires Output_it::value_type == Random_it1
transfer_iterator_shifts(Input_it first, Input_it last, Output_it d_first, Random_it0 start, Random_it1 d_start) -> void {
  std::transform(first,last,d_first,[=](const auto& it){
    int index = it - start;
    return d_start + index;
  });
}


} // std_e
