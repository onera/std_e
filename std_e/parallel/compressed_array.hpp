#pragma once


#include <vector>
#include "std_e/interval/knot_sequence.hpp"


namespace std_e {


template<class T>
struct compressed_array {
  std::vector<T> data;
  std::vector<int> lengths;
  knot_vector<int> offsets;
};


} // std_e
