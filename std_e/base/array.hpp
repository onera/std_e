#pragma once


#include <array>


namespace std_e {


/**
 *  \brief  std_e::array is just a std::array with an **int** parameter size (instead of a **size**)
 *          
 */
template<class T, int N>
class array : public std::array<T,N>
{};


}
