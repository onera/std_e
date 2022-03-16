#include "std_e/unit_test/doctest.hpp"

#include "std_e/data_structure/block_range/ivblock_iterator.hpp"

using namespace std_e;
using std::vector;

TEST_CASE("ivblock_iterator") {
  vector<int> c = {4, 2,3,4,5,   3, 6,10,12};

  SUBCASE("const") {
    ivblock_iterator<const int> const_it = {c.data()};

    CHECK( *const_it == vector{2,3,4,5} );

    ++const_it;

    CHECK( *const_it == vector{6,10,12} );
  }

  SUBCASE("non-const") {
    ivblock_iterator<int> it = {c.data()};

    ivblock_ref<int> con0 = *it;
    con0[0] = 42;
    con0[1] = 43;
    con0[2] = 44;
    con0[3] = 45;

    ++it;

    ivblock_ref<int> con1 = *it;
    con1[0] = 100;
    con1[1] = 101;
    con1[2] = 102;

    CHECK( c[0] == 4 );
    CHECK( c[1] == 42 );
    CHECK( c[2] == 43 );
    CHECK( c[3] == 44 );
    CHECK( c[4] == 45 );
    CHECK( c[5] == 3 );
    CHECK( c[6] == 100 );
    CHECK( c[7] == 101 );
    CHECK( c[8] == 102 );
  }

  SUBCASE("assign") {
    vector<int> v0  = {3, 0,1,2};
    vector<int> v1(4);

    ivblock_iterator<int> it0 = {v0.data()};
    ivblock_iterator<int> it1 = {v1.data()};

    *it1 = *it0;

    CHECK( v1 == vector{3, 0,1,2} );
  }
}
