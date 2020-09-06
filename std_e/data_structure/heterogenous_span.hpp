#pragma once

#include "std_e/future/span.hpp"
#include "std_e/data_structure/heterogenous_range.hpp"

namespace std_e {

template<class... Ts> using hspan = hrange<std_e::span,Ts...>;

} // std_e
