#include "std_e/unit_test/doctest.hpp"
using std::vector;
using std::string;

// [Sphinx Doc] STD_E_ENUM_CLASS {
#include "std_e/utils/enum.hpp"

// Define enum class "my_enum_example_name" with more features
STD_E_ENUM_CLASS(my_enum_example_name,
  value_0,
  value_1
)
TEST_CASE("STD_E_ENUM_CLASS") {
  SUBCASE("enum_size") {
    static_assert(std_e::enum_size<my_enum_example_name> == 2 );
  }

  SUBCASE("to_string(enum)value)") {
    CHECK( to_string(my_enum_example_name::value_0) == "value_0" );
    CHECK( to_string(my_enum_example_name::value_1) == "value_1" );
  }
  SUBCASE("to_enum<enum_type>(value_as_string)") {
    CHECK( to_enum<my_enum_example_name>("value_0") == my_enum_example_name::value_0 );
    CHECK( to_enum<my_enum_example_name>("value_1") == my_enum_example_name::value_1 );
  }

  SUBCASE("enum_to_strings<enum_type>") {
    const vector<string>& strs = std_e::enum_to_strings<my_enum_example_name>;
    REQUIRE( strs.size() == 2 );
    CHECK( strs[0] == "value_0" );
    CHECK( strs[1] == "value_1" );
  }
}
// [Sphinx Doc] STD_E_ENUM_CLASS }




namespace testing_nspace {
  STD_E_ENUM_CLASS(my_enum_example_name,
    value_2,
    value_3,
    value_4
  )
}

TEST_CASE("STD_E_ENUM_CLASS within namespace enum_to_strings") {
  const vector<string>& strs = std_e::enum_to_strings<testing_nspace::my_enum_example_name>;
  REQUIRE( strs.size() == 3 );
  CHECK( strs[0] == "value_2" );
  CHECK( strs[1] == "value_3" );
  CHECK( strs[2] == "value_4" );
}
TEST_CASE("STD_E_ENUM_CLASS within namespace enum_size") {
  static_assert(std_e::enum_size<testing_nspace::my_enum_example_name> == 3 );
}
TEST_CASE("to_string(STD_E_ENUM_CLASS within namespace)") {
  CHECK( testing_nspace::to_string(testing_nspace::my_enum_example_name::value_2) == "value_2" );
  CHECK( testing_nspace::to_string(testing_nspace::my_enum_example_name::value_3) == "value_3" );
  CHECK( testing_nspace::to_string(testing_nspace::my_enum_example_name::value_4) == "value_4" );
}

TEST_CASE("to_enum<STD_E_ENUM_CLASS within namespace>()") {
  CHECK( std_e::to_enum<testing_nspace::my_enum_example_name>("value_2") == testing_nspace::my_enum_example_name::value_2 );
  CHECK( std_e::to_enum<testing_nspace::my_enum_example_name>("value_3") == testing_nspace::my_enum_example_name::value_3 );
  CHECK( std_e::to_enum<testing_nspace::my_enum_example_name>("value_4") == testing_nspace::my_enum_example_name::value_4 );
}
