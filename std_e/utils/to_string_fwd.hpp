#pragma once


#include <string>


namespace std_e {


template<class R> auto
range_to_string(const R& x, const std::string& pre="{", const std::string& inter=";", const std::string& post="}") -> std::string;

template<class R> auto
range_to_lite_string(const R& x, const std::string& inter=",") -> std::string;


} // std_e
