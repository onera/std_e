#include "std_e/unit_test/doctest.hpp"

#include "std_e/data_structure/block_range/block_iterator.hpp"

using namespace std_e;
using std::vector;

TEST_CASE("block_iterator") {
  vector<int> c = {3,4,5,  6,10,12};

  SUBCASE("const") {
    block_iterator<const int,3> const_it(c.data());

    CHECK( *const_it == vector{3, 4, 5} );

    ++const_it;

    CHECK( *const_it == vector{6,10,12} );
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

  SUBCASE("assign") {
    vector<double> v0  = {0.,1.,2.};
    vector<double> v1(3);

    block_iterator<double,3> it0 = {v0.data()};
    block_iterator<double,3> it1 = {v1.data()};

    *it1 = *it0;

    CHECK( v1 == vector{0.,1.,2.} );
  }
}
