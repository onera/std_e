#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/parallel/mpi/point_to_point/point_to_point.hpp"
#include "std_e/logging/time_logger.hpp" // TODO
#include "std_e/log.hpp" // TODO
#include "std_e/execution/execution.hpp"

#include "std_e/graph/adjacency_graph/graph_to_dot_string.hpp"
#include "std_e/utils/concatenate.hpp"

using namespace std_e;
using namespace std::string_literals;
using namespace std::chrono_literals;

MPI_TEST_CASE("send and recv",2) {
  // 0. init
  std::vector<int> buf_0;
  std::vector<int> buf_1;
  if (test_rank == 0) {
    buf_0 = {0,1,2};
    buf_1 = {3,4,5};
  }
  if (test_rank == 1) {
    buf_0 = {-1,-1,-1};
    buf_1 = {-1,-1,-1};
  }

  // 1. send and recv
  int tag_0 = 42;
  int tag_1 = 43;
  if (test_rank == 0) {
    send(buf_0,1,tag_0,test_comm);
    send(buf_1,1,tag_1,test_comm);
  }
  if (test_rank == 1) {
    recv(buf_0,0,tag_0,test_comm);
    recv(buf_1,0,tag_1,test_comm);
  }

  // 2. test
  CHECK(buf_0 == std::vector{0,1,2});
  CHECK(buf_1 == std::vector{3,4,5});
}


namespace {
  struct two_vec {
    std::vector<int> v0;
    std::vector<int> v1;
  };
}


namespace async_test {

auto
send_recv_msg_0(two_vec& x, MPI_Comm comm) -> std::vector<int> {
  std::this_thread::sleep_for(10ms);
  int rk = rank(comm);
  int tag = 42;
  if (rk == 0) {
    send(x.v0,1,tag,comm);
  }
  if (rk == 1) {
    recv(x.v0,0,tag,comm);
  }
  return x.v0;
}
auto
send_recv_msg_1(two_vec& x, MPI_Comm comm) -> std::vector<int> {
  std::this_thread::sleep_for(10ms);
  int rk = rank(comm);
  int tag = 43;
  if (rk == 0) {
    send(x.v1,1,tag,comm);
  }
  if (rk == 1) {
    recv(x.v1,0,tag,comm);
  }
  return x.v1;
}

auto
reverse_msg(std::vector<int>&& v) -> std::vector<int> {
  std::reverse(begin(v),end(v));
  return v;
}
constexpr auto concatenate_vec = [](const std::tuple<std::vector<int>,std::vector<int>>& vs){
  return concatenate(std::get<0>(vs),std::get<1>(vs));
};


MPI_TEST_CASE("send recv async overlap",2) {
  int rk = rank(test_comm);

  two_vec x;
  if (rk == 0) {
    x.v0 = {0,1,2};
    x.v1 = {3,4,5};
  }
  if (rk == 1) {
    x.v0 = {-1,-1,-1};
    x.v1 = {-1,-1,-1};
  }

  task_graph tg;
  Task_graph_handle auto s0 = input_data(tg,std::move(x)) | split();
  Task_graph_handle auto s1 = s0 | then_comm(send_recv_msg_0,test_comm) | then(reverse_msg);
  Task_graph_handle auto s2 = s0 | then_comm(send_recv_msg_1,test_comm) | then(reverse_msg);
  auto s3 = join(s1,s2) | then(concatenate_vec);

  CHECK( tg.size() == 7 );

  // TODO doctest deadlocks with two SUBCASE (even if empty)
  //SUBCASE("seq") {
  //  auto _ = std_e::stdout_time_logger("execute seq");
  //  CHECK( execute_seq(s3) == std::vector{2,1,0, 5,4,3} );
  //}
  SUBCASE("async comm") {
    thread_pool comm_tp(2);
    auto _ = std_e::stdout_time_logger("send recv async comm");
    CHECK( execute_async_comm(s3,comm_tp) == std::vector{2,1,0, 5,4,3} );
  }
}

} // async_test
