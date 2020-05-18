#include "std_e/unit_test/doctest.hpp"
#include "std_e/data_structure/heterogenous_vector.hpp"

using namespace std;
using namespace std_e;

//! [Test]
//! \brief a unit test for M. Berthoul
void unit_test()
{
}
//! [Test]

TEST_CASE("hvector") {

//! [Test2]
  hvector<int,double> hv = { vector{1,2,3,4} , vector{3.14,2.7} };
//! [Test2]

  SUBCASE("hsize") {
    CHECK( hvector<int,double>::hsize() == 2 );
    CHECK( hv.hsize() == 2 );
  }
  SUBCASE("size") {
    CHECK( hv.size() == 4+2 );
  }

  SUBCASE("assessors") {
    CHECK( std_e::get<int>(hv) == vector{1,2,3,4} );
    CHECK( std_e::get<double>(hv) == vector{3.14,2.7} );
  }
  SUBCASE("assessors from index") {
    CHECK( std_e::get<0>(hv) == vector{1,2,3,4} );
    CHECK( std_e::get<1>(hv) == vector{3.14,2.7} );
  }

  SUBCASE("mutators") {
    std_e::get<int>(hv)[0] = 100;
    std_e::get<double>(hv).push_back(10000.);

    CHECK( hv.size() == 4+3 );

    CHECK( std_e::get<int>(hv) == vector{100,2,3,4} );
    CHECK( std_e::get<double>(hv) == vector{3.14,2.7,10000.} );
  }

  SUBCASE("for_each_vector") {
//! [Test3]
    int sz = 0;
    auto f = [&sz](auto v){ sz += v.size(); };
    for_each_vector(hv,f);
    CHECK( sz == 4+2 );
//! [Test3]
  }
  SUBCASE("for_each_element") {
    double sum = 0;
    auto f = [&sum](auto elt){ sum += elt; };
    for_each_element(hv,f);
    CHECK( sum == 1+2+3+4 + 3.14+2.7 );
  }

  SUBCASE("find_apply") {
    double value;
    auto f = [&value](auto x){ value = 10*x; };

    SUBCASE("case 1") {
      auto p = [](auto x){ return double(x)==3.14; };
      auto [pos_in_tuple,pos_in_vec] = find_apply(hv,p,f);
      CHECK( value == 3.14*10 );
      CHECK( pos_in_tuple == 1 );
      CHECK( pos_in_vec == 0 );
    }
    SUBCASE("case 2") {
      auto p = [](auto x){ return double(x)==3.; };
      auto [pos_in_tuple,pos_in_vec] = find_apply(hv,p,f);
      CHECK( value == 3*10 );
      CHECK( pos_in_tuple == 0 );
      CHECK( pos_in_vec == 2 );
    }
    SUBCASE("case not found") {
      value = 42.5;
      auto p = [](auto){ return false; };
      auto [pos_in_tuple,pos_in_vec] = find_apply(hv,p,f);
      CHECK( value == 42.5 );
      CHECK( pos_in_tuple == 2 );
      CHECK( pos_in_vec == 2 );
    }
  }

  SUBCASE("find_position") {
    auto p = [](auto x){ return double(x)==3.14; };
    auto [pos_in_tuple,pos_in_vec] = find_position(hv,p);
    CHECK( pos_in_tuple == 1 );
    CHECK( pos_in_vec == 0 );
  }

  SUBCASE("for_each_if") {
    std::vector<double> res;
    auto pred = [](auto x){ return x<3; };
    auto f = [&res](auto x){ res.push_back(2.*x); };
    for_each_if(hv,pred,f);
    std::vector<double> expected_res = {2.,4.,5.4};
    CHECK( res == expected_res );
  }
}
