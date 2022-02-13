#include "std_e/unit_test/doctest.hpp"
#include "std_e/data_structure/multi_range2.hpp"

#include <string>

// TODO RENAME cell->elt, row->multi_elt, col->range
using std::vector;
using std::string;

TEST_CASE("multi_vector2") {
  std_e::multi_vector2<int,std::string,double> t;
  t.push_back(42,"X",3.14);
  t.push_back(43,"Y",2.7);
  t.push_back(0,"ABC",100.);

  SUBCASE("range") {
    std::vector<int>         expected_col0 = {42,43,0};
    std::vector<std::string> expected_col1 = {"X","Y","ABC"};
    std::vector<double>      expected_col2 = {3.14,2.7,100.};

    SUBCASE("by index") {
      std::vector<int>&         col0 = std_e::range<0>(t);
      std::vector<std::string>& col1 = std_e::range<1>(t);
      std::vector<double>&      col2 = std_e::range<2>(t);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
    //SUBCASE("by type") {
    //  std::vector<int>&         col0 = std_e::range<int>(t);
    //  std::vector<std::string>& col1 = std_e::range<std::string>(t);
    //  std::vector<double>&      col2 = std_e::range<double>(t);
    //  CHECK( col0 == expected_col0 );
    //  CHECK( col1 == expected_col1 );
    //  CHECK( col2 == expected_col2 );
    //}
  }

  SUBCASE("indexing") {
    std::tuple<int&,std::string&,double&> row0 = t[0];
    std::tuple<int&,std::string&,double&> row1 = t[1];
    std::tuple<int&,std::string&,double&> row2 = t[2];
    CHECK( std::get<0>(row0) == 42 ); CHECK( std::get<1>(row0) == "X"  ); CHECK( std::get<2>(row0) == 3.14 );
    CHECK( std::get<0>(row1) == 43 ); CHECK( std::get<1>(row1) == "Y"  ); CHECK( std::get<2>(row1) == 2.7  );
    CHECK( std::get<0>(row2) ==  0 ); CHECK( std::get<1>(row2) == "ABC"); CHECK( std::get<2>(row2) == 100. );
  }
}

TEST_CASE("multi_vector2 ctor with size") {
  std_e::multi_vector2<int,std::string,double> t(2);
  std_e::element<0>(t,0) = 42; std_e::element<1>(t,0) = "X"; std_e::element<2>(t,0) = 3.14;
  std_e::element<0>(t,1) = 43; std_e::element<1>(t,1) = "Y"; std_e::element<2>(t,1) = 2.7;

  std::vector<int>         col0 = std_e::range<0>(t);
  std::vector<std::string> col1 = std_e::range<1>(t);
  std::vector<double>      col2 = std_e::range<2>(t);

  std::vector<int>         expected_col0 = {42,43};
  std::vector<std::string> expected_col1 = {"X","Y"};
  std::vector<double>      expected_col2 = {3.14,2.7};

  CHECK( col0 == expected_col0 );
  CHECK( col1 == expected_col1 );
  CHECK( col2 == expected_col2 );

  SUBCASE("algorithms") {
    SUBCASE("find") {
      auto it = std::find(t.begin(),t.end(),std::tuple{43,std::string("Y"),2.7});
      CHECK( it == t.begin()+1 );
    }
    SUBCASE("find_if") {
      auto first_eq_to_43 = [](const std::tuple<int&,std::string&,double>& x){ return std::get<0>(x)==43; };
      auto it = std::find_if(t.begin(),t.end(),first_eq_to_43);
      CHECK( it == t.begin()+1 );

      auto first_eq_to_100 = [](const std::tuple<int&,std::string&,double>& x){ return std::get<0>(x)==100; };
      auto it_not_found = std::find_if(t.begin(),t.end(),first_eq_to_100);
      CHECK( it_not_found == t.end() );
    }
    SUBCASE("sort") {
      //using _Tp = std::tuple<int&>;
      //int i;
      //_Tp __t (i);
      //const std::tuple<int&> rc(i);
      //rc = std::forward<_Tp>(__t);

      //const_cast<const std::iter_reference_t<_Out>&&>(*__o) = std::forward<_Tp>(__t);
      //const_cast<const std::tuple<int&>&&>(r) = std::forward<_Tp>(__t);

      //static_assert(std::indirectly_writable<_Out,_Tp>);
      ////std::sort(t.begin(),t.end());
      ////
      //std_e::multi_vector2<int> t;
      //t.push_back(42);
      //t.push_back(43);
      //t.push_back(0);

      //std::ranges::sort(t.begin(),t.end());
      //// check
      //const std::vector<int>         expected_col0 = {42,43,0};
      //const std::vector<std::string> expected_col1 = {"X","Y","ABC"};
      //const std::vector<double>      expected_col2 = {3.14,2.7,100.};
      //std::vector<int>&         col0 = std_e::range<0>(t);
      //std::vector<std::string>& col1 = std_e::range<1>(t);
      //std::vector<double>&      col2 = std_e::range<2>(t);
      //CHECK( col0 == expected_col0 );
      //CHECK( col1 == expected_col1 );
      //CHECK( col2 == expected_col2 );
    }
  }
}


TEST_CASE("multi_vector2 ctor from vectors") {
  std::vector<int>         x = {42,43,0};
  std::vector<std::string> y = {"X","Y","ABC"};
  std_e::multi_vector2<int,std::string> t(x,y);
  //std_e::multi_vector2<int,std::string> t(std::vector<int>{42,43,0},std::vector<std::string>{"X","Y","ABC"});

  std::vector<int>         expected_col0 = {42,43,0};
  std::vector<std::string> expected_col1 = {"X","Y","ABC"};

  std::vector<int>         col0 = std_e::range<0>(t);
  std::vector<std::string> col1 = std_e::range<1>(t);

  CHECK( col0 == expected_col0 );
  CHECK( col1 == expected_col1 );
}


//TEST_CASE("multi_span") {
//  std::vector<int> vi = {42,43,0};
//  std::vector<std::string> vs = {"X","Y","ABC"};
//  std::vector<double> vd = {3.14,2.7,100.};
//  std_e::multi_span<int,std::string,double> t(std_e::make_span(vi),std_e::make_span(vs),std_e::make_span(vd));
//
//  SUBCASE("range") {
//    std::vector<int>         expected_col0 = {42,43,0};
//    std::vector<std::string> expected_col1 = {"X","Y","ABC"};
//    std::vector<double>      expected_col2 = {3.14,2.7,100.};
//
//    SUBCASE("by index") {
//      std_e::span<int>&         col0 = std_e::range<0>(t);
//      std_e::span<std::string>& col1 = std_e::range<1>(t);
//      std_e::span<double>&      col2 = std_e::range<2>(t);
//      CHECK( col0 == expected_col0 );
//      CHECK( col1 == expected_col1 );
//      CHECK( col2 == expected_col2 );
//    }
//    SUBCASE("by type") {
//      std_e::span<int>&         col0 = std_e::range<int>(t);
//      std_e::span<std::string>& col1 = std_e::range<std::string>(t);
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
//  std_e::multi_vector2<int,std::string,double> mv;
//  mv.push_back(42,"X",3.14);
//  mv.push_back(43,"Y",2.7);
//  mv.push_back(0,"ABC",100.);
//
//  SUBCASE("complete view") {
//    std::vector<int>         expected_col0 = {42,43,0};
//    std::vector<std::string> expected_col1 = {"X","Y","ABC"};
//    std::vector<double>      expected_col2 = {3.14,2.7,100.};
//
//    SUBCASE("non-const") {
//      std_e::multi_span<int,std::string,double> ms = make_span(mv);
//
//      std_e::span<int>&         col0 = std_e::range<0>(ms);
//      std_e::span<std::string>& col1 = std_e::range<1>(ms);
//      std_e::span<double>&      col2 = std_e::range<2>(ms);
//      CHECK( col0 == expected_col0 );
//      CHECK( col1 == expected_col1 );
//      CHECK( col2 == expected_col2 );
//    }
//    SUBCASE("const") {
//      const auto& cmv = mv;
//      std_e::multi_span<const int,const std::string,const double> ms = make_span(cmv);
//
//      std_e::span<const int>&         col0 = std_e::range<0>(ms);
//      std_e::span<const std::string>& col1 = std_e::range<1>(ms);
//      std_e::span<const double>&      col2 = std_e::range<2>(ms);
//      CHECK( col0 == expected_col0 );
//      CHECK( col1 == expected_col1 );
//      CHECK( col2 == expected_col2 );
//    }
//  }
//  SUBCASE("partial view") {
//    std::vector<int>         expected_col0 = {43,0};
//    std::vector<std::string> expected_col1 = {"Y","ABC"};
//    std::vector<double>      expected_col2 = {2.7,100.};
//
//    std_e::multi_span<int,std::string,double> ms = make_span_n(mv,1,2);
//
//    std_e::span<int>&         col0 = std_e::range<0>(ms);
//    std_e::span<std::string>& col1 = std_e::range<1>(ms);
//    std_e::span<double>&      col2 = std_e::range<2>(ms);
//    CHECK( col0 == expected_col0 );
//    CHECK( col1 == expected_col1 );
//    CHECK( col2 == expected_col2 );
//  }
//}
