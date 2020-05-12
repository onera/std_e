#include "std_e/unit_test/doctest.hpp"
#include "std_e/data_structure/table.hpp"

#include <string>

using std::vector;
using std::string;

TEST_CASE("table") {
  std_e::table<int,std::string,double> s;
  s.push_back(42,"X",3.14);
  s.push_back(43,"Y",2.7);
  s.push_back(0,"ABC",100.);

  SUBCASE("col") {
    std::vector<int>         expected_col0 = {42,43,0};
    std::vector<std::string> expected_col1 = {"X","Y","ABC"};
    std::vector<double>      expected_col2 = {3.14,2.7,100.};

    SUBCASE("by index") {
      std::vector<int>         col0 = std_e::col<0>(s);
      std::vector<std::string> col1 = std_e::col<1>(s);
      std::vector<double>      col2 = std_e::col<2>(s);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
    SUBCASE("by type") {
      std::vector<int>         col0 = std_e::col<int>(s);
      std::vector<std::string> col1 = std_e::col<std::string>(s);
      std::vector<double>      col2 = std_e::col<double>(s);
      CHECK( col0 == expected_col0 );
      CHECK( col1 == expected_col1 );
      CHECK( col2 == expected_col2 );
    }
  }

  SUBCASE("row") {
    SUBCASE("indexing") {
      std::tuple<int&,std::string&,double&> row0 = s[0];
      std::tuple<int&,std::string&,double&> row1 = s[1];
      std::tuple<int&,std::string&,double&> row2 = s[2];
      CHECK( std::get<0>(row0) == 42 ); CHECK( std::get<1>(row0) == "X"  ); CHECK( std::get<2>(row0) == 3.14 );
      CHECK( std::get<0>(row1) == 43 ); CHECK( std::get<1>(row1) == "Y"  ); CHECK( std::get<2>(row1) == 2.7  );
      CHECK( std::get<0>(row2) ==  0 ); CHECK( std::get<1>(row2) == "ABC"); CHECK( std::get<2>(row2) == 100. );
    }

    SUBCASE("find_row") {
      std::tuple<int&,std::string&,double&> row43 = std_e::find_row<0>(s,43);  // 0: position of type "int"
      std::tuple<int&,std::string&,double&> rowX  = std_e::find_row<1>(s,"X"); // 1: position of type "std::string"
      CHECK( std::get<0>(row43) == 43 ); CHECK( std::get<1>(row43) == "Y" );   CHECK( std::get<2>(row43) == 2.7  );
      CHECK( std::get<0>(rowX ) == 42 ); CHECK( std::get<1>(rowX ) == "X" );   CHECK( std::get<2>(rowX ) == 3.14 );
    }
    SUBCASE("find_cell") {
      double& cell_43_2 = std_e::find_cell<0,2>(s,43); // 0: search position of type "int", 2: found position of type "double"
      CHECK( cell_43_2 == 2.7 );

      const double& cell_43_2_0 = std_e::table_find<2>::from<0>(s,43);
      //const double& cell_43_2_0 = std_e::find<2>(std_e::from<0>)(s,43);
      CHECK( cell_43_2_0 == 2.7 );
    }
  }
}

TEST_CASE("table ctor with size") {
  std_e::table<int,std::string,double> s(2);
  std_e::cell<0>(s,0) = 42; std_e::cell<1>(s,0) = "X"; std_e::cell<2>(s,0) = 3.14;
  std_e::cell<0>(s,1) = 43; std_e::cell<1>(s,1) = "Y"; std_e::cell<2>(s,1) = 2.7;

  std::vector<int>         col0 = std_e::col<0>(s);
  std::vector<std::string> col1 = std_e::col<1>(s);
  std::vector<double>      col2 = std_e::col<2>(s);

  std::vector<int>         expected_col0 = {42,43};
  std::vector<std::string> expected_col1 = {"X","Y"};
  std::vector<double>      expected_col2 = {3.14,2.7};

  CHECK( col0 == expected_col0 );
  CHECK( col1 == expected_col1 );
  CHECK( col2 == expected_col2 );
}
