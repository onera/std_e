#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"
#include "std_e/data_structure/multi_range/multi_range.hpp"
#include "std_e/data_structure/block_range/block_range.hpp"
#include "std_e/future/sort/sort_ranges.hpp"

#include <string>



// TODO RENAME cell->elt, row->multi_elt, col->range
using std::vector;
using std::string;
#if defined REAL_GCC && __GNUC__ < 11
  using std::get; // found by ADL with GCC 11
#endif

TEST_CASE("multi_vector2") {
  std_e::multi_vector2<int,string,double> t;
  t.push_back(42,"X",3.14);
  t.push_back(43,"Y",2.7);
  t.push_back(0,"ABC",100.);

  SUBCASE("range") {
    vector<int>         expected_col0 = {42,43,0};
    vector<string> expected_col1 = {"X","Y","ABC"};
    vector<double>      expected_col2 = {3.14,2.7,100.};

    SUBCASE("by index") {
      vector<int>&         col0 = std_e::range<0>(t);
      vector<string>& col1 = std_e::range<1>(t);
      vector<double>&      col2 = std_e::range<2>(t);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
    //SUBCASE("by type") {
    //  vector<int>&         col0 = std_e::range<int>(t);
    //  vector<string>& col1 = std_e::range<string>(t);
    //  vector<double>&      col2 = std_e::range<double>(t);
    //  CHECK( col0 == expected_col0 );
    //  CHECK( col1 == expected_col1 );
    //  CHECK( col2 == expected_col2 );
    //}
  }

  SUBCASE("indexing") {
    std_e::tuple<int&,string&,double&> row0 = t[0];
    std_e::tuple<int&,string&,double&> row1 = t[1];
    std_e::tuple<int&,string&,double&> row2 = t[2];
    CHECK( get<0>(row0) == 42 ); CHECK( get<1>(row0) == "X"  ); CHECK( get<2>(row0) == 3.14 );
    CHECK( get<0>(row1) == 43 ); CHECK( get<1>(row1) == "Y"  ); CHECK( get<2>(row1) == 2.7  );
    CHECK( get<0>(row2) ==  0 ); CHECK( get<1>(row2) == "ABC"); CHECK( get<2>(row2) == 100. );
  }

  SUBCASE("structured bindings") {
    auto& [x,y,z] = t;
    CHECK( x == vector<int>        {42  ,43 ,0    } );
    CHECK( y == vector<string>{"X" ,"Y","ABC"} );
    CHECK( z == vector<double>     {3.14,2.7,100.} );

    x[0] = 100;
    CHECK( range<0>(t) == vector{100, 43, 0} );
  }
}

TEST_CASE("multi_vector2 ctor with size") {
  std_e::multi_vector2<int,string,double> t(2);
  std_e::element<0>(t,0) = 42; std_e::element<1>(t,0) = "X"; std_e::element<2>(t,0) = 3.14;
  std_e::element<0>(t,1) = 43; std_e::element<1>(t,1) = "Y"; std_e::element<2>(t,1) = 2.7;

  vector<int>         col0 = std_e::range<0>(t);
  vector<string> col1 = std_e::range<1>(t);
  vector<double>      col2 = std_e::range<2>(t);

  vector<int>         expected_col0 = {42,43};
  vector<string> expected_col1 = {"X","Y"};
  vector<double>      expected_col2 = {3.14,2.7};

  CHECK( col0 == expected_col0 );
  CHECK( col1 == expected_col1 );
  CHECK( col2 == expected_col2 );

}

TEST_CASE("algorithms") {
  std_e::multi_vector2<int,string,double> t;
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
    SUBCASE("sanity: one range") {
      std_e::multi_vector2<int> t(2);
      range<0>(t) = vector{4,3,9,8,0,1,2,7,6,5};

      std_e::ranges::sort(t);

      CHECK( range<0>(t) == vector{0,1,2,3,4,5,6,7,8,9} );
    }

    SUBCASE("more ranges") {
      std_e::ranges::sort(t);
      // check
      const vector<int>         expected_col0 = {0    ,42  ,43 };
      const vector<string> expected_col1 = {"ABC","X" ,"Y"};
      const vector<double>      expected_col2 = {100. ,3.14,2.7};
      vector<int>&         col0 = std_e::range<0>(t);
      vector<string>& col1 = std_e::range<1>(t);
      vector<double>&      col2 = std_e::range<2>(t);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }

    SUBCASE("more ranges - other comp") {
      auto proj = [](const auto& x){ return get<2>(x); };
      std_e::ranges::sort(t,{},proj);
      // check
      const vector<int>         expected_col0 = {43 ,42  ,0    };
      const vector<string> expected_col1 = {"Y","X" ,"ABC"};
      const vector<double>      expected_col2 = {2.7,3.14,100. };
      vector<int>&         col0 = std_e::range<0>(t);
      vector<string>& col1 = std_e::range<1>(t);
      vector<double>&      col2 = std_e::range<2>(t);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
  }
}
TEST_CASE("multi_range2 of block_range") { // case with proxy reference of proxy reference
  vector<int> x = {10,4,5,  6,3,12};
  using block_range_type = std_e::block_range<vector<int>&,3>;
  block_range_type xb = std_e::view_as_block_range<3>(x);

  std_e::multi_range2<block_range_type> t(xb);

  // Note: As of GCC 11, std::ranges::sort is broken for proxy references
  //     see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=104561
  //std::ranges::sort(t.begin(),t.end());
  std_e::quicksort(t.begin(),t.end());
  CHECK( x == vector{6,3,12,  10,4,5} );
}
TEST_CASE("multi_range2 of block_range,vector") { // case with proxy reference of mixed proxy reference/real references
  vector<int> x = {10,4,5,  6,3,12};
  vector<int> y = {100   ,  200};

  using block_range_type = std_e::block_range<vector<int>&,3>;
  block_range_type xb = std_e::view_as_block_range<3>(x);

  using span_type = std_e::span<int>;
  span_type ys (y.data(),y.size());

  std_e::multi_range2<block_range_type,span_type> t(xb,ys);

  // Note: As of GCC 11, std::ranges::sort is broken for proxy references
  //     see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=104561
  //std::ranges::sort(t.begin(),t.end());
  constexpr auto proj = [](const auto& x) -> decltype(auto) { return get<0>(x); };
  std_e::ranges::sort(t,{},proj);
  CHECK( x == vector{6,3,12,  10,4,5} );
  CHECK( y == vector{200   ,  100} );
}


TEST_CASE("multi_vector2 ctor from vectors") {
  vector<int>         x = {42,43,0};
  vector<string> y = {"X","Y","ABC"};
  std_e::multi_vector2<int,string> t(x,y);

  vector<int>         expected_col0 = {42,43,0};
  vector<string> expected_col1 = {"X","Y","ABC"};

  vector<int>         col0 = std_e::range<0>(t);
  vector<string> col1 = std_e::range<1>(t);

  CHECK( col0 == expected_col0 );
  CHECK( col1 == expected_col1 );
}
TEST_CASE("view_as_multi_range") {
  vector<int>         x = {42,43,0};
  vector<string> y = {"X","Y","ABC"};
  auto t = std_e::view_as_multi_range(x,y);

  vector<int>        & x_ref = std_e::range<0>(t);
  vector<string>& y_ref = std_e::range<1>(t);

  CHECK( x_ref == vector<int>{42,43,0} );
  CHECK( y_ref == vector<string>{"X","Y","ABC"} );

  x_ref = vector{97,98,99};
  y_ref[2] = "Z";
  CHECK( x     == vector<int>{97,98,99} );
  CHECK( x_ref == vector<int>{97,98,99} );
  CHECK( y     == vector<string>{"X","Y","Z"} );
  CHECK( y_ref == vector<string>{"X","Y","Z"} );
}

TEST_CASE("Ctor argument deduction") {
  std_e::multi_range2 t( vector<int>{4,2}, vector<double>{1.5,9.9} );

  CHECK( range<0>(t) == vector{4,2} );
  CHECK( range<1>(t) == vector{1.5,9.9} );
}

//TEST_CASE("multi_span") {
//  vector<int> vi = {42,43,0};
//  vector<string> vs = {"X","Y","ABC"};
//  vector<double> vd = {3.14,2.7,100.};
//  std_e::multi_span<int,string,double> t(std_e::make_span(vi),std_e::make_span(vs),std_e::make_span(vd));
//
//  SUBCASE("range") {
//    vector<int>         expected_col0 = {42,43,0};
//    vector<string> expected_col1 = {"X","Y","ABC"};
//    vector<double>      expected_col2 = {3.14,2.7,100.};
//
//    SUBCASE("by index") {
//      std_e::span<int>&         col0 = std_e::range<0>(t);
//      std_e::span<string>& col1 = std_e::range<1>(t);
//      std_e::span<double>&      col2 = std_e::range<2>(t);
//      CHECK( col0 == expected_col0 );
//      CHECK( col1 == expected_col1 );
//      CHECK( col2 == expected_col2 );
//    }
//    SUBCASE("by type") {
//      std_e::span<int>&         col0 = std_e::range<int>(t);
//      std_e::span<string>& col1 = std_e::range<string>(t);
//      std_e::span<double>&      col2 = std_e::range<double>(t);
//      CHECK( col0 == expected_col0 );
//      CHECK( col1 == expected_col1 );
//      CHECK( col2 == expected_col2 );
//    }
//    SUBCASE("is view") {
//      auto col0 = std_e::range<int>(t);
//      col0[0] = 50;
//      CHECK( vi[0] == 50 );
//    }
//  }
//}
//TEST_CASE("make_span") {
//  std_e::multi_vector2<int,string,double> mv;
//  mv.push_back(42,"X",3.14);
//  mv.push_back(43,"Y",2.7);
//  mv.push_back(0,"ABC",100.);
//
//  SUBCASE("complete view") {
//    vector<int>         expected_col0 = {42,43,0};
//    vector<string> expected_col1 = {"X","Y","ABC"};
//    vector<double>      expected_col2 = {3.14,2.7,100.};
//
//    SUBCASE("non-const") {
//      std_e::multi_span<int,string,double> ms = make_span(mv);
//
//      std_e::span<int>&         col0 = std_e::range<0>(ms);
//      std_e::span<string>& col1 = std_e::range<1>(ms);
//      std_e::span<double>&      col2 = std_e::range<2>(ms);
//      CHECK( col0 == expected_col0 );
//      CHECK( col1 == expected_col1 );
//      CHECK( col2 == expected_col2 );
//    }
//    SUBCASE("const") {
//      const auto& cmv = mv;
//      std_e::multi_span<const int,const string,const double> ms = make_span(cmv);
//
//      std_e::span<const int>&         col0 = std_e::range<0>(ms);
//      std_e::span<const string>& col1 = std_e::range<1>(ms);
//      std_e::span<const double>&      col2 = std_e::range<2>(ms);
//      CHECK( col0 == expected_col0 );
//      CHECK( col1 == expected_col1 );
//      CHECK( col2 == expected_col2 );
//    }
//  }
//  SUBCASE("partial view") {
//    vector<int>         expected_col0 = {43,0};
//    vector<string> expected_col1 = {"Y","ABC"};
//    vector<double>      expected_col2 = {2.7,100.};
//
//    std_e::multi_span<int,string,double> ms = make_span_n(mv,1,2);
//
//    std_e::span<int>&         col0 = std_e::range<0>(ms);
//    std_e::span<string>& col1 = std_e::range<1>(ms);
//    std_e::span<double>&      col2 = std_e::range<2>(ms);
//    CHECK( col0 == expected_col0 );
//    CHECK( col1 == expected_col1 );
//    CHECK( col2 == expected_col2 );
//  }
//}
#endif // C++>17
