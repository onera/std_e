#include "std_e/unit_test/doctest.hpp"

#include "std_e/multi_index/multi_index_range.hpp"
#include "std_e/log.hpp" // TODO


using namespace std_e;


// [Sphinx Doc] multi_index_range fortran order {
TEST_CASE("multi_index_range fortran order") {
  std::vector<int> v;
  for (auto [i,j,k] : fortran_multi_index_range({4,3,2})) {
    v.push_back( 100*i + 10*j + k );
  }
  CHECK( v.size() == 4*3*2 );

  std::vector<int> expected_v = {
//  ijk
//  |||
//  vvv
      0,
    100,
    200,
    300,
     10,
    110,
    210,
    310,
     20,
    120,
    220,
    320,
      1,
    101,
    201,
    301,
     11,
    111,
    211,
    311,
     21,
    121,
    221,
    321
  };
  CHECK( v == expected_v );
}
// [Sphinx Doc] multi_index_range fortran order }


TEST_CASE("multi_index_range fortran order -- no_dim_range") {
  std_e::multi_index<int> no_dim = {};
  auto no_dim_range = fortran_multi_index_range(no_dim);
  CHECK( no_dim_range.size() == 1 );
  CHECK( no_dim_range.begin() != no_dim_range.end() );
  CHECK( ++no_dim_range.begin() == no_dim_range.end() );
  CHECK( *no_dim_range.begin() == std_e::multi_index<int>{} );
}


// [Sphinx Doc] c_multi_index_range {
TEST_CASE("multi_index_range c order") {
  auto all_indices_range = c_multi_index_range({4,3,2});

  std::vector<int> v;
  for (auto [i,j,k] : all_indices_range) {
    v.push_back( 100*i + 10*j + k );
  }
  CHECK( v.size() == 4*3*2 );

  std::vector<int> expected_v = {
//  ijk
//  |||
//  vvv
      0,
      1,
     10,
     11,
     20,
     21,
    100,
    101,
    110,
    111,
    120,
    121,
    200,
    201,
    210,
    211,
    220,
    221,
    300,
    301,
    310,
    311,
    320,
    321,
  };
  CHECK( v == expected_v );
}
// [Sphinx Doc] c_multi_index_range }


// [Sphinx Doc] multi_index_range arbitrary order {
TEST_CASE("multi_index_range arbitrary order") {
  auto all_indices_range = multi_index_range_with_order({4,3,2},{1,2,0});

  std::vector<int> v;
  for (auto [i,j,k] : all_indices_range) {
    v.push_back( 100*i + 10*j + k );
  }
  CHECK( v.size() == 4*3*2 );

  std::vector<int> expected_v = {
//  ijk
//  |||
//  vvv
      0,
     10,
     20,
      1,
     11,
     21,
    100,
    110,
    120,
    101,
    111,
    121,
    200,
    210,
    220,
    201,
    211,
    221,
    300,
    310,
    320,
    301,
    311,
    321,
  };
  CHECK( v == expected_v );
}
// [Sphinx Doc] multi_index_range arbitrary order }
