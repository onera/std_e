#include "std_e/unit_test/doctest.hpp"
#include "std_e/data_structure/table.hpp"

#include <string>

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
