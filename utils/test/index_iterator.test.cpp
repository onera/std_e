#include "doctest/doctest.hpp"

#include "std_e/utils/index_iterator.hpp"
#include <vector>

using std::vector;

// TODO
//TEST_CASE("index_iterator") {
//  std::vector<double> v = {3.14,42.,5.1,10.001,2.7}; 
//  std::vector<int> sub_v_indices = {0,2,4};
//  auto sub_range = std_e::make_indexed_range(v,sub_v_indices);
//
//  REQUIRE(sub_range.size() == 3);
//  CHECK(sub_range[0] == 3.14);
//  CHECK(sub_range[1] == 5.1);
//  CHECK(sub_range[2] == 2.7);
//
//  SUBCASE("begin, size") {
//    std::vector<double> sub_range_copy;
//    std::copy_n(begin(sub_range),sub_range.size(),std::back_inserter(sub_range_copy));
//    REQUIRE(sub_range_copy.size() == 3);
//
//    CHECK( std::equal(begin(sub_range_copy),end(sub_range_copy),begin(sub_range)) );
//  }
//}
