#if __cplusplus > 201703L


#include "std_e/unit_test/doctest.hpp"
#include "std_e/unit_test/thread.hpp"

#include "std_e/utils/timer.hpp"
#include "std_e/graph/adjacency_graph/graph_to_dot_string.hpp"
#include "std_e/execution/task.hpp"
#include "std_e/execution/schedule/seq.hpp"
#include "std_e/execution/schedule/async_comm.hpp"

#include "std_e/utils/concatenate.hpp"

using namespace std_e;
using namespace std::string_literals;

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
constexpr auto concatenate_vec = [](const std::vector<int>& x, const std::vector<int>& y){
  return concatenate(x,y);
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

  CHECK( *s0.result() == std::vector{3,0,1,2} );
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

  CHECK( *s0.result() == std::vector{3,0,1,2,5} );
}

TEST_CASE("task fork join") {
  task_graph tg;

  auto s0 = input_data(tg,std::vector{3,0,1,2}) | then(push_5);
  auto s1 = join(
    s0 | then(reverse_vec),
    s0 | then(sort_vec)
  );
  auto s2 = std::move(s1) | then(concatenate_vec);

  CHECK( tg.size() == 5 );

  // execute graph by hand
  tg.node(0).execute();
  tg.node(1).execute();
  tg.node(2).execute();
  tg.node(3).execute();
  tg.node(4).execute();
                                //  reverse  /   sort
                                // v v v v v   v v v v v
  CHECK( *s2.result() == std::vector{5,2,1,0,3,  0,1,2,3,5} );
}

constexpr double task_test_sleep_duration = 0.01;

constexpr auto get_remote_info = [](std::vector<int> x){
  // here, suppose that we are getting info from elsewhere
  // through an "i/o" operation, that is, an operation that needs to wait
  // but that is not compute-intensive
  sleep_for_seconds(task_test_sleep_duration);
  x.push_back(6);
  x.push_back(5);
  x.push_back(4);
  x.push_back(7);
  return x;
};
constexpr auto reverse_vec_with_delay = [](std::vector<int> v){
  sleep_for_seconds(task_test_sleep_duration);
  std::vector res = v;
  std::reverse(begin(res),end(res));
  return res;
};
TEST_CASE("then_comm") {
  task_graph tg;

  auto s0 = input_data(tg,std::vector{3,0,1,2}) | then(sort_vec);
  auto s1 = s0 | then_comm(get_remote_info);
  auto s2 = s0 | then(reverse_vec_with_delay);
  auto s3 = join(std::move(s1),std::move(s2)) | then(concatenate_vec);

  CHECK( tg.size() == 5 );

  // TODO Timings are too high with OpenMPI
  //SUBCASE("seq") {
  //  timer t;
  //  auto res = execute_seq(s3);
  //  double elaps = t.elapsed();
  //  CHECK( res == std::vector{0,1,2,3, 6,5,4,7,    3,2,1,0} );
  //  // since tasks are executed sequentially, it needs approx 2*task_test_sleep_duration
  //  CHECK( elaps - 2*task_test_sleep_duration < task_test_sleep_duration / 10 );
  //}
  //#if __cpp_lib_atomic_wait
  //  SUBCASE("comm") {
  //    thread_pool comm_tp(1);

  //    timer t;
  //    auto res = execute_async_comm(s3,comm_tp);
  //    double elaps = t.elapsed();

  //    CHECK( res == std::vector{0,1,2,3, 6,5,4,7,    3,2,1,0} );
  //    // the two "heavy" tasks `get_remote_info` and `revers_vec_with_delay` need appox task_test_sleep_duration, but are done in parallel
  //    CHECK( elaps - task_test_sleep_duration < task_test_sleep_duration / 10 );
  //  }
  //#endif
}


#endif // C++20
