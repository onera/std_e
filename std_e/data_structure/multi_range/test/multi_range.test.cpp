#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"
#include "std_e/data_structure/multi_range/multi_range.hpp"
#include "std_e/data_structure/block_range/block_range.hpp"
#include "std_e/future/sort/sort_ranges.hpp"

#include <string>



using std::vector;
using std::string;
using std_e::span;
using std_e::span_ref;
using std_e::element;
using std_e::multi_range2;
using std_e::multi_vector2;
using std_e::multi_span2;
#if defined REAL_GCC && __GNUC__ < 11
  using std::get; // found by ADL with GCC 11
#endif

TEST_CASE("multi_vector2") {
  multi_vector2<int,string,double> t;
  t.push_back(42,"X",3.14);
  t.push_back(43,"Y",2.7);
  t.push_back(0,"ABC",100.);

  SUBCASE("range_view") {
    vector<int>    expected_col0 = {42,43,0};
    vector<string> expected_col1 = {"X","Y","ABC"};
    vector<double> expected_col2 = {3.14,2.7,100.};

    SUBCASE("range_view") {
      SUBCASE("by index") {
        span<int>    col0 = std_e::range_view<0>(t);
        span<string> col1 = std_e::range_view<1>(t);
        span<double> col2 = std_e::range_view<2>(t);
        CHECK( col0 == expected_col0 );
        CHECK( col1 == expected_col1 );
        CHECK( col2 == expected_col2 );
      }
      SUBCASE("by index if const") {
        const auto& t_const_ref = t;
        span<const int>    col0 = std_e::range_view<0>(t_const_ref);
        span<const string> col1 = std_e::range_view<1>(t_const_ref);
        span<const double> col2 = std_e::range_view<2>(t_const_ref);
        CHECK( col0 == expected_col0 );
        CHECK( col1 == expected_col1 );
        CHECK( col2 == expected_col2 );
      }
      SUBCASE("by type") {
        span<int>    col0 = std_e::range_view<int>(t);
        span<string> col1 = std_e::range_view<string>(t);
        span<double> col2 = std_e::range_view<double>(t);
        CHECK( col0 == expected_col0 );
        CHECK( col1 == expected_col1 );
        CHECK( col2 == expected_col2 );
      }

      SUBCASE("view points to underlying") {
        // modifying the view modifies the underlying object
        CHECK( std::get<0>(t.underlying_tuple()) == vector{42,43,0} );
        span<int> col0 = std_e::range_view<0>(t);
        REQUIRE( col0.data() == std::get<0>(t.underlying_tuple()).data() );
        col0[1] = 10000;
        CHECK( std::get<0>(t.underlying_tuple()) == vector{42,10000,0} );
      }

      SUBCASE("view can be rebound") {
        // operator= is rebinding the view
        CHECK( std::get<0>(t.underlying_tuple()) == vector{42,43,0} );
        span<int> col0 = std_e::range_view<0>(t);
        REQUIRE( col0.data() == std::get<0>(t.underlying_tuple()).data() );
        col0[1] = 10000;
        std::vector v = {4,4,4};
        col0 = v;
        REQUIRE( col0.data() == v.data() );
        col0[2] = 20000;
        CHECK( std::get<0>(t.underlying_tuple()) == vector{42,10000,0} );
        CHECK( v == vector{4,4,20000} );
      }
    }
    SUBCASE("range_ref") {
      SUBCASE("by index") {
        span_ref<int>    col0 = std_e::range_ref<0>(t);
        span_ref<string> col1 = std_e::range_ref<1>(t);
        span_ref<double> col2 = std_e::range_ref<2>(t);
        CHECK( col0 == expected_col0 );
        CHECK( col1 == expected_col1 );
        CHECK( col2 == expected_col2 );
      }
      SUBCASE("ref points to underlying") {
        // modifying the ref modifies the underlying object (same as view)
        CHECK( std::get<0>(t.underlying_tuple()) == vector{42,43,0} );
        span_ref<int> col0 = std_e::range_ref<0>(t);
        REQUIRE( col0.data() == std::get<0>(t.underlying_tuple()).data() );
        col0[1] = 10000;
        CHECK( std::get<0>(t.underlying_tuple()) == vector{42,10000,0} );
      }

      SUBCASE("ref assigns through") {
        // operator= assigns through to the underlying object (contrary to view)
        CHECK( std::get<0>(t.underlying_tuple()) == vector{42,43,0} );
        span_ref<int> col0 = std_e::range_ref<0>(t);
        REQUIRE( col0.data() == std::get<0>(t.underlying_tuple()).data() );
        col0[1] = 10000;
        std::vector v = {4,4,4};
        col0 = v;
        REQUIRE( col0.data() == std::get<0>(t.underlying_tuple()).data() ); // still bound to the original mr
        col0[2] = 20000;
        CHECK( std::get<0>(t.underlying_tuple()) == vector{4,4,20000} );
        CHECK( v == vector{4,4,4} );
      }
    }
  }

  SUBCASE("indexing") {
    std_e::tuple<int&,string&,double&> row0 = t[0];
    std_e::tuple<int&,string&,double&> row1 = t[1];
    std_e::tuple<int&,string&,double&> row2 = t[2];
    CHECK( get<0>(row0) == 42 ); CHECK( get<1>(row0) == "X"  ); CHECK( get<2>(row0) == 3.14 );
    CHECK( get<0>(row1) == 43 ); CHECK( get<1>(row1) == "Y"  ); CHECK( get<2>(row1) == 2.7  );
    CHECK( get<0>(row2) ==  0 ); CHECK( get<1>(row2) == "ABC"); CHECK( get<2>(row2) == 100. );
  }

  SUBCASE("structured bindings") { // TODO more refined around value/view/ref concepts
    auto& [x,y,z] = t;
    CHECK( x == vector<int>   {42  ,43 ,0    } );
    CHECK( y == vector<string>{"X" ,"Y","ABC"} );
    CHECK( z == vector<double>{3.14,2.7,100.} );

    x[0] = 100;
    CHECK( range_view<0>(t) == vector{100, 43, 0} );
  }
}

TEST_CASE("Ctor argument deduction") {
  multi_vector2 t( vector<int>{4,2}, vector<double>{1.5,9.9} );

  CHECK( range_view<0>(t) == vector{4,2} );
  CHECK( range_view<1>(t) == vector{1.5,9.9} );
}

TEST_CASE("multi_vector2 ctor with size") {
  multi_vector2<int,string,double> t(2);
  std_e::element<0>(t,0) = 42; std_e::element<1>(t,0) = "X"; std_e::element<2>(t,0) = 3.14;
  std_e::element<0>(t,1) = 43; std_e::element<1>(t,1) = "Y"; std_e::element<2>(t,1) = 2.7;

  auto col0 = std_e::range_view<0>(t);
  auto col1 = std_e::range_view<1>(t);
  auto col2 = std_e::range_view<2>(t);

  vector<int>    expected_col0 = {42,43};
  vector<string> expected_col1 = {"X","Y"};
  vector<double> expected_col2 = {3.14,2.7};

  CHECK( col0 == expected_col0 );
  CHECK( col1 == expected_col1 );
  CHECK( col2 == expected_col2 );

}

TEST_CASE("algorithms") {
  multi_vector2<int,string,double> t;
  t.push_back(42,"X"  ,3.14);
  t.push_back(43,"Y"  ,2.7);
  t.push_back(0 ,"ABC",100.);

  SUBCASE("find algos") {
    SUBCASE("find") {
      auto it = std::find(t.begin(),t.end(),std_e::tuple<int,string,double>{43,string("Y"),2.7});
      CHECK( it == t.begin()+1 );
    }
    SUBCASE("find_if") {
      auto first_eq_to_43 = [](const std_e::tuple<int&,string&,double&>& x){ return get<0>(x)==43; };
      auto it = std::find_if(t.begin(),t.end(),first_eq_to_43);
      CHECK( it == t.begin()+1 );

      auto first_eq_to_100 = [](const std_e::tuple<int&,string&,double&>& x){ return get<0>(x)==999; };
      auto it_not_found = std::find_if(t.begin(),t.end(),first_eq_to_100);
      CHECK( it_not_found == t.end() );
    }
  }
  SUBCASE("sort") {
    SUBCASE("sanity: one range_view") {
      multi_vector2<int> single(vector{4,3,9,8,0,1,2,7,6,5});

      std_e::ranges::sort(single);

      CHECK( range_view<0>(single) == vector{0,1,2,3,4,5,6,7,8,9} );
    }

    SUBCASE("more ranges") {
      std_e::ranges::sort(t);
      // check
      const vector<int>    expected_col0 = {0    ,42  ,43 };
      const vector<string> expected_col1 = {"ABC","X" ,"Y"};
      const vector<double> expected_col2 = {100. ,3.14,2.7};
      auto col0 = std_e::range_view<0>(t);
      auto col1 = std_e::range_view<1>(t);
      auto col2 = std_e::range_view<2>(t);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }

    SUBCASE("more ranges - other comp") {
      std_e::ranges::sort(t, std_e::proj<2>);
      // check
      const vector<int>    expected_col0 = {43 ,42  ,0    };
      const vector<string> expected_col1 = {"Y","X" ,"ABC"};
      const vector<double> expected_col2 = {2.7,3.14,100. };
      auto col0 = std_e::range_view<0>(t);
      auto col1 = std_e::range_view<1>(t);
      auto col2 = std_e::range_view<2>(t);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
  }
}
TEST_CASE("multi_range2 of block_range") { // case with proxy reference of proxy reference
  vector<int> x = {10,4,5,  6,3,12};
  auto xb = std_e::view_as_block_range<3>(x);

  multi_range2 t(xb);

  // Note: As of GCC 11, std::ranges::sort is broken for proxy references
  //     see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=104561
  //std::ranges::sort(t.begin(),t.end());
  std_e::quicksort(t.begin(),t.end());
  CHECK( x == vector{6,3,12,  10,4,5} );
}
TEST_CASE("multi_range2 of block_range,vector") { // case with proxy reference of mixed proxy reference/real references
  vector<int> x = {10,4,5,  6,3,12};
  vector<int> y = {100   ,  200};

  auto xb = std_e::view_as_block_range<3>(x);
  auto ys = std_e::span<int>(y.data(),y.size());

  multi_range2 t(xb,ys);

  // Note: As of GCC 11, std::ranges::sort is broken for proxy references
  //     see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=104561
  //std::ranges::sort(t.begin(),t.end());
  std_e::ranges::sort(t, std_e::proj<0>);
  CHECK( x == vector{6,3,12,  10,4,5} );
  CHECK( y == vector{200   ,  100} );
}


TEST_CASE("multi_vector2 ctor from vectors") {
  vector<int>    x = {42,43,0};
  vector<string> y = {"X","Y","ABC"};
  multi_vector2 t(std::move(x),std::move(y));

  vector<int>    expected_col0 = {42,43,0};
  vector<string> expected_col1 = {"X","Y","ABC"};

  auto col0 = std_e::range_view<0>(t);
  auto col1 = std_e::range_view<1>(t);

  CHECK( col0 == expected_col0 );
  CHECK( col1 == expected_col1 );
}
TEST_CASE("zip") {
  vector<int>    x = {42,43,0};
  vector<string> y = {"X","Y","ABC"};
  auto t = std_e::zip(std::move(x),std::move(y));
  CHECK(element<0>(t,0) == 42 ); CHECK(element<0>(t,1) == 43 ); CHECK(element<0>(t,2) == 0);
  CHECK(element<1>(t,0) == "X"); CHECK(element<1>(t,1) == "Y"); CHECK(element<1>(t,2) == "ABC");
}
TEST_CASE("zip_view") {
  vector<int>    x = {42,43,0};
  vector<string> y = {"X","Y","ABC"};
  auto t = std_e::zip_view(x,y);

  auto x_view = std_e::range_view<0>(t);
  auto y_view = std_e::range_view<1>(t);

  CHECK( x_view == vector<int>{42,43,0} );
  CHECK( y_view == vector<string>{"X","Y","ABC"} );

  // check that the memory is shared with the original x and y objects
  x_view[0] = 97;
  y_view[2] = "Z";
  CHECK( x_view == vector{97,43,0} );
  CHECK( x      == vector{97,43,0} );
  CHECK( y_view == vector<string>{"X","Y","Z"} );
  CHECK( y      == vector<string>{"X","Y","Z"} );
}

TEST_CASE("multi_span2") {
  vector<int> vi = {42,43,0};
  vector<string> vs = {"X","Y","ABC"};
  vector<double> vd = {3.14,2.7,100.};
  multi_span2 t(std_e::make_span(vi),std_e::make_span(vs),std_e::make_span(vd));

  SUBCASE("range_view") {
    vector<int>    expected_col0 = {42,43,0};
    vector<string> expected_col1 = {"X","Y","ABC"};
    vector<double> expected_col2 = {3.14,2.7,100.};

    SUBCASE("by index") {
      auto col0 = std_e::range_view<0>(t);
      auto col1 = std_e::range_view<1>(t);
      auto col2 = std_e::range_view<2>(t);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
    SUBCASE("by type") {
      auto col0 = std_e::range_view<int>(t);
      auto col1 = std_e::range_view<string>(t);
      auto col2 = std_e::range_view<double>(t);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
    SUBCASE("is view") {
      auto col0 = std_e::range_view<int>(t);
      col0[0] = 50;
      CHECK( vi[0] == 50 );
    }
  }
}
TEST_CASE("views") {
  multi_vector2<int,string,double> mv;
  mv.push_back(42,"X",3.14);
  mv.push_back(43,"Y",2.7);
  mv.push_back(0,"ABC",100.);

  SUBCASE("complete view") {
    vector<int>    expected_col0 = {42,43,0};
    vector<string> expected_col1 = {"X","Y","ABC"};
    vector<double> expected_col2 = {3.14,2.7,100.};

    SUBCASE("non-const") {
      multi_span2 ms = make_span(mv);

      auto col0 = std_e::range_view<0>(ms);
      auto col1 = std_e::range_view<1>(ms);
      auto col2 = std_e::range_view<2>(ms);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
    SUBCASE("const") {
      const auto& cmv = mv;
      multi_span2 ms = make_span(cmv);

      auto col0 = std_e::range_view<0>(ms);
      auto col1 = std_e::range_view<1>(ms);
      auto col2 = std_e::range_view<2>(ms);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
  }
  SUBCASE("partial view") {
    vector<int>    expected_col0 = {43,0};
    vector<string> expected_col1 = {"Y","ABC"};
    vector<double> expected_col2 = {2.7,100.};

    multi_span2 ms = make_span_n(mv,1,2);

    auto col0 = std_e::range_view<0>(ms);
    auto col1 = std_e::range_view<1>(ms);
    auto col2 = std_e::range_view<2>(ms);
    CHECK( col0 == expected_col0 );
    CHECK( col1 == expected_col1 );
    CHECK( col2 == expected_col2 );
  }
}
#endif // C++>17
