#include "std_e/unit_test/doctest.hpp"
#include "std_e/data_structure/jagged_range.hpp"

using namespace std_e;
using namespace std;

TEST_CASE("jagged_vector") {
  SUBCASE("base test") {
    jagged_vector<int> v;
    v.push_level();
    v.push_back(10);
    v.push_level();
    v.push_back(20);
    v.push_back(30);
    v.push_level();
    v.push_back(40);
    v.push_level();
    v.push_back(50);
    v.push_back(60);
    v.push_back(70);

    CHECK( v.size() == 4 );
    CHECK( v.flat_view() == std::vector{10,20,30,40,50,60,70} );
    CHECK( v.indices() == knot_vector<int>{0,1,3,4,7} );
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
    jagged_vector<int> v = {{0},{10,20},{30},{40,50,60}};
    CHECK( v.flat_view() == std::vector{0,10,20,30,40,50,60} );
    CHECK( v.indices() == knot_vector<int>{0,1,3,4,7} );
  }
  SUBCASE("equality") {
    jagged_vector<int> v = {{0},{10,20}};
    jagged_vector<int> w = {{0},{10,20}};
    jagged_vector<int> x = {{0},{10,21}};
    jagged_vector<int> y = {{0,10},{20}};
    CHECK( v == w );
    CHECK( v != x );
    CHECK( v != y );
  }

  SUBCASE("rank 3") {
    jagged_vector<int,3> v;
    v.push_level(1);
    v.push_level(2);
    v.push_back(10);
    v.push_level(2);
    v.push_back(20);
    v.push_back(30);
    v.push_level(1);
    v.push_level(2);
    v.push_back(40);
    v.push_level(2);
    v.push_back(50);
    v.push_back(60);
    v.push_back(70);

    CHECK( v.flat_view() == std::vector{10,20,30,40,50,60,70} );
    CHECK( v.index_array()[0] == knot_vector<int>{0,3,7} );
    CHECK( v.index_array()[1] == knot_vector<int>{0,1,3,4,7} );

    CHECK( v == jagged_vector<int,3>{{{10},{20,30}},{{40},{50,60,70}}} );
     
    CHECK( v[0].flat_view() == std::vector{10,20,30} );
    CHECK( v[0].indices() == knot_vector<int>{0,1,3} );
    CHECK( v[0].offset() == 0 );
    jagged_vector<int,2> v0 = {{10},{20,30}};
    CHECK( v[0] == v0 );
    CHECK( v[0][0] == std::vector{10} );
    CHECK( v[0][1] == std::vector{20,30} );

    CHECK( v[1].flat_view() == std::vector{40,50,60,70} );
    CHECK( v[1].indices() == knot_vector<int>{3,4,7} );
    CHECK( v[1].offset() == 3 );
    jagged_vector<int,2> v1 = {{40},{50,60,70}};
    CHECK( v[1] == v1 );
    CHECK( v[1][0] == std::vector{40} );
    CHECK( v[1][1] == std::vector{50,60,70} );
  }


  //SUBCASE("iteration") {
  //}

  //SUBCASE("non-const iteration") {
  //}

  //SUBCASE("equality") {
  //}
}

TEST_CASE("jagged_multi_vector") {
  jagged_multi_vector<2,int,string> v;
  v.push_level();
  v.push_back(10,"10");
  v.push_level();
  v.push_back(20,"20");
  v.push_back(30,"30");
  v.push_level();
  v.push_back(40,"40");
  v.push_level();
  v.push_back(50,"50");
  v.push_back(60,"60");
  v.push_back(70,"70");

  CHECK( v.size() == 4 );
  CHECK( range<0>(v.flat_view()) == std::vector{10,20,30,40,50,60,70} );
  CHECK( range<1>(v.flat_view()) == std::vector{"10","20","30","40","50","60","70"} );
  CHECK( v.indices() == knot_vector<int>{0,1,3,4,7} );
  CHECK( v[0].size() == 1 );
  CHECK( std::get<0>(v[0][0]) == 10 );
  CHECK( std::get<1>(v[0][0]) == "10" );
}
