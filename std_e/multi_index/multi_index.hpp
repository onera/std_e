#pragma once


#include "std_e/multi_index/concept.hpp"
#include "std_e/utils/array.hpp"
#include "std_e/base/dynamic_size.hpp"
#include "std_e/future/algorithm.hpp"
#include "std_e/utils/array_vector_common.hpp"


namespace std_e {


// multi_index {
template<class Integer, int rank>
struct multi_index__impl
{
  using type = std::array<Integer,rank>;
};

template<class Integer>
struct multi_index__impl<Integer,dynamic_size>
{
  using type = std::vector<Integer>;
};

template<class Integer, int rank = dynamic_size>
using multi_index = typename multi_index__impl<Integer,rank>::type;
// multi_index }


// index_type_of {
template<class Multi_index>
using index_type_of = typename Multi_index::value_type;
// index_type_of }




// zero multi_index {
template<class Integer, size_t N> constexpr multi_index<Integer,N> zero_multi_index = default_array<Integer,N>;

template<class Multi_index> constexpr auto
make_zero_multi_index(size_t rank) {
  auto zero = make_array_of_size<Multi_index>(rank);
  std_e::fill(begin(zero),end(zero),0);
  return zero;
}
// zero multi_index }


} // std_e
