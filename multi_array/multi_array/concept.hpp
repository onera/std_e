#pragma once


#include "std_e/multi_index/concept.hpp"
#include "std_e/multi_array/shape/concept.hpp"


namespace std_e {


/*
  concept Multi_array:
    index_type
    multi_index_type
    shape_type

    value_type
    pointer
    const_pointer
    reference
    const_reference

    static constexpr size_t fixed_rank

    rank() -> size_t
    extent() -> Multi_index
    offset() -> Multi_index

    data() -> pointer

    shape() -> shape_type

    operator() -> reference
*/


} // std_e
