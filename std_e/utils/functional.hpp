#pragma once

#include "std_e/base/macros.hpp"
#include "std_e/meta/reference_traits.hpp"
#include <algorithm>


namespace std_e {


constexpr const auto identity = [](auto&& x)->decltype(auto){ return FWD(x); };
using identity_closure = decltype(identity);

constexpr auto always_true_predicate = [](auto /*x*/){ return true; };
constexpr auto always_false_predicate = [](auto /*x*/){ return false; };

template<class F, class... Args>
using value_return_t = reference_value_t<std::invoke_result_t<F,Args...>>;


constexpr auto std_sort_lambda        = [](auto f, auto l, auto&& comp){ std::sort       (f,l,comp); };
constexpr auto std_stable_sort_lambda = [](auto f, auto l, auto&& comp){ std::stable_sort(f,l,comp); };


} // std_e
