#include "std_e/unit_test/doctest.hpp"

#include "std_e/data_structure/block_range/block_iterator.hpp"

using namespace std_e;

TEST_CASE("block_iterator") {
  std::vector<int> c = {3,4,5,  6,10,12};

  SUBCASE("const") {
    block_iterator<const int,3> const_it(c.data());

    CHECK( *const_it == std::vector{3, 4, 5} );

    ++const_it;

    CHECK( *const_it == std::vector{6,10,12} );
  }

  SUBCASE("non-const") {
    block_iterator<int,3> it = {c.data()};

    span_ref<int,3> con0 = *it;

    con0[0] = 42;
    con0[1] = 43;
    con0[2] = 44;

    CHECK( c[0] == 42 );
    CHECK( c[1] == 43 );
    CHECK( c[2] == 44 );


    ++it;


    span_ref<int,3> con1 = *it;

    con1[0] = 100;
    con1[1] = 101;
    con1[2] = 102;

    CHECK( c[3] == 100 );
    CHECK( c[4] == 101 );
    CHECK( c[5] == 102 );
  }
}
