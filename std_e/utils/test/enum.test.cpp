#include "std_e/unit_test/doctest.hpp"
#include "std_e/utils/enum.hpp"
#include <iostream>
using std::vector;
using std::string;
#include "std_e/utils/vector.hpp"

STD_E_ENUM_CLASS(enum_for_testing,
  value_0,
  value_1
)

TEST_CASE("STD_E_ENUM_CLASS enum_to_strings") {
  const vector<string>& strs = std_e::enum_to_strings<enum_for_testing>;
  REQUIRE( strs.size() == 2 );
  CHECK( strs[0] == "value_0" );
  CHECK( strs[1] == "value_1" );
}
TEST_CASE("STD_E_ENUM_CLASS enum_size") {
  static_assert(std_e::enum_size<enum_for_testing> == 2 );
}

TEST_CASE("to_string(STD_E_ENUM_CLASS)") {
  CHECK( to_string(enum_for_testing::value_0) == "value_0" );
  CHECK( to_string(enum_for_testing::value_1) == "value_1" );
}
TEST_CASE("to_enum<STD_E_ENUM_CLASS>()") {
  CHECK( to_enum<enum_for_testing>("value_0") == enum_for_testing::value_0 );
  CHECK( to_enum<enum_for_testing>("value_1") == enum_for_testing::value_1 );
}




namespace testing_nspace {
  STD_E_ENUM_CLASS(enum_for_testing,
    value_2,
    value_3,
    value_4
  )
}

TEST_CASE("STD_E_ENUM_CLASS within namespace enum_to_strings") {
  const vector<string>& strs = std_e::enum_to_strings<testing_nspace::enum_for_testing>;
  REQUIRE( strs.size() == 3 );
  CHECK( strs[0] == "value_2" );
  CHECK( strs[1] == "value_3" );
  CHECK( strs[2] == "value_4" );
}
TEST_CASE("STD_E_ENUM_CLASS within namespace enum_size") {
  static_assert(std_e::enum_size<testing_nspace::enum_for_testing> == 3 );
}
TEST_CASE("to_string(STD_E_ENUM_CLASS within namespace)") {
  CHECK( testing_nspace::to_string(testing_nspace::enum_for_testing::value_2) == "value_2" );
  CHECK( testing_nspace::to_string(testing_nspace::enum_for_testing::value_3) == "value_3" );
  CHECK( testing_nspace::to_string(testing_nspace::enum_for_testing::value_4) == "value_4" );
}

TEST_CASE("to_enum<STD_E_ENUM_CLASS within namespace>()") {
  CHECK( std_e::to_enum<testing_nspace::enum_for_testing>("value_2") == testing_nspace::enum_for_testing::value_2 );
  CHECK( std_e::to_enum<testing_nspace::enum_for_testing>("value_3") == testing_nspace::enum_for_testing::value_3 );
  CHECK( std_e::to_enum<testing_nspace::enum_for_testing>("value_4") == testing_nspace::enum_for_testing::value_4 );
}
