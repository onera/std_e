#include "std_e/unit_test/doctest.hpp"

#include "std_e/utils/to_string.hpp"
#include <vector>

using namespace std_e;


TEST_CASE("to_string(std::string)") {
  std::string s = "abc";
  CHECK( std_e::to_string(s) == "abc" );
}

TEST_CASE("to_string(std::string_view)") {
  std::string s = "abc";
  std::string_view sv = s;
  CHECK( std_e::to_string(sv) == "abc" );
}

TEST_CASE("to_string(ptr)") {
  int* ptr = nullptr;
  CHECK( std_e::to_string(ptr) == "0" );
}

TEST_CASE("to_string(const char[N])") {
  CHECK( std_e::to_string("my const char array") == "my const char array" );
}

TEST_CASE("to_string(const char*)") {
  const char* ptr = "my const char ptr";
  CHECK( std_e::to_string(ptr) == "my const char ptr" );
}

TEST_CASE("range_to_string") {
  std::vector<int> v0 = {};
  CHECK( range_to_string(v0)==std::string("{}") );

  std::vector<int> v1 = {42};
  CHECK( range_to_string(v1)==std::string("{42}") );

  std::vector<int> v2 = {42,43,44};
  CHECK( range_to_string(v2)==std::string("{42,43,44}") );
}

TEST_CASE("str_cat") {
  std::string s0 = "This is";
  std::string s = "with";
  std::string_view s1 = s;
  const char* s2 = "number ";
  int i = 42;
  CHECK( std_e::str_cat(s0, " my sentence ", s1, ' ', s2, i) == "This is my sentence with number 42" );
}
