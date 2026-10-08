#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"
#include "std_e/future/array_ref.hpp"


TEST_CASE("array_ref") {
  int i = 10;
  int j = 20;
  std_e::array_ref<int,2> a = {i,j};

  SUBCASE("is a reference type") {
    CHECK(a.size() == 2);
    CHECK(&a[0] == &i);
    CHECK(&a[1] == &j);
    CHECK(a[0] == i);
    CHECK(a[1] == j);

    a = std::vector{12,13};
    CHECK(&a[0] == &i);
    CHECK(&a[1] == &j);
    CHECK(a[0] == 12);
    CHECK(a[1] == 13);
    CHECK(i == 12);
    CHECK(j == 13);
  }

  SUBCASE("iteration") {
    for (auto& x : a) {
      x *= 2;
    }
    CHECK(a[0] == 20);
    CHECK(a[1] == 40);
    CHECK(i == 20);
    CHECK(j == 40);
  }

  SUBCASE("equality") {
    int k = 10;
    int l = 20;
    std_e::array_ref<int,2> b = {k,l};
    int m = 10;
    int n = 11;
    std_e::array_ref<int,2> c = {m,n};
    CHECK( a == b );
    CHECK( a != c );

    std::vector v0 = {10,20};
    std::vector v1 = {10,20,30};
    std::vector v2 = {10};
    std::vector v3 = {10,11};
    CHECK( a == v0 );
    CHECK( a != v1 );
    CHECK( a != v2 );
    CHECK( a != v3 );
  }

  SUBCASE("structured bindings") {
    auto& [ii,jj] = a;
    CHECK( ii == 10 );
    CHECK( jj == 20 );
  }
}
#endif // C++>17
