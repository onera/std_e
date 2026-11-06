#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"
#include "std_e/data_structure/multi_range/cmulti_range.hpp"
#include "std_e/data_structure/block_range/block_range.hpp"
#include "std_e/future/sort/sort_ranges.hpp"

using std::vector;
using std_e::span;
using std_e::span_ref;
using std_e::element;
using std_e::cmulti_range;
using std_e::cmulti_vector;
using std_e::cmulti_span;
using std_e::multi_span;
#if defined REAL_GCC && __GNUC__ < 11
  using std::get; // found by ADL with GCC 11
#endif

TEST_CASE("cmulti_vector") {
  cmulti_vector<int,2> t = {{42,43,0, 1000,500,10}};

  SUBCASE("range_view") {
    vector<int> expected_row0 = {42,43,0};
    vector<int> expected_row1 = {1000,500,10};

    SUBCASE("range_view") {
      SUBCASE("by index") {
        span<int> row0 = std_e::range_view<0>(t);
        span<int> row1 = std_e::range_view<1>(t);
        CHECK( row0 == expected_row0 );
        CHECK( row1 == expected_row1 );
      }
      SUBCASE("by index if const") {
        const auto& t_const_ref = t;
        span<const int> row0 = std_e::range_view<0>(t_const_ref);
        span<const int> row1 = std_e::range_view<1>(t_const_ref);
        CHECK( row0 == expected_row0 );
        CHECK( row1 == expected_row1 );
      }

      SUBCASE("view points to underlying") {
        // modifying the view modifies the underlying object
        CHECK( t.underlying() == vector{42,43,0, 1000,500,10} );
        span<int> row0 = std_e::range_view<0>(t);
        span<int> row1 = std_e::range_view<1>(t);
        REQUIRE( row0.data() == t.underlying().data() );
        REQUIRE( row1.data() == t.underlying().data()+3 );
        row0[1] = 100000;
        CHECK( t.underlying() == vector{42,100000,0, 1000,500,10} );
      }

      SUBCASE("view can be rebound") {
        // operator= is rebinding the view
        CHECK( t.underlying() == vector{42,43,0, 1000,500,10} );
        span<int> row0 = std_e::range_view<0>(t);
        REQUIRE( row0.data() == t.underlying().data() );
        row0[1] = 100000;
        std::vector v = {4,4,4};
        row0 = v;
        REQUIRE( row0.data() == v.data() );
        row0[2] = 20000;
        CHECK( t.underlying() == vector{42,100000,0, 1000,500,10} );
        CHECK( v == vector{4,4,20000} );
      }
    }
    SUBCASE("range_ref") {
      SUBCASE("by index") {
        span_ref<int> row0 = std_e::range_ref<0>(t);
        span_ref<int> row1 = std_e::range_ref<1>(t);
        CHECK( row0 == expected_row0 );
        CHECK( row1 == expected_row1 );
      }
      SUBCASE("ref points to underlying") {
        // modifying the ref modifies the underlying object (same as view)
        CHECK( t.underlying() == vector{42,43,0, 1000,500,10} );
        span_ref<int> row0 = std_e::range_ref<0>(t);
        span_ref<int> row1 = std_e::range_ref<1>(t);
        REQUIRE( row0.data() == t.underlying().data() );
        REQUIRE( row1.data() == t.underlying().data()+3 );
        row0[1] = 100000;
        CHECK( t.underlying() == vector{42,100000,0, 1000,500,10} );
      }

      SUBCASE("ref assigns through") {
        // operator= assigns through to the underlying object (contrary to view)
        CHECK( t.underlying() == vector{42,43,0, 1000,500,10} );
        span_ref<int> row0 = std_e::range_ref<0>(t);
        REQUIRE( row0.data() == t.underlying().data() );
        row0[1] = 100000;
        std::vector v = {4,4,4};
        row0 = v;
        REQUIRE( row0.data() == t.underlying().data() ); // still bound to the original mr
        row0[2] = 20000;
        CHECK( t.underlying() == vector{4,4,20000, 1000,500,10} );
        CHECK( v == vector{4,4,4} );
      }
    }
  }

  SUBCASE("indexing") {
    std_e::array_ref<int,2> col0 = t[0];
    std_e::array_ref<int,2> col1 = t[1];
    std_e::array_ref<int,2> col2 = t[2];
    CHECK( col0[0] == 42 ); CHECK( col1[0] == 43 ); CHECK( col2[0] ==  0 );
    CHECK( col0[1] == 1000); CHECK( col1[1] == 500); CHECK( col2[1] == 10);
  }

  //SUBCASE("structured bindings") { // TODO make it work
  //  //auto [x,y] = std::move(t);
  //  auto [x,y] = t;
  //  //auto& [x,y] = t;
  //  CHECK( x == vector{ 42, 43,  0} );
  //  CHECK( y == vector{1000,500,10} );

  //  x[0] = 1000;
  //  CHECK( range_view<0>(t) == vector{1000, 43, 0} );
  //}
}

TEST_CASE("cmulti_vector ctor with size and element") {
  cmulti_vector<int,2> t(3);
  std_e::element<0>(t,0) =  42 ; std_e::element<0>(t,1) =  43;  std_e::element<0>(t,2) =  0;
  std_e::element<1>(t,0) = 1000; std_e::element<1>(t,1) = 500;  std_e::element<1>(t,2) = 10;

  auto row0 = std_e::range_view<0>(t);
  auto row1 = std_e::range_view<1>(t);

  vector<int> expected_row0 = {  42, 43 ,0};
  vector<int> expected_row1 = {1000,500,10};

  CHECK( row0 == expected_row0 );
  CHECK( row1 == expected_row1 );
}

TEST_CASE("cmulti_vector operator()") {
  cmulti_vector<int,2> t(3);
  t[0,0] =  42 ;  t[0,1] =  43;  t[0,2] =  0;
  t[1,0] = 1000;  t[1,1] = 500;  t[1,2] = 10;

  auto row0 = std_e::range_view<0>(t);
  auto row1 = std_e::range_view<1>(t);

  vector<int> expected_row0 = {  42, 43 ,0};
  vector<int> expected_row1 = {1000,500,10};

  CHECK( row0 == expected_row0 );
  CHECK( row1 == expected_row1 );
}

TEST_CASE("algorithms") {
  cmulti_vector<int,2> t = {{42,43,0, 1000,500,10}};

  SUBCASE("find algos") {
    SUBCASE("find") {
      auto it = std::find(t.begin(),t.end(),std_e::array<int,2>{43,500});
      CHECK( it == t.begin()+1 );
    }
    SUBCASE("find_if") {
      auto first_eq_to_43 = [](std_e::array_ref<int,2>&& x){ return x[0]==43; };
      auto it = std::find_if(t.begin(),t.end(),first_eq_to_43);
      CHECK( it == t.begin()+1 );

      auto first_eq_to_999 = [](std_e::array_ref<int,2>&& x){ return x[0]==999; };
      auto it_not_found = std::find_if(t.begin(),t.end(),first_eq_to_999);
      CHECK( it_not_found == t.end() );
    }
  }

  SUBCASE("sort") {
  // TODO
    //SUBCASE("sanity: one range_view") {
    //  cmulti_vector<int,1> single = {{4,3,9,8,0,1,2,7,6,5}};

    //  //using _Out = std_e::multi_range_iterator<std_e::cmulti_range<std::vector<int, std::allocator<int> >, 1> >;
    //  //using _Tp = std_e::array_ref<int, 1>;
    //  //_Out&& __o = single.begin();
    //  //int i;
    //  //_Tp&& __t = std_e::array_ref<int, 1>{i};
    //  //const_cast<const decltype(*(std::declval<_Tp&>)())&&>(*__o) =(std::forward<_Tp>)(__t);

    //  std_e::ranges::sort(single);

    //  //CHECK( range_view<0>(single) == vector{0,1,2,3,4,5,6,7,8,9} );
    //}

    //SUBCASE("more ranges") {
    //  std_e::ranges::sort(t);
    //  // check
    //  const vector<int> expected_row0 = {0 ,   42,  43};
    //  const vector<int> expected_row1 = {10, 1000, 500};
    //  auto row0 = std_e::range_view<0>(t);
    //  auto row1 = std_e::range_view<1>(t);
    //  CHECK( row0 == expected_row0 );
    //  CHECK( row1 == expected_row1 );
    //}

    //SUBCASE("more ranges - other comp") {
    //  std_e::ranges::sort(t, std_e::crange_proj<1>);
    //  // check
    //  const vector<int> expected_row0 = {0 ,  43,   42};
    //  const vector<int> expected_row1 = {10, 500, 1000};
    //  auto row0 = std_e::range_view<0>(t);
    //  auto row1 = std_e::range_view<1>(t);
    //  CHECK( row0 == expected_row0 );
    //  CHECK( row1 == expected_row1 );
    //}
  }
}
TEST_CASE("cmulti_range of block_range") { // case with proxy reference of proxy reference
  //vector<int> x = {10,4,5,  6,3,12};
  //auto xb = std_e::view_as_block_range<3>(x);

  //auto t = make_cmulti_range<1>(xb);

  //// TODO
  ////// Note: As of GCC 11, std::ranges::sort is broken for proxy references
  //////     see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=104561
  //////std::ranges::sort(t.begin(),t.end());
  ////std_e::quicksort(t.begin(),t.end());
  ////CHECK( x == vector{6,3,12,  10,4,5} );
}
TEST_CASE("cmulti_range of block_range,vector") { // case with proxy reference of mixed proxy reference/real references
  //vector<int> x = {10,4,5, 6,3,12,    100,101,102, 3,2,1};
  //auto xb = std_e::view_as_block_range<3>(x);
  //auto t = make_cmulti_range<2>(xb);

  //// TODO
  ////// Note: As of GCC 11, std::ranges::sort is broken for proxy references
  //////     see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=104561
  //////std::ranges::sort(t.begin(),t.end());
  ////std_e::ranges::sort(t, std_e::proj<0>);
  ////CHECK( x == vector{6,3,12,  10,4,5,    3,2,1, 100,101,102} );
}


TEST_CASE("cmulti_span") {
  vector<int> v = {42,43,0,  1000,500,10};
  cmulti_span<int,2> t(std_e::make_span(v));

  SUBCASE("range_view") {
    vector<int> expected_row0 = {42,43,0};
    vector<int> expected_row1 = {1000,500,10};

    SUBCASE("by index") {
      auto row0 = std_e::range_view<0>(t);
      auto row1 = std_e::range_view<1>(t);
      CHECK( row0 == expected_row0 );
      CHECK( row1 == expected_row1 );
    }
    SUBCASE("is view") {
      auto row0 = std_e::range_view<0>(t);
      row0[0] = 50;
      CHECK( v[0] == 50 );
    }
  }
}

TEST_CASE("multi_range_view") {
  cmulti_vector<int,2> mv = {{42,43,0, 1000,500,10}};
  multi_span<int,int> mvv = mv.multi_range_view();
  vector expected_row0 = {42,43,0};
  vector expected_row1 = {1000,500,10};
  auto row0 = std_e::range_view<0>(mvv);
  auto row1 = std_e::range_view<1>(mvv);
  CHECK( row0 == expected_row0 );
  CHECK( row1 == expected_row1 );
}

TEST_CASE("views") {
  cmulti_vector<int,2> mv = {{42,43,0, 1000,500,10}};

  SUBCASE("complete view") {
    vector expected_row0 = {42,43,0};
    vector expected_row1 = {1000,500,10};

    SUBCASE("non-const") {
      cmulti_span ms = view(mv);

      auto row0 = std_e::range_view<0>(ms);
      auto row1 = std_e::range_view<1>(ms);
      CHECK( row0 == expected_row0 );
      CHECK( row1 == expected_row1 );
    }
    SUBCASE("const") {
      const auto& cmv = mv;
      cmulti_span ms = view(cmv);

      auto row0 = std_e::range_view<0>(ms);
      auto row1 = std_e::range_view<1>(ms);
      CHECK( row0 == expected_row0 );
      CHECK( row1 == expected_row1 );
    }
  }
  SUBCASE("partial view") {
    vector expected_row0 = {43,0};
    vector expected_row1 = {500,10};

    multi_span ms = subview_n(mv,1,2);

    auto row0 = std_e::range_view<0>(ms);
    auto row1 = std_e::range_view<1>(ms);
    CHECK( row0 == expected_row0 );
    CHECK( row1 == expected_row1 );
  }
}
#endif // C++>17
