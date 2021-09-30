#pragma once


#include <string>
#include <vector>
#include <deque>
#include <tuple>


namespace std_e {


template<class R> auto
range_to_string(const R& x, const std::string& pre="{", const std::string& inter=";", const std::string& post="}") -> std::string;

template<class R> auto
range_to_lite_string(const R& x, const std::string& inter=",") -> std::string;

template<class T, class A> auto
to_string(const std::vector<T,A>& x) -> std::string;
template<class T, class A> auto
to_string(const std::deque<T,A>& x) -> std::string;
template<class... Ts> auto
to_string(const std::tuple<Ts...>& x) -> std::string;


} // std_e
