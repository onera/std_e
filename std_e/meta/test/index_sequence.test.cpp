#include "std_e/unit_test/doctest.hpp"

#include "std_e/meta/index_sequence.hpp"
#include <type_traits>


TEST_CASE("integer_sequence get_value_at") {
  using int_seq = std::integer_sequence<int,4,3,5>;
  static_assert( std_e::get_value_at(0,int_seq{}) == 4 );
  static_assert( std_e::get_value_at(1,int_seq{}) == 3 );
  static_assert( std_e::get_value_at(2,int_seq{}) == 5 );
}

TEST_CASE("integer_sequence take_n") {
  using int_seq = std::index_sequence<4,3,5,6,8>;
  using sub_int_seq = decltype(std_e::take_n<3>(int_seq{}));
  using expected_sub_int_seq = std::index_sequence<4,3,5>;
  static_assert(std::is_same_v<sub_int_seq,expected_sub_int_seq>);
}

TEST_CASE("integer_sequence position_in_seq") {
  using int_seq = std::index_sequence<4,3,5,6,8>;
  static_assert( std_e::position_in_seq(4,int_seq{}) == 0 );
  static_assert( std_e::position_in_seq(3,int_seq{}) == 1 );
  static_assert( std_e::position_in_seq(8,int_seq{}) == 4 );
  static_assert( std_e::position_in_seq(2,int_seq{}) == 5 );
}
