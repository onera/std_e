#include "std_e/unit_test/doctest.hpp"

#include "std_e/log.hpp" // TODO
#include "std_e/logging/time_logger.hpp" // TODO
#include "std_e/execution/sender.hpp"
#include "std_e/execution/execution.hpp"
#include "std_e/execution/execution_ext.hpp"

#include "std_e/utils/concatenate.hpp"

#include <thread>
#include <chrono>

using namespace std_e;
using namespace std::string_literals;
using namespace std::chrono_literals;

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


constexpr auto get_remote_info = [](std::vector<int> x){
  // here, suppose that we are getting info from elsewhere
  // through an "i/o" operation, that is, an operation that needs to wait
  // but that is not compute-intensive
  std::this_thread::sleep_for(0.1s);
  x.push_back(6);
  x.push_back(5);
  x.push_back(4);
  x.push_back(7);
  return x;
};
constexpr auto max_vec = [](const std::vector<int>& x){
  std::this_thread::sleep_for(0.1s);
  return *std::max_element(begin(x),end(x));
};
TEST_CASE("then comm") {
  auto s0 = input_sender(std::vector{3,0,1,2}) | then(sort_vec) | split();
  auto s1 = s0 | then_comm(get_remote_info);
  auto s2 = s0 | then(max_vec);
  auto s3 = wait_all(s1,s2);
  auto _ = std_e::stdout_time_logger("execute comm and compute in par");
  CHECK( execute(s3) == std::tuple{std::vector{0,1,2,3, 6,5,4,7}, 3} );
}
