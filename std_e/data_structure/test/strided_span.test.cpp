#include "doctest/doctest.h"
#include "std_e/data_structure/strided_span.hpp"

#include <vector>

TEST_CASE("strided_span") {
  std::vector<int> v = {0,1,2,3,4,5,6,7,8,9,10,11};
  
  SUBCASE("base test") {
    auto s = std_e::make_strided_span(v.data(),6,2);
    CHECK( s.data() == v.data() );
    CHECK( s.size() == 6 );
    CHECK( s.stride_length() == 2 );
    CHECK( s[0] ==  0 );
    CHECK( s[1] ==  2 );
    CHECK( s[2] ==  4 );
    CHECK( s[3] ==  6 );
    CHECK( s[4] ==  8 );
    CHECK( s[5] == 10 );
  }

  SUBCASE("size can be smaller") {
    auto s = std_e::make_strided_span(v.data()+1,2,4);
    CHECK( s.data() == v.data()+1 );
    CHECK( s.size() == 2 );
    CHECK( s.stride_length() == 4 );
    CHECK( s[0] ==  1 );
    CHECK( s[1] ==  5 );
  }

  SUBCASE("from container") {
    auto s = std_e::make_strided_span(v,4);
    CHECK( s.data() == v.data() );
    CHECK( s.size() == 3 );
    CHECK( s.stride_length() == 4 );
    CHECK( s[0] ==  0 );
    CHECK( s[1] ==  4 );
    CHECK( s[2] ==  8 );
  }

  SUBCASE("compile time stride") {
    auto s = std_e::make_strided_span<4>(v.data(),3);
    CHECK( s.data() == v.data() );
    CHECK( s.size() == 3 );
    CHECK( decltype(s)::stride_length() == 4 );
    CHECK( s[0] ==  0 );
    CHECK( s[1] ==  4 );
    CHECK( s[2] ==  8 );
  }
  SUBCASE("from container, ct stride length") {
    auto s = std_e::make_strided_span<4>(v);
    CHECK( s.data() == v.data() );
    CHECK( s.size() == 3 );
    CHECK( decltype(s)::stride_length() == 4 );
    CHECK( s[0] ==  0 );
    CHECK( s[1] ==  4 );
    CHECK( s[2] ==  8 );
  }

  SUBCASE("iteration") {
    auto s = std_e::make_strided_span<4>(v.data(),3);
    std::vector<int> res;
    for (int i : s) {
      res.push_back(i);
    }
    std::vector<int> expected_res = {0,4,8};
    CHECK( res == expected_res );
  }

  SUBCASE("non-const iteration") {
    auto s = std_e::make_strided_span<4>(v.data(),3);
    for (int& i : s) {
      i += 1000;
    }
    std::vector<int> expected_v = {1000,1,2,3, 1004,5,6,7, 1008,9,10,11};
    CHECK( v == expected_v );
  }
}
