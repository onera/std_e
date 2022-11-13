#pragma once


#include "std_e/execution/task/task_kind.hpp"
#include <type_traits>


namespace std_e {


template<class T>
concept is_task = std::remove_cvref_t<T>::enable_task; // TODO not really a concept


} // std_e
