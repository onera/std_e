#pragma once

#include "std_e/base/template_alias.hpp"
#include "std_e/future/span.hpp"
#include "std_e/data_structure/heterogenous_range.hpp"

namespace std_e {

template<class... Ts> using hspan = hrange<dyn_span, Ts...>;

} // std_e
