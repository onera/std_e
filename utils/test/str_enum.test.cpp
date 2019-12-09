#include "doctest/doctest.h"
#include "std_e/utils/str_enum.hpp"
using std::vector;
using std::string;

STR_ENUM(enum_for_testing,
  value_0,
  value_1
)

TEST_CASE("STR_ENUM enum_to_strings") {
  const vector<string>& strs = std_e::enum_to_strings<enum_for_testing>;
  REQUIRE( strs.size() == 2 );
  CHECK( strs[0] == "value_0" );
  CHECK( strs[1] == "value_1" );
}
TEST_CASE("STR_ENUM enum_size") {
  REQUIRE( std_e::enum_size<enum_for_testing> == 2 );
}

TEST_CASE("to_string(STR_ENUM)") {
  CHECK( std_e::to_string(enum_for_testing::value_0) == "value_0" );
  CHECK( std_e::to_string(enum_for_testing::value_1) == "value_1" );
}




// BAD: (won't compile because of no namespace support by macros)
// namespace testing_nspace {
//   STR_ENUM(enum_for_testing,
//     value_2,
//     value_3
//   )
// }
// GOOD:
STR_ENUM_NSPACE(testing_nspace, enum_for_testing,
  value_2,
  value_3
)
TEST_CASE("STR_ENUM_NSPACE enum_to_strings") {
  const vector<string>& strs = std_e::enum_to_strings<testing_nspace::enum_for_testing>;
  REQUIRE( strs.size() == 2 );
  CHECK( strs[0] == "value_2" );
  CHECK( strs[1] == "value_3" );
}
