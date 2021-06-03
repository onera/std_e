#pragma once

#include "std_e/base/template_alias.hpp"
#include "std_e/utils/vector.hpp"
#include "std_e/data_structure/heterogenous_range.hpp"

namespace std_e {

/// \brief hvector<Ts...> is a wrapper around tuple<vector<Ts...>> with convenient accessors and algorithms
template<class... Ts> using hvector = hrange<std_alloc_vector,Ts...>;

template<class... Ts, class F> [[deprecated("use for_each_range instead")]] constexpr auto
for_each_vector(hvector<Ts...>& hr, F f) -> void {
  for_each_range(hr,f);
}
template<class... Ts, class F> [[deprecated("use for_each_range instead")]]  constexpr auto
for_each_vector(const hvector<Ts...>& hr, F f) -> void {
  for_each_range(hr,f);
}

// TODO Test
//template<class... UTypes>
//hrange(std::vector<UTypes>...) -> hrange<UTypes...>;
// template<class T0>
// hrange(std::vector<T0>) -> hrange<std::vector,T0>;
// template<class T0, class T1>
// hrange(std::vector<T0>,std::vector<T1>) -> hrange<std::vector,T0,T1>;
// template<class T0, class T1, class T2>
// hrange(std::vector<T0>,std::vector<T1>,std::vector<T2>) -> hrange<std::vector,T0,T1,T2>;
// template<class T0, class T1, class T2, class T3>
// hrange(std::vector<T0>,std::vector<T1>,std::vector<T2>,std::vector<T3>) -> hrange<std::vector,T0,T1,T2,T3>;

} // std_e
