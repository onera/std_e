#pragma once


namespace std_e {


template<class multi_index_type> constexpr size_t size = std::tuple_size_v<multi_index_type>;


} // std_e
