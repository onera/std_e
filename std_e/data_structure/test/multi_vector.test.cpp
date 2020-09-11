#include "std_e/unit_test/doctest.hpp"
#include "std_e/data_structure/multi_vector.hpp"

#include <string>

STD_E_MULTI_VECTOR(my_test_mvector,
  int, ids,
  std::string, names,
  double, values
)
TEST_CASE("STD_E_MULTI_VECTOR") {
  my_test_mvector t;
  t.push_back(42,"X",3.14);
  t.push_back(43,"Y",2.7);
  t.push_back(0,"ABC",100.);

  CHECK( ids(t) == std::vector<int>{42,43,0} );
  CHECK( names(t) == std::vector<std::string>{"X","Y","ABC"} );
  CHECK( values(t) == std::vector<double>{3.14,2.7,100.} );
}
