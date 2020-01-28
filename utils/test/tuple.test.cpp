#include "doctest/doctest.h"
#include "std_e/utils/tuple.hpp"

#include <vector>
#include <algorithm>
using std::vector;
using std::tuple;

struct S1{
  int i;
};
struct S2{
  int i;
};
struct S3{
  int i;
};


TEST_CASE("tuple for_each") {
  SUBCASE("tuple<S1,S2,S3>") {
    tuple<S1, S2, S2> t = {S1{1}, S2{2}, S2{3}};

    vector<int> v;
    std_e::for_each(t, [&](auto& t){ v.push_back(t.i); });

    vector<int> v_expected = {1, 2, 3};
    CHECK( v == v_expected );
  }

  SUBCASE("tuple<vector<S>...> ") {
    tuple<vector<S1>,vector<S2>,vector<S2>> t = {{S1{3},S1{2},S1{1}} , {S2{-9}}, {S2{30}}};

    vector<int> v;
    std_e::for_each(t, [&](auto& vt){
        std::for_each(begin(vt), end(vt), [&](auto& t){ v.push_back(t.i); });
    });

    vector<int> v_expected = {3, 2, 1, -9, 30};
    CHECK(v ==  v_expected);
  }
}


TEST_CASE("tuple find_apply") {
  tuple<S1, S2, S2> t = {S1{1}, S2{2}, S2{3}};

  auto pred = [](auto x){ return x.i==2; };

  int cnt;
  auto f = [&cnt](auto x){ cnt = 10*x.i; };

  auto idx_found = std_e::find_apply(t,pred,f);

  CHECK( cnt == 10*2 );
  CHECK( idx_found == 1 );
}

TEST_CASE("tuple for_each_until") {
  tuple<S1, S2, S2> t = {S1{1}, S2{2}, S2{3}};

  SUBCASE("non modifying") {
    int cnt;
    auto f = [&cnt](const auto& x){
      if (x.i==2) {
        cnt = 10*x.i;
        return true;
      } else {
        return false;
      }
    };

    auto idx_found = std_e::for_each_until(t,f);

    CHECK( idx_found == 1 );
    CHECK( cnt == 10*2 );
  }

  SUBCASE("modifying") {
    int cnt;
    auto f = [&cnt](auto& x){
      if (x.i==2) {
        x.i = 10*x.i;
        return true;
      } else {
        return false;
      }
    };

    auto idx_found = std_e::for_each_until(t,f);

    CHECK( idx_found == 1 );
    CHECK( std::get<0>(t).i ==    1 );
    CHECK( std::get<1>(t).i == 10*2 );
    CHECK( std::get<2>(t).i ==    3 );
  }
}
