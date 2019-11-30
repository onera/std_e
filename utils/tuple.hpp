#pragma once


#include <tuple>


namespace std_e {


// https://stackoverflow.com/questions/1198260/how-can-you-iterate-over-the-elements-of-an-stdtuple
// shut down GCC 9 erroneous warning because of "if constexpr"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
template<int I, class tuple_type, class F> constexpr auto
for_each__impl_tuple(tuple_type&& x, F f) -> void {
  constexpr int sz = std::tuple_size_v<std::decay_t<tuple_type>>;
  if constexpr (I<sz) {
    f(std::get<I>(x));
    for_each__impl_tuple<I+1>(x,f);
  }
}
#pragma GCC diagnostic pop

template<class... Ts, class F> constexpr auto
for_each(std::tuple<Ts...>& x, F f) -> void {
  for_each__impl_tuple<0>(x,f);
}
template<class... Ts, class F> constexpr auto
for_each(const std::tuple<Ts...>& x, F f) -> void {
  for_each__impl_tuple<0>(x,f);
}




template<class... Ts, class T, class bin_op> constexpr auto
accumulate(const std::tuple<Ts...>& x, T& init, bin_op op) -> void {
  auto acc = [&init,op](const auto& val){ init = op(init,val); };
  for_each(x,acc);
}




template<int I, class tuple_type, class F>  constexpr auto
for_each_until__impl_tuple(tuple_type&& x, F f) -> decltype(auto) {
  constexpr int sz = std::tuple_size_v<std::decay_t<tuple_type>>;
  if constexpr (I<sz) {
    auto&& res = f(std::get<I>(x));
    if (res) return res;
    else for_each_until__impl_tuple<I+1>(x,f);
  }
}

template<class... Ts, class F> constexpr auto
for_each_until(std::tuple<Ts...>& x, F f) -> decltype(auto) {
  for_each_until__impl_tuple<0>(x,f);
}
template<class... Ts, class F> constexpr auto
for_each_until(const std::tuple<Ts...>& x, F f) -> decltype(auto) {
  for_each_until__impl_tuple<0>(x,f);
}


} // std_e
