#include "std_e/unit_test/doctest.hpp"

#include "std_e/log.hpp"
#include "std_e/execution/sender.hpp"
#include "std_e/execution/pipeable.hpp"
#include "std_e/execution/execution.hpp"

using namespace std_e;
using namespace std::string_literals;

TEST_CASE("input_sender") {
  std::string str = "test_string";

  auto s0 = input_sender(std::move(str));
  CHECK( execute(s0) == "test_string" );
}


constexpr auto say_hello = [](const std::string& x){
  return "hello " + x;
};
TEST_CASE("chaining input and then") {
  auto s0 = input_sender("test_string"s) | then(say_hello);
  CHECK( execute(s0) == "hello test_string" );
}

constexpr auto reverse_str = [](auto&& str){
  std::reverse(begin(str),end(str));
  return str;
};

TEST_CASE("chaining multiple then") {
  auto s0 = input_sender("123"s) | then(say_hello) | then(reverse_str);
  CHECK( execute(s0) == "321 olleh" );
}
