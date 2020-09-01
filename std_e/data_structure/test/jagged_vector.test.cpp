#include "std_e/unit_test/doctest.hpp"
#include "std_e/data_structure/jagged_vector.hpp"
#include "std_e/log.hpp" // TODO


TEST_CASE("jagged_vector") {
  SUBCASE("base test") {
    std_e::jagged_vector<int> v;
    v.push_back();
    v.push_back(10);
    v.push_back();
    v.push_back(20);
    v.push_back(30);
    v.push_back();
    v.push_back(40);
    v.push_back();
    v.push_back(50);
    v.push_back(60);
    v.push_back(70);

    CHECK( v.size() == 4 );
    CHECK( v.flat_view() == std::vector{10,20,30,40,50,60,70} );
    CHECK( v.offsets() == std_e::knot_vector<int>{0,1,3,4,7} );
    CHECK( v[0].size() == 1 );
    CHECK( v[0][0] == 10 );

    CHECK( v[1].size() == 2 );
    CHECK( v[1][0] == 20 );
    CHECK( v[1][1] == 30 );

    CHECK( v[2].size() == 1 );
    CHECK( v[2][0] == 40 );
    CHECK( v[3].size() == 3 );
    CHECK( v[3][0] == 50 );
    CHECK( v[3][1] == 60 );
    CHECK( v[3][2] == 70 );
  }

  SUBCASE("init list") {
    std_e::jagged_vector<int> v = {{0},{10,20},{30},{40,50,60}};
    CHECK( v.flat_view() == std::vector{0,10,20,30,40,50,60} );
    CHECK( v.offsets() == std_e::knot_vector<int>{0,1,3,4,7} );
  }
  SUBCASE("equality") {
    std_e::jagged_vector<int> v = {{0},{10,20}};
    std_e::jagged_vector<int> w = {{0},{10,20}};
    std_e::jagged_vector<int> x = {{0},{10,21}};
    std_e::jagged_vector<int> y = {{0,10},{20}};
    CHECK( v == w );
    CHECK( v != x );
    CHECK( v != y );
  }
  //SUBCASE("from container") {
  //}

  //SUBCASE("iteration") {
  //}

  //SUBCASE("non-const iteration") {
  //}

  //SUBCASE("equality") {
  //}
}
