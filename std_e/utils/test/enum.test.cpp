#include "std_e/unit_test/doctest.hpp"
using std::vector;
using std::string;
#include <iostream> // TODO

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

  SUBCASE("to_int") {
    CHECK( std_e::to_int(my_enum_example_name::value_0) == 0 );
    CHECK( std_e::to_int(my_enum_example_name::value_1) == 1 );
  }

  SUBCASE("to_string(enum)value)") {
    CHECK( to_string(my_enum_example_name::value_0) == "value_0" );
    CHECK( to_string(my_enum_example_name::value_1) == "value_1" );
  }
  SUBCASE("to_enum<enum_type>(value_as_string)") {
    CHECK( std_e::to_enum<my_enum_example_name>("value_0") == my_enum_example_name::value_0 );
    CHECK( std_e::to_enum<my_enum_example_name>("value_1") == my_enum_example_name::value_1 );
  }

  SUBCASE("enum_to_strings<enum_type>") {
    const vector<string>& strs = std_e::enum_to_strings<my_enum_example_name>;
    REQUIRE( strs.size() == 2 );
    CHECK( strs[0] == "value_0" );
    CHECK( strs[1] == "value_1" );
  }
}
// [Sphinx Doc] STD_E_ENUM_CLASS }

#define xxx(...) #__VA_ARGS__

// [Sphinx Doc] STD_E_ENUM_CLASS_WITH_VALUES {
// Define enum class "my_enum_example_name" with more features
STD_E_ENUM_CLASS(my_e_val_ex_name,
  value_0=200,
  value_1,
  value_2=0,
  value_3=30,
  value_4,
  value_5=10
)
TEST_CASE("STD_E_ENUM_CLASS_WITH_VALUES") {
  SUBCASE("enum_size") {
    static_assert(std_e::enum_size<my_e_val_ex_name> == 6 );
  }

  SUBCASE("to_int") {
    CHECK( std_e::to_int(my_e_val_ex_name::value_0) == 200 );
    CHECK( std_e::to_int(my_e_val_ex_name::value_1) == 201 );
    CHECK( std_e::to_int(my_e_val_ex_name::value_2) ==   0 );
    CHECK( std_e::to_int(my_e_val_ex_name::value_3) ==  30 );
    CHECK( std_e::to_int(my_e_val_ex_name::value_4) ==  31 );
    CHECK( std_e::to_int(my_e_val_ex_name::value_5) ==  10 );
  }

  SUBCASE("to_string(enum)value)") {
    CHECK( to_string(my_e_val_ex_name::value_0) == "value_0" );
    CHECK( to_string(my_e_val_ex_name::value_1) == "value_1" );
    CHECK( to_string(my_e_val_ex_name::value_2) == "value_2" );
    CHECK( to_string(my_e_val_ex_name::value_3) == "value_3" );
    CHECK( to_string(my_e_val_ex_name::value_4) == "value_4" );
    CHECK( to_string(my_e_val_ex_name::value_5) == "value_5" );
  }
  SUBCASE("to_enum<enum_type>(value_as_string)") {
    CHECK( std_e::to_enum<my_e_val_ex_name>("value_0") == my_e_val_ex_name::value_0 );
    CHECK( std_e::to_enum<my_e_val_ex_name>("value_1") == my_e_val_ex_name::value_1 );
    CHECK( std_e::to_enum<my_e_val_ex_name>("value_2") == my_e_val_ex_name::value_2 );
    CHECK( std_e::to_enum<my_e_val_ex_name>("value_3") == my_e_val_ex_name::value_3 );
    CHECK( std_e::to_enum<my_e_val_ex_name>("value_4") == my_e_val_ex_name::value_4 );
    CHECK( std_e::to_enum<my_e_val_ex_name>("value_5") == my_e_val_ex_name::value_5 );
  }

  SUBCASE("enum_to_strings<enum_type>") {
    const vector<string>& strs = std_e::enum_to_strings<my_e_val_ex_name>;
    REQUIRE( strs.size() == 6 );
    CHECK( strs[0] == "value_0" );
    CHECK( strs[1] == "value_1" );
    CHECK( strs[2] == "value_2" );
    CHECK( strs[3] == "value_3" );
    CHECK( strs[4] == "value_4" );
    CHECK( strs[5] == "value_5" );
  }
}
// [Sphinx Doc] STD_E_ENUM_CLASS_WITH_VALUES }




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
