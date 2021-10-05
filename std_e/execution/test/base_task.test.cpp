#include "std_e/unit_test/doctest.hpp"

#include "std_e/log.hpp" // TODO
#include "std_e/logging/time_logger.hpp" // TODO
#include "std_e/graph/adjacency_graph/graph_to_dot_string.hpp"
#include "std_e/execution/task.hpp"
#include "std_e/execution/schedule/seq.hpp"
#include "std_e/execution/schedule/async_comm.hpp"

#include "std_e/utils/concatenate.hpp"

#include <thread>
#include <chrono>

using namespace std_e;
using namespace std::string_literals;
using namespace std::chrono_literals;

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
TEST_CASE("input_task") {
  task_graph tg;

  auto s0 = input_data(tg,std::vector{3,0,1,2});
  CHECK( tg.size() == 1 );
  CHECK( tg.in_indices().size() == 1 );
  CHECK( tg.out_indices().size() == 1 );

  CHECK( tg.in_indices(0).size() == 0 );
  CHECK( tg.out_indices(0).size() == 0 );

  // execute graph by hand
  tg.node(0).execute();

  CHECK( *s0.result == std::vector{3,0,1,2} );
}
TEST_CASE("then_task") {
  task_graph tg;

  auto s0 = then( input_data(tg,std::vector{3,0,1,2}) , push_5);
  CHECK( tg.size() == 2 );
  CHECK( tg.in_indices().size() == 2 );
  CHECK( tg.out_indices().size() == 2 );

  CHECK( tg.in_indices(0).size() == 0 );
  CHECK( tg.out_indices(0).size() == 1 );
  CHECK( tg.out_indices(0)[0] == 1 );

  CHECK( tg.in_indices(1).size() == 1 );
  CHECK( tg.out_indices(1).size() == 0 );
  CHECK( tg.in_indices(1)[0] == 0 );

  // execute graph by hand
  tg.node(0).execute();
  tg.node(1).execute();

  CHECK( *s0.result == std::vector{3,0,1,2,5} );
}

TEST_CASE("task fork join") {
  task_graph tg;

  auto s0 = input_data(tg,std::vector{3,0,1,2}) | then( push_5) | split();
  auto s1 = join(
    s0 | then(reverse_vec),
    s0 | then(sort_vec)
  );
  auto s2 = s1 | then(concatenate_vec);

  CHECK( tg.size() == 6 );

  // execute graph by hand
  tg.node(0).execute();
  tg.node(1).execute();
  tg.node(2).execute();
  tg.node(3).execute();
  tg.node(4).execute();
  tg.node(5).execute();
                                //  reverse  /   sort
                                // v v v v v   v v v v v
  CHECK( *s2.result == std::vector{5,2,1,0,3,  0,1,2,3,5} );
}


constexpr auto get_remote_info = [](std::vector<int> x){
  // here, suppose that we are getting info from elsewhere
  // through an "i/o" operation, that is, an operation that needs to wait
  // but that is not compute-intensive
  std::this_thread::sleep_for(0.11s);
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
TEST_CASE("then_comm") {
  task_graph tg;

  auto s0 = input_data(tg,std::vector{3,0,1,2}) | then(sort_vec) | split();
  auto s1 = s0 | then_comm(get_remote_info);
  auto s2 = s0 | then(max_vec);
  auto s3 = join(s1,s2);

  CHECK( tg.size() == 5 );

  SUBCASE("seq") {
    auto _ = std_e::stdout_time_logger("execute seq");
    CHECK( execute_seq(s3) == std::tuple{std::vector{0,1,2,3, 6,5,4,7}, 3} );
  }
  SUBCASE("comm") {
    thread_pool comm_tp(1);
    auto _ = std_e::stdout_time_logger("execute async comm");
    CHECK( execute_async_comm(s3,comm_tp) == std::tuple{std::vector{0,1,2,3, 6,5,4,7}, 3} );
  }
}
