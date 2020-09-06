#include "std_e/unit_test/doctest.hpp"
#include "std_e/data_structure/multi_range.hpp"

#include <string>

// TODO RENAME cell->elt, row->multi_elt, col->range
using std::vector;
using std::string;

TEST_CASE("multi_vector") {
  std_e::multi_vector<int,std::string,double> t;
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
    SUBCASE("by type") {
      std::vector<int>&         col0 = std_e::range<int>(t);
      std::vector<std::string>& col1 = std_e::range<std::string>(t);
      std::vector<double>&      col2 = std_e::range<double>(t);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
  }

  SUBCASE("row") {
    SUBCASE("indexing") {
      std::tuple<int&,std::string&,double&> row0 = t[0];
      std::tuple<int&,std::string&,double&> row1 = t[1];
      std::tuple<int&,std::string&,double&> row2 = t[2];
      CHECK( std::get<0>(row0) == 42 ); CHECK( std::get<1>(row0) == "X"  ); CHECK( std::get<2>(row0) == 3.14 );
      CHECK( std::get<0>(row1) == 43 ); CHECK( std::get<1>(row1) == "Y"  ); CHECK( std::get<2>(row1) == 2.7  );
      CHECK( std::get<0>(row2) ==  0 ); CHECK( std::get<1>(row2) == "ABC"); CHECK( std::get<2>(row2) == 100. );
    }

    SUBCASE("find") {
      std::tuple<int&,std::string&,double&> row43 = std_e::find<0>(t,43);  // 0: position of type "int"
      std::tuple<int&,std::string&,double&> rowX  = std_e::find<1>(t,"X"); // 1: position of type "std::string"
      CHECK( std::get<0>(row43) == 43 ); CHECK( std::get<1>(row43) == "Y" );   CHECK( std::get<2>(row43) == 2.7  );
      CHECK( std::get<0>(rowX ) == 42 ); CHECK( std::get<1>(rowX ) == "X" );   CHECK( std::get<2>(rowX ) == 3.14 );
    }
    SUBCASE("find_element") {
      double& cell_43_2 = std_e::find_element<0,2>(t,43); // 0: search position of type "int", 2: found position of type "double"
      CHECK( cell_43_2 == 2.7 );
    }
  }
  SUBCASE("sort") {
    CHECK( t.sorted_range_index() == -1 ); // means invalid

    std_e::sort_by<0>(t);

    CHECK( t.sorted_range_index() == 0 );

    std::vector<int>         expected_col0 = {0,42,43};
    std::vector<std::string> expected_col1 = {"ABC","X","Y"};
    std::vector<double>      expected_col2 = {100.,3.14,2.7};

    std::vector<int>         col0 = std_e::range<0>(t);
    std::vector<std::string> col1 = std_e::range<1>(t);
    std::vector<double>      col2 = std_e::range<2>(t);
    CHECK( col0 == expected_col0 );
    CHECK( col1 == expected_col1 );
    CHECK( col2 == expected_col2 );
  }
}

TEST_CASE("multi_vector ctor with size") {
  std_e::multi_vector<int,std::string,double> t(2);
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
}

TEST_CASE("multi_vector with two columns") {
  std_e::multi_vector<int,std::string> t;
  t.push_back(42,"X");
  t.push_back(43,"Y");
  t.push_back(0,"ABC");

  std::tuple<int&,std::string&> row_42 = find(t,42);
  CHECK( std::get<0>(row_42) == 42 );
  CHECK( std::get<1>(row_42) == "X" );

  using namespace std::string_literals;
  std::tuple<int&,std::string&> row_Y = find(t,"Y"s); // char[] can't be used to select the find overload with std::string
  CHECK( std::get<0>(row_Y) == 43 );
  CHECK( std::get<1>(row_Y) == "Y" );

  std::string& associated_42 = find_associate(t,42);
  CHECK( associated_42 == "X" );

  int& associated_Y = find_associate(t,"Y"s);
  CHECK( associated_Y == 43 );
}

TEST_CASE("multi_vector ctor from vectors") {
  std::vector<int>         x = {42,43,0};
  std::vector<std::string> y = {"X","Y","ABC"};
  std_e::multi_vector<int,std::string> t(x,y);
  //std_e::multi_vector<int,std::string> t(std::vector<int>{42,43,0},std::vector<std::string>{"X","Y","ABC"});

  std::vector<int>         expected_col0 = {42,43,0};
  std::vector<std::string> expected_col1 = {"X","Y","ABC"};

  std::vector<int>         col0 = std_e::range<0>(t);
  std::vector<std::string> col1 = std_e::range<1>(t);

  CHECK( col0 == expected_col0 );
  CHECK( col1 == expected_col1 );
}

TEST_CASE("multi_span") {
  std::vector<int> vi = {42,43,0};
  std::vector<std::string> vs = {"X","Y","ABC"};
  std::vector<double> vd = {3.14,2.7,100.};
  std_e::multi_span<int,std::string,double> t(std_e::make_span(vi),std_e::make_span(vs),std_e::make_span(vd));

  SUBCASE("range") {
    std::vector<int>         expected_col0 = {42,43,0};
    std::vector<std::string> expected_col1 = {"X","Y","ABC"};
    std::vector<double>      expected_col2 = {3.14,2.7,100.};

    SUBCASE("by index") {
      std_e::span<int>&         col0 = std_e::range<0>(t);
      std_e::span<std::string>& col1 = std_e::range<1>(t);
      std_e::span<double>&      col2 = std_e::range<2>(t);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
    SUBCASE("by type") {
      std_e::span<int>&         col0 = std_e::range<int>(t);
      std_e::span<std::string>& col1 = std_e::range<std::string>(t);
      std_e::span<double>&      col2 = std_e::range<double>(t);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
    SUBCASE("is view") {
      auto col0 = std_e::range<int>(t);
      col0[0] = 50;
      CHECK( vi[0] == 50 );
    }
  }
}
TEST_CASE("make_multi_span") {
  std_e::multi_vector<int,std::string,double> mv;
  mv.push_back(42,"X",3.14);
  mv.push_back(43,"Y",2.7);
  mv.push_back(0,"ABC",100.);

  std::vector<int>         expected_col0 = {42,43,0};
  std::vector<std::string> expected_col1 = {"X","Y","ABC"};
  std::vector<double>      expected_col2 = {3.14,2.7,100.};

  SUBCASE("non-const") {
    std_e::multi_span<int,std::string,double> ms = make_multi_span(mv);

    std_e::span<int>&         col0 = std_e::range<0>(ms);
    std_e::span<std::string>& col1 = std_e::range<1>(ms);
    std_e::span<double>&      col2 = std_e::range<2>(ms);
    CHECK( col0 == expected_col0 );
    CHECK( col1 == expected_col1 );
    CHECK( col2 == expected_col2 );
  }
  SUBCASE("const") {
    const auto& cmv = mv;
    std_e::multi_span<const int,const std::string,const double> ms = make_multi_span(cmv);

    std_e::span<const int>&         col0 = std_e::range<0>(ms);
    std_e::span<const std::string>& col1 = std_e::range<1>(ms);
    std_e::span<const double>&      col2 = std_e::range<2>(ms);
    CHECK( col0 == expected_col0 );
    CHECK( col1 == expected_col1 );
    CHECK( col2 == expected_col2 );
  }
}
