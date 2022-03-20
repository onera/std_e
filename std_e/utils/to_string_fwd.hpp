#pragma once


#include <string>
#include <vector>
#include <deque>
#include <tuple>


namespace std_e {


inline auto
to_string(const std::string& s) -> const std::string&;
template<class T> auto
to_string(T* ptr) -> std::string;

template<class T, size_t N> auto
to_string(const std::array<T,N>& x) -> std::string;
template<class T, class A> auto
to_string(const std::vector<T,A>& x) -> std::string;
template<class T, class A> auto
to_string(const std::deque<T,A>& x) -> std::string;
template<class... Ts> auto
to_string(const std::tuple<Ts...>& x) -> std::string;


constexpr auto default_to_string_fn =
  [](const auto& x){
    using std::to_string;
    using std_e::to_string;
    return to_string(x);
  };
using default_to_string_closure = decltype(default_to_string_fn);

template<class R, class F = default_to_string_closure> auto
range_to_string(const R& x, const std::string& pre="{", const std::string& inter=",", const std::string& post="}", F f = default_to_string_fn) -> std::string;

template<class R, class F> auto
range_to_string(const R& x, F f) -> std::string;

template<class R, class F = default_to_string_closure> auto
range_to_lite_string(const R& x, const std::string& inter=",", F to_string_fn = default_to_string_fn) -> std::string;

template<typename... Ts, std::enable_if_t<(sizeof...(Ts)>1),int> =0> auto
to_string(const Ts&... xs) -> std::string;

} // std_e
