#include "std_e/unit_test/doctest.hpp"
#include "std_e/future/zip.hpp"

#include <vector>

TEST_CASE("zip") {
  std::vector<int>    v0 = {4  ,3  ,1  ,2  ,6};
  std::vector<double> v1 = {40.,30.,10.,20.};

  auto v = std_e::zip(v0,v1);

  SUBCASE("size") {
    CHECK( v.size_0() == 5 );
    CHECK( v.size_1() == 4 );
    CHECK( v.size() == 4 );
  }

  SUBCASE("operator[]") {
    CHECK( v[0] == std_e::zpair<int,double>(4,40.) );
    CHECK( v[1] == std_e::zpair<int,double>(3,30.) );
    CHECK( v[2] == std_e::zpair<int,double>(1,10.) );
    CHECK( v[3] == std_e::zpair<int,double>(2,20.) );
    CHECK( v[3] == std_e::zpair<int,double>(2,20.) );
  }

  SUBCASE("ref") {
    CHECK( &v[0].first  == &v0[0] );
    CHECK( &v[0].second == &v1[0] );

    v[0] = std_e::zpair<int,double>{0,0.};
    v[1] = std_e::zpair<int,double>{1,1.};
    v[2] = std_e::zpair<int,double>{2,2.};
    v[3] = std_e::zpair<int,double>{3,3.};
    CHECK( v[0] == std_e::zpair<int,double>(0,0.) );
    CHECK( v[1] == std_e::zpair<int,double>(1,1.) );
    CHECK( v[2] == std_e::zpair<int,double>(2,2.) );
    CHECK( v[3] == std_e::zpair<int,double>(3,3.) );
  }

  SUBCASE("iterator") {
    int i = 10;
    for (auto [x,y] : v) {
      x = i;
      y = i+5.;
      i *= 2; 
    }
    CHECK( v[0] == std_e::zpair<int,double>(10,15.) );
    CHECK( v[1] == std_e::zpair<int,double>(20,25.) );
    CHECK( v[2] == std_e::zpair<int,double>(40,45.) );
    CHECK( v[3] == std_e::zpair<int,double>(80,85.) );
  }
}
