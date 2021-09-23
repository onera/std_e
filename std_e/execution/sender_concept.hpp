#pragma once


#include <type_traits>


namespace std_e {


template<class T> concept Sender = std::remove_cvref_t<T>::enable_sender;


} // std_e
