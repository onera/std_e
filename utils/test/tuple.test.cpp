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


TEST_CASE("tuple for_each")
{
  SUBCASE("tuple<S1,S2,S3>"){
    tuple<S1, S2, S2> t = {S1{1}, S2{2}, S2{3}};

    vector<int> v;
    std_e::for_each(t, [&](auto& t){ v.push_back(t.i); });

    vector<int> v_expected = {1, 2, 3};
    CHECK( v == v_expected );
  }

  SUBCASE("tuple<vector<S>...> "){
    tuple<vector<S1>,vector<S2>,vector<S2>> t = {{S1{3},S1{2},S1{1}} , {S2{-9}}, {S2{30}}};

    vector<int> v;
    std_e::for_each(t, [&](auto& vt){
        std::for_each(begin(vt), end(vt), [&](auto& t){ v.push_back(t.i); });
    });

    vector<int> v_expected = {3, 2, 1, -9, 30};
    CHECK(v ==  v_expected);
  }
}
