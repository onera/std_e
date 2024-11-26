#include "std_e/unit_test/doctest.hpp"
#include "std_e/graph/csr/struct.hpp"
#include "std_e/future/dynarray.hpp"
#include "std_e/future/span.hpp"

using namespace std_e;

TEST_CASE("Edge_node_view") {
  int n_node = 5;
  dynarray<int> l = {0, 1, 2, 3};
  dynarray<int> r = {1, 2, 4,-1};

  Edge_node_view x(n_node, l, r);

  SUBCASE("base interface") {
    CHECK( x.n_node() == 5 );
    CHECK( x.n_edge() == 4 );
    CHECK( x.left () == dynarray{0, 1, 2, 3} );
    CHECK( x.right() == dynarray{1, 2, 4,-1} );
  }

  SUBCASE("is view") {
    x.left()[2] = 100;
    r[0] = 200;

    CHECK( l         == dynarray{0  , 1, 100, 3} );
    CHECK( r         == dynarray{200, 2, 4  ,-1} );
    CHECK( x.left () == dynarray{0  , 1, 100, 3} );
    CHECK( x.right() == dynarray{200, 2, 4  ,-1} );

    CHECK( x.left ().data() == l.data() );
    CHECK( x.right().data() == r.data() );
  }

  SUBCASE("multi_range interface") {
    CHECK( x.size() == 4 );
    auto [l3,r3] = x[3];
    CHECK( l3 ==  3 );
    CHECK( r3 == -1 );
  }

  SUBCASE("copy") {
    Edge_node y = x.copy();
    // same values
    CHECK( y.n_node() == 5 );
    CHECK( y.n_edge() == 4 );
    CHECK( y.left () == dynarray{0, 1, 2, 3} );
    CHECK( y.right() == dynarray{1, 2, 4,-1} );

    // memory is not shared (true copy)
    CHECK( y.left ().data() != x.left ().data() );
    CHECK( y.right().data() != x.right().data() );

    // both memory spaces are independent
    x.left ()[2] = 100;
    CHECK( y.left ()[2] == 2 );

    y.right()[0] = 200;
    CHECK( x.right()[0] == 1 );
  }
}
