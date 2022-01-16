#pragma once

#include "std_e/base/macros.hpp"
#include <type_traits>


namespace std_e {


constexpr const auto identity = [](auto&& x)->decltype(auto){ return FWD(x); };
using identity_closure = decltype(identity);

template<class Proj, class T>
using proj_return_type = std::decay_t<decltype(std::declval<Proj>()(std::declval<T>()))>;


} // std_e
