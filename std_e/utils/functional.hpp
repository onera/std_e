#pragma once

#include "std_e/base/macros.hpp"
#include "std_e/meta/reference_traits.hpp"
// TODO move to meta/


namespace std_e {


constexpr const auto identity = [](auto&& x)->decltype(auto){ return FWD(x); };
using identity_closure = decltype(identity);

template<class F, class... Args>
using proj_return_type = reference_value_t<std::invoke_result_t<F,Args...>>;


} // std_e
