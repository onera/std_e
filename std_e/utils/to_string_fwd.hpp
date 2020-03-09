#pragma once


#include <string>


namespace std_e {


template<class R> auto
range_to_string(const R& x, const std::string& pre="{", const std::string& inter=";", const std::string& post="}") -> std::string;


} // std_e
