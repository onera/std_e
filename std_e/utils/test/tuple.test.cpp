#include "std_e/unit_test/doctest.hpp"
#include "std_e/utils/tuple.hpp"

#include <vector>
#include <algorithm>
using std::vector;
using std::tuple;

namespace {

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
    tuple<S1, S2, S3> t = {S1{1}, S2{2}, S3{3}};

    vector<int> v;
    std_e::for_each(t, [&](auto& t){ v.push_back(t.i); });

    vector<int> v_expected = {1, 2, 3};
    CHECK( v == v_expected );
  }

  SUBCASE("tuple<vector<S>...> ") {
    tuple<vector<S1>,vector<S2>,vector<S3>> t = {{S1{3},S1{2},S1{1}} , {S2{-9}}, {S3{30}}};

    vector<int> v;
    std_e::for_each(t, [&](auto& vt){
      std::for_each(begin(vt), end(vt), [&](auto& x){v.push_back(x.i);});
    });

    vector<int> v_expected = {3, 2, 1, -9, 30};
    CHECK(v ==  v_expected);
  }
}


TEST_CASE("tuple find_apply") {
  tuple<S1, S2, S3> t = {S1{1}, S2{2}, S3{3}};

  int cnt = 0;
  auto f = [&cnt](auto x){ cnt = 10*x.i; };


  SUBCASE("found") {
    auto pred = [](auto x){ return x.i==2; };

    auto idx_found = std_e::find_apply(t,pred,f);

    CHECK( cnt == 10*2 );
    CHECK( idx_found == 1 );
  }


  SUBCASE("not found") {
    auto always_false = [](auto /*x*/){ return false; };

    auto idx_found = std_e::find_apply(t,always_false,f);

    CHECK( cnt == 0 );
    CHECK( idx_found == 3 );
  }
}

TEST_CASE("tuple for_each_until") {
  tuple<S1, S2, S3> t = {S1{1}, S2{2}, S3{3}};

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
    auto f = [](auto& x){
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


constexpr auto my_transform_0 = [](const auto& x) -> int { return x.i; };
constexpr auto my_transform_1 = [](auto& x) -> int { x.i *= 2; return x.i+3; };
constexpr auto my_transform_2 = [](auto& x) { return decltype(x){x.i*3}; };
constexpr auto my_transform_3 = [](auto& x) { x.i *= 2; };

TEST_CASE("tuple transform") {
  SUBCASE("const function, uniform return type") {
    const tuple<S1,S2,S3> t = {S1{1}, S2{2}, S3{3}};
    std::array<int,3> res = std_e::transform(t, my_transform_0);
    CHECK( res == std::array{1,2,3} );
  }

  SUBCASE("non-const function, uniform return type") {
    tuple<S1,S2,S3> t = {S1{1}, S2{2}, S3{3}};
    std::array<int,3> res = std_e::transform(t, my_transform_1);
    CHECK( std::get<0>(t).i == 2 );
    CHECK( std::get<1>(t).i == 4 );
    CHECK( std::get<2>(t).i == 6 );
    CHECK( res == std::array{5,7,9} );
  }
  SUBCASE("const function, non-uniform return type") {
    const tuple<S1,S2,S3> t = {S1{1}, S2{2}, S3{3}};
    std::tuple<S1,S2,S3> res = std_e::transform(t, my_transform_2);
    CHECK( std::get<0>(res).i == 3 );
    CHECK( std::get<1>(res).i == 6 );
    CHECK( std::get<2>(res).i == 9 );
  }
  SUBCASE("non-const function, void return type") {
    tuple<S1,S2,S3> t = {S1{1}, S2{2}, S3{3}};
    std_e::transform(t, my_transform_3);
    CHECK( std::get<0>(t).i == 2 );
    CHECK( std::get<1>(t).i == 4 );
    CHECK( std::get<2>(t).i == 6 );
  }
}

}
