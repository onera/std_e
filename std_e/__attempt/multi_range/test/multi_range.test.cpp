#include "std_e/unit_test/doctest.hpp"
#include "std_e/__attempt/multi_range/multi_range.hpp"

#include <string>

// TODO RENAME cell->elt, row->multi_elt, col->range
using std::vector;
using std::string;

TEST_CASE("std_e::old::multi_vector") {
  std_e::old::multi_vector<int,string,double> t;
  t.push_back(42,"X",3.14);
  t.push_back(43,"Y",2.7);
  t.push_back(0,"ABC",100.);

  SUBCASE("range") {
    vector<int>    expected_col0 = {42,43,0};
    vector<string> expected_col1 = {"X","Y","ABC"};
    vector<double> expected_col2 = {3.14,2.7,100.};

    SUBCASE("by index") {
      vector<int>&    col0 = std_e::old::range<0>(t);
      vector<string>& col1 = std_e::old::range<1>(t);
      vector<double>& col2 = std_e::old::range<2>(t);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
    SUBCASE("by type") {
      vector<int>&    col0 = std_e::old::range<int>(t);
      vector<string>& col1 = std_e::old::range<string>(t);
      vector<double>& col2 = std_e::old::range<double>(t);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
  }

  SUBCASE("row") {
    SUBCASE("indexing") {
      std::tuple<int&,string&,double&> row0 = t[0];
      std::tuple<int&,string&,double&> row1 = t[1];
      std::tuple<int&,string&,double&> row2 = t[2];
      CHECK( std::get<0>(row0) == 42 ); CHECK( std::get<1>(row0) == "X"  ); CHECK( std::get<2>(row0) == 3.14 );
      CHECK( std::get<0>(row1) == 43 ); CHECK( std::get<1>(row1) == "Y"  ); CHECK( std::get<2>(row1) == 2.7  );
      CHECK( std::get<0>(row2) ==  0 ); CHECK( std::get<1>(row2) == "ABC"); CHECK( std::get<2>(row2) == 100. );
    }

    SUBCASE("find") {
      std::tuple<int&,string&,double&> row43 = std_e::old::find<0>(t,43);  // 0: position of type "int"
      std::tuple<int&,string&,double&> rowX  = std_e::old::find<1>(t,"X"); // 1: position of type "string"
      CHECK( std::get<0>(row43) == 43 ); CHECK( std::get<1>(row43) == "Y" );   CHECK( std::get<2>(row43) == 2.7  );
      CHECK( std::get<0>(rowX ) == 42 ); CHECK( std::get<1>(rowX ) == "X" );   CHECK( std::get<2>(rowX ) == 3.14 );
    }
    SUBCASE("find_element") {
      double& cell_43_2 = std_e::old::find_element<0,2>(t,43); // 0: search position of type "int", 2: found position of type "double"
      CHECK( cell_43_2 == 2.7 );
    }
  }
  SUBCASE("sort") {
    CHECK( t.sorted_range_index() == -1 ); // means invalid

    std_e::old::sort_by<0>(t);

    CHECK( t.sorted_range_index() == 0 );

    vector<int>    expected_col0 = {0,42,43};
    vector<string> expected_col1 = {"ABC","X","Y"};
    vector<double> expected_col2 = {100.,3.14,2.7};

    vector<int>    col0 = std_e::old::range<0>(t);
    vector<string> col1 = std_e::old::range<1>(t);
    vector<double> col2 = std_e::old::range<2>(t);
    CHECK( col0 == expected_col0 );
    CHECK( col1 == expected_col1 );
    CHECK( col2 == expected_col2 );
  }
}

TEST_CASE("std_e::old::multi_vector ctor with size") {
  std_e::old::multi_vector<int,string,double> t(2);
  std_e::old::element<0>(t,0) = 42; std_e::old::element<1>(t,0) = "X"; std_e::old::element<2>(t,0) = 3.14;
  std_e::old::element<0>(t,1) = 43; std_e::old::element<1>(t,1) = "Y"; std_e::old::element<2>(t,1) = 2.7;

  vector<int>    col0 = std_e::old::range<0>(t);
  vector<string> col1 = std_e::old::range<1>(t);
  vector<double> col2 = std_e::old::range<2>(t);

  vector<int>    expected_col0 = {42,43};
  vector<string> expected_col1 = {"X","Y"};
  vector<double> expected_col2 = {3.14,2.7};

  CHECK( col0 == expected_col0 );
  CHECK( col1 == expected_col1 );
  CHECK( col2 == expected_col2 );
}

TEST_CASE("std_e::old::multi_vector with two columns") {
  std_e::old::multi_vector<int,string> t;
  t.push_back(42,"X");
  t.push_back(43,"Y");
  t.push_back(0,"ABC");

  std::tuple<int&,string&> row_42 = find(t,42);
  CHECK( std::get<0>(row_42) == 42 );
  CHECK( std::get<1>(row_42) == "X" );

  using namespace std::string_literals;
  std::tuple<int&,string&> row_Y = find(t,"Y"s); // char[] can't be used to select the find overload with string
  CHECK( std::get<0>(row_Y) == 43 );
  CHECK( std::get<1>(row_Y) == "Y" );

  string& associated_42 = find_associate(t,42);
  CHECK( associated_42 == "X" );

  int& associated_Y = find_associate(t,"Y"s);
  CHECK( associated_Y == 43 );
}

TEST_CASE("std_e::old::multi_vector ctor from vectors") {
  vector<int>    x = {42,43,0};
  vector<string> y = {"X","Y","ABC"};
  std_e::old::multi_vector<int,string> t(x,y);
  //std_e::old::multi_vector<int,string> t(vector<int>{42,43,0},vector<string>{"X","Y","ABC"});

  vector<int>    expected_col0 = {42,43,0};
  vector<string> expected_col1 = {"X","Y","ABC"};

  vector<int>    col0 = std_e::old::range<0>(t);
  vector<string> col1 = std_e::old::range<1>(t);

  CHECK( col0 == expected_col0 );
  CHECK( col1 == expected_col1 );
}

TEST_CASE("std_e::old::multi_span") {
  vector<int> vi = {42,43,0};
  vector<string> vs = {"X","Y","ABC"};
  vector<double> vd = {3.14,2.7,100.};
  std_e::old::multi_span<int,string,double> t(std_e::make_span(vi),std_e::make_span(vs),std_e::make_span(vd));

  SUBCASE("range") {
    vector<int>    expected_col0 = {42,43,0};
    vector<string> expected_col1 = {"X","Y","ABC"};
    vector<double> expected_col2 = {3.14,2.7,100.};

    SUBCASE("by index") {
      std_e::span<int>&    col0 = std_e::old::range<0>(t);
      std_e::span<string>& col1 = std_e::old::range<1>(t);
      std_e::span<double>& col2 = std_e::old::range<2>(t);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
    SUBCASE("by type") {
      std_e::span<int>&    col0 = std_e::old::range<int>(t);
      std_e::span<string>& col1 = std_e::old::range<string>(t);
      std_e::span<double>& col2 = std_e::old::range<double>(t);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
    SUBCASE("is view") {
      auto col0 = std_e::old::range<int>(t);
      col0[0] = 50;
      CHECK( vi[0] == 50 );
    }
  }
}
TEST_CASE("make_span") {
  std_e::old::multi_vector<int,string,double> mv;
  mv.push_back(42,"X",3.14);
  mv.push_back(43,"Y",2.7);
  mv.push_back(0,"ABC",100.);

  SUBCASE("complete view") {
    vector<int>    expected_col0 = {42,43,0};
    vector<string> expected_col1 = {"X","Y","ABC"};
    vector<double> expected_col2 = {3.14,2.7,100.};

    SUBCASE("non-const") {
      std_e::old::multi_span<int,string,double> ms = make_span(mv);

      std_e::span<int>&    col0 = std_e::old::range<0>(ms);
      std_e::span<string>& col1 = std_e::old::range<1>(ms);
      std_e::span<double>& col2 = std_e::old::range<2>(ms);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
    SUBCASE("const") {
      const auto& cmv = mv;
      std_e::old::multi_span<const int,const string,const double> ms = make_span(cmv);

      std_e::span<const int>&    col0 = std_e::old::range<0>(ms);
      std_e::span<const string>& col1 = std_e::old::range<1>(ms);
      std_e::span<const double>& col2 = std_e::old::range<2>(ms);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
  }
  SUBCASE("partial view") {
    vector<int>    expected_col0 = {43,0};
    vector<string> expected_col1 = {"Y","ABC"};
    vector<double> expected_col2 = {2.7,100.};

    std_e::old::multi_span<int,string,double> ms = make_span_n(mv,1,2);

    std_e::span<int>&    col0 = std_e::old::range<0>(ms);
    std_e::span<string>& col1 = std_e::old::range<1>(ms);
    std_e::span<double>& col2 = std_e::old::range<2>(ms);
    CHECK( col0 == expected_col0 );
    CHECK( col1 == expected_col1 );
    CHECK( col2 == expected_col2 );
  }
}
