#include "std_e/unit_test/doctest.hpp"
#include "std_e/meta/pack.hpp"


using namespace std_e;


TEST_CASE("is_one_of") {
  static_assert(  is_one_of<int, double,int,char> );
  static_assert( !is_one_of<int, double,char> );
  static_assert(  is_one_of<int, double,int,char,int> );

  // type modifiers -> not the same type
  static_assert( !is_one_of<int, int&> );
  static_assert( !is_one_of<int, const int> );

  static_assert( !is_one_of<int> ); // empty case
}

TEST_CASE("are_all_of") {
  static_assert( !are_all_of<int, int,double,char> );
  static_assert(  are_all_of<int, int> );
  static_assert(  are_all_of<int, int,int> );

  // type modifiers -> not the same type
  static_assert( !are_all_of<int, int&> );
  static_assert( !are_all_of<int, const int> );

  static_assert(  are_all_of<int> ); // empty case
}
