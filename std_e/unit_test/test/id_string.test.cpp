#include "std_e/unit_test/doctest.hpp"
#include "std_e/unit_test/id_string.hpp"

TEST_CASE("to_string") {
  std_e::id_string x = {10,"abc"};

  CHECK( to_string(x) == "{10,abc}" );
}
