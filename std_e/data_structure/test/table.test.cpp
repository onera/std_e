#include "std_e/unit_test/doctest.hpp"
#include "std_e/data_structure/table.hpp"

#include <string>

using std::vector;
using std::string;

TEST_CASE("table") {
  std_e::table<int,std::string,double> t;
  t.push_back(42,"X",3.14);
  t.push_back(43,"Y",2.7);
  t.push_back(0,"ABC",100.);

  SUBCASE("col") {
    std::vector<int>         expected_col0 = {42,43,0};
    std::vector<std::string> expected_col1 = {"X","Y","ABC"};
    std::vector<double>      expected_col2 = {3.14,2.7,100.};

    SUBCASE("by index") {
      std::vector<int>         col0 = std_e::col<0>(t);
      std::vector<std::string> col1 = std_e::col<1>(t);
      std::vector<double>      col2 = std_e::col<2>(t);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
    SUBCASE("by type") {
      std::vector<int>         col0 = std_e::col<int>(t);
      std::vector<std::string> col1 = std_e::col<std::string>(t);
      std::vector<double>      col2 = std_e::col<double>(t);
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

    SUBCASE("find_row") {
      std::tuple<int&,std::string&,double&> row43 = std_e::find_row<0>(t,43);  // 0: position of type "int"
      std::tuple<int&,std::string&,double&> rowX  = std_e::find_row<1>(t,"X"); // 1: position of type "std::string"
      CHECK( std::get<0>(row43) == 43 ); CHECK( std::get<1>(row43) == "Y" );   CHECK( std::get<2>(row43) == 2.7  );
      CHECK( std::get<0>(rowX ) == 42 ); CHECK( std::get<1>(rowX ) == "X" );   CHECK( std::get<2>(rowX ) == 3.14 );
    }
    SUBCASE("find_cell") {
      double& cell_43_2 = std_e::find_cell<0,2>(t,43); // 0: search position of type "int", 2: found position of type "double"
      CHECK( cell_43_2 == 2.7 );
    }
  }
  SUBCASE("sort") {
    CHECK( t.sorted_column_index() == -1 ); // means invalid

    std_e::sort_by_col<0>(t);

    CHECK( t.sorted_column_index() == 0 );

    std::vector<int>         expected_col0 = {0,42,43};
    std::vector<std::string> expected_col1 = {"ABC","X","Y"};
    std::vector<double>      expected_col2 = {100.,3.14,2.7};

    std::vector<int>         col0 = std_e::col<0>(t);
    std::vector<std::string> col1 = std_e::col<1>(t);
    std::vector<double>      col2 = std_e::col<2>(t);
    CHECK( col0 == expected_col0 );
    CHECK( col1 == expected_col1 );
    CHECK( col2 == expected_col2 );
  }
}

TEST_CASE("table ctor with size") {
  std_e::table<int,std::string,double> t(2);
  std_e::cell<0>(t,0) = 42; std_e::cell<1>(t,0) = "X"; std_e::cell<2>(t,0) = 3.14;
  std_e::cell<0>(t,1) = 43; std_e::cell<1>(t,1) = "Y"; std_e::cell<2>(t,1) = 2.7;

  std::vector<int>         col0 = std_e::col<0>(t);
  std::vector<std::string> col1 = std_e::col<1>(t);
  std::vector<double>      col2 = std_e::col<2>(t);

  std::vector<int>         expected_col0 = {42,43};
  std::vector<std::string> expected_col1 = {"X","Y"};
  std::vector<double>      expected_col2 = {3.14,2.7};

  CHECK( col0 == expected_col0 );
  CHECK( col1 == expected_col1 );
  CHECK( col2 == expected_col2 );
}

TEST_CASE("table with two columns") {
  std_e::table<int,std::string> t;
  t.push_back(42,"X");
  t.push_back(43,"Y");
  t.push_back(0,"ABC");

  std::tuple<int&,std::string&> row_42 = find_row(t,42);
  CHECK( std::get<0>(row_42) == 42 );
  CHECK( std::get<1>(row_42) == "X" );

  using namespace std::string_literals;
  std::tuple<int&,std::string&> row_Y = find_row(t,"Y"s); // char[] can't be used to select the find_row overload with std::string
  CHECK( std::get<0>(row_Y) == 43 );
  CHECK( std::get<1>(row_Y) == "Y" );

  std::string& associated_42 = find_associate(t,42);
  CHECK( associated_42 == "X" );

  int& associated_Y = find_associate(t,"Y"s);
  CHECK( associated_Y == 43 );
}

STD_E_TABLE(my_test_table,
  int, id,
  std::string, name,
  double, value
)
TEST_CASE("STD_E_TABLE") {
  my_test_table t;
  t.push_back(42,"X",3.14);
  t.push_back(43,"Y",2.7);
  t.push_back(0,"ABC",100.);

  std::vector<int>& col0 = ids(t);
  CHECK( col0[0] == 42 );
  CHECK( col0[1] == 43 );
  CHECK( col0[2] ==  0 );

  auto row1 = find_row_from_id(t,43);
  CHECK( std::get<0>(row1) == 43 );
  CHECK( std::get<1>(row1) == "Y" );
  CHECK( std::get<2>(row1) == 2.7 );

  const double& cell_43_2 = find_value_from_id(t,43);
  CHECK( cell_43_2 == 2.7 );
}
