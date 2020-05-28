#include "std_e/unit_test/doctest.hpp"
#include "std_e/multi_index/multi_index.hpp"

using namespace std_e;

// [Sphinx Doc] multi_index {
TEST_CASE("multi_index") {
  multi_index<int,4> is = {5,4,3,2};

  SUBCASE("structured bindings") {
    auto [i,j,k,l] = is;
    CHECK ( i == 5 );
    CHECK ( j == 4 );
    CHECK ( k == 3 );
    CHECK ( l == 2 );
  }

  SUBCASE("multi_index operations") {
    static_assert(rank_of<decltype(is)> == 4);
    static_assert(decltype(is)::rank() == 4);
    CHECK( is.rank() == 4 );

    static_assert(std::is_same_v< index_type_of<decltype(is)> , int >);
  }

  SUBCASE("array operations") {
    CHECK( is.size() == 4 ); // size() == rank()
    CHECK( is[0] == 5 );
    CHECK( is[1] == 4 );
    CHECK( is[2] == 3 );
    CHECK( is[3] == 2 );
  }

  SUBCASE("satisfies the Multi_index concept") {
    static_assert(is_multi_index<decltype(is)>);
  }
}
// [Sphinx Doc] multi_index }

// [Sphinx Doc] dynamic multi_index {
TEST_CASE("dynamic multi_index") {
  multi_index<int,dynamic_size> is;

  static_assert(rank_of<decltype(is)> == dynamic_size);

  CHECK( is.rank() == 0 );

  is.resize(2);
  CHECK( is.rank() == 2 );
}
// [Sphinx Doc] dynamic multi_index }
