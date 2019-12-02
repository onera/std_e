#include "doctest/doctest.h"
#include "doctest/doctest_utils.hpp"
#include "std_e/data_structure/heterogenous_vector.hpp"

using namespace std;
using namespace std_e;

TEST_CASE("exclusive_iota") {
  hvector<int,double> hv = { vector{1,2,3,4} , vector{3.14,2.7} };

  SUBCASE("assessors") {
    CHECK( hv.size() == 4+2 );

    CHECK( std_e::get<int>(hv) == vector{1,2,3,4} );
    CHECK( std_e::get<double>(hv) == vector{3.14,2.7} );
  }

  SUBCASE("mutators") {
    std_e::get<int>(hv)[0] = 100;
    std_e::get<double>(hv).push_back(10000.);

    CHECK( hv.size() == 4+3 );

    CHECK( std_e::get<int>(hv) == vector{100,2,3,4} );
    CHECK( std_e::get<double>(hv) == vector{3.14,2.7,10000.} );
  }

  SUBCASE("for_each_vector") {
    int sz = 0;
    auto f = [&sz](auto v){ sz += v.size(); };
    for_each_vector(hv,f);
    CHECK( sz == 4+2 );
  }
  SUBCASE("for_each_element") {
    double sum = 0;
    auto f = [&sum](auto elt){ sum += elt; };
    for_each_element(hv,f);
    CHECK( sum == 1+2+3+4 + 3.14+2.7 );
  }
}
