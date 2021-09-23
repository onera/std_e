#include "std_e/unit_test/doctest.hpp"

#include "std_e/log.hpp"
#include "std_e/execution/sender.hpp"
#include "std_e/execution/execution.hpp"

#include "std_e/utils/concatenate.hpp"

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
  CHECK( execute(s0) == "321 olleh" ); // "olleh" is "hello" reversed
}

constexpr auto push_5 = [](std::vector<int>&& x){
  x.push_back(5);
  return x;
};
constexpr auto reverse_vec = [](std::vector<int> v){
  std::vector res = v;
  std::reverse(begin(res),end(res));
  return res;
};
constexpr auto sort_vec = [](std::vector<int> v){
  std::sort(begin(v),end(v));
  return v;
};
constexpr auto concatenate_vec = [](const std::tuple<std::vector<int>,std::vector<int>>& vs){
  return concatenate(std::get<0>(vs),std::get<1>(vs));
};
TEST_CASE("fork join") {
  auto multi_shot = input_sender(std::vector{3,0,1,2}) | then(push_5) | split();
  auto s2 = wait_all(
    multi_shot | then(reverse_vec),
    multi_shot | then(sort_vec)
  );
  auto s3 = std::move(s2) | then(concatenate_vec);
                                 //  reverse  /   sort
                                 // v v v v v   v v v v v
  CHECK( execute(s3) == std::vector{5,2,1,0,3,  0,1,2,3,5} );
}
