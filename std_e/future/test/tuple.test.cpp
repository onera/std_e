#include "std_e/unit_test/doctest.hpp"
#include "std_e/future/tuple.hpp"


TEST_CASE("tuple") {
  SUBCASE("ctor") {
    std_e::tuple<int,int*,double> t = {10,nullptr,3.14};
    CHECK( std_e::get<0>(t) == 10 );
    CHECK( std_e::get<1>(t) == nullptr );
    CHECK( std_e::get<2>(t) == 3.14 );
  }

  SUBCASE("default ctor + assignement") {
    std_e::tuple<int,int*,double> t;
    t = std_e::tuple<int,int*,double>{10,nullptr,3.14};
    CHECK( std_e::get<0>(t) == 10 );
    CHECK( std_e::get<1>(t) == nullptr );
    CHECK( std_e::get<2>(t) == 3.14 );
  }

  SUBCASE("comparisons") {
    std_e::tuple<int,int*,double> t0 = {10,nullptr,3.14};
    std_e::tuple<int,int*,double> t1 = {10,nullptr,3.14};
    std_e::tuple<int,int*,double> t2 = { 9,nullptr,3.14};
    std_e::tuple<int,int*,double> t3 = {11,nullptr,3.14};
    std_e::tuple<int,int*,double> t4 = {10,nullptr,100.};

    CHECK( t0 == t1 );
    CHECK( t0 != t2 );
    CHECK( t0 != t3 );
    CHECK( t0 != t4 );

    CHECK( t0 <= t1 );
    CHECK( t1 >= t1 );
    CHECK( t0 >  t2 );
    CHECK( t0 <  t3 );
    CHECK( t0 <  t4 );

    // compare in lexicographic order
    std_e::tuple<int,int*,double> t5 = { 9,nullptr,100.};
    CHECK ( t5 < t0 ); // compare 9<10 before 1000>3.14
  }

  SUBCASE("std_e::get gives a reference") {
    std_e::tuple<int,int*,double> t = {10,nullptr,3.14};
    std_e::get<0>(t) = 99;
    CHECK( std_e::get<0>(t) == 99 );
  }
  SUBCASE("const std_e::get") {
    const std_e::tuple<int,int*,double> t = {10,nullptr,3.14};
    CHECK( std_e::get<0>(t) == 10 );
    CHECK( std_e::get<1>(t) == nullptr );
    CHECK( std_e::get<2>(t) == 3.14 );
  }

  SUBCASE("tuple protocol") {
    static_assert(std::tuple_size_v<std_e::tuple<int,int*,double>> == 3);
  }
}

//TEST_CASE("tuple of references works as proxy reference (unlike C++20 std::tuple)") {
//  int i;
//  std_e::tuple<int&> t(i);
//  std_e::tuple<int&> r(i);
//
//  const_cast<const std_e::tuple<int&>&&>(r) = std::forward<std_e::tuple<int&>>(t);
//}
