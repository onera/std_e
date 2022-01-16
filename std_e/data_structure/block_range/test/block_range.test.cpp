#include "std_e/unit_test/doctest.hpp"

#include "std_e/data_structure/block_range/block_range.hpp"

using namespace std_e;

TEST_CASE("block_range") {
  std::vector<int> c = {3,4,5,  6,10,12};

  block_range<std::vector<int>,3> c_range = make_block_range<3>(c);

  SUBCASE("basic methods") {
    CHECK( c_range.total_size() == 6 );
    CHECK( c_range.size() == 2 );

    CHECK( c_range.block_size() == 3 );
    static_assert( block_range<std::vector<int>,3>::block_size() == 3 ); // same thing, but as static method
  }

  SUBCASE("operator[]") {
    CHECK( c_range[0] == std::vector{3, 4, 5} );
    CHECK( c_range[1] == std::vector{6,10,12} );
  }
  SUBCASE("assignement") {
    c_range[0][1] = 99;
    c_range[1][2] = 0;
    CHECK( c_range[0] == std::vector{3,99, 5} );
    CHECK( c_range[1] == std::vector{6,10, 0} );
  }
};
