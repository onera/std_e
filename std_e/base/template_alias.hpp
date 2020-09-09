#pragma once

#include "std_e/future/span.hpp"

namespace std_e {

template<class T> using std_alloc_vector = std::vector<T>; // This one is for clang error : template template argument has different template parameters than its corresponding template template parameter
template<class T> using dyn_span = std_e::span<T>;

}
