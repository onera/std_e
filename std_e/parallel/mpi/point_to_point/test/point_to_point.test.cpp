#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/parallel/mpi/point_to_point/point_to_point.hpp"
#include "std_e/logging/time_logger.hpp" // TODO
#include "std_e/execution/execution.hpp"

#include "std_e/graph/adjacency_graph/graph_to_dot_string.hpp"
#include "std_e/log.hpp"

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


auto
send_recv_msg_0(MPI_Comm comm) -> std::vector<int> {
  std::this_thread::sleep_for(0.1s);
  int rk = rank(comm);
  // 0. init
  std::vector<int> buf;
  if (rk == 0) {
    buf = {0,1,2};
  }
  if (rk == 1) {
    buf = {-1,-1,-1};
  }

  // 1. send and recv
  int tag = 42;
  if (rk == 0) {
    send(buf,1,tag,comm);
  }
  if (rk == 1) {
    recv(buf,0,tag,comm);
  }
  return buf;
}
auto
send_recv_msg_1(MPI_Comm comm) -> std::vector<int> {
  int rk = rank(comm);
  // 0. init
  std::vector<int> buf;
  if (rk == 0) {
    buf = {3,4,5};
  }
  if (rk == 1) {
    buf = {-1,-1,-1};
  }

  // 1. send and recv
  int tag = 43;
  if (rk == 0) {
    send(buf,1,tag,comm);
  }
  if (rk == 1) {
    recv(buf,0,tag,comm);
  }
  return buf;
}

auto
reverse_msg(std::vector<int>&& v) -> std::vector<int> {
  std::reverse(begin(v),end(v));
  return v;
}


MPI_TEST_CASE("send recv overlap",2) {
  task_graph tg;
  //auto s0 = s | then_comm(send_recv_msg_0) | then(reverse_msg);
  //auto s1 = s | then_comm(send_recv_msg_1) | then(reverse_msg);
  task_graph_handle auto s0 = input_data(tg,0) | split(); // TODO 0 because need to input sth
  task_graph_handle auto s1 = s0 | then_comm([test_comm](int){ return send_recv_msg_0(test_comm); }) | then(reverse_msg); // TODO int&& is for the previous "0" of input_data
  task_graph_handle auto s2 = s0 | then_comm([test_comm](int){ return send_recv_msg_1(test_comm); }) | then(reverse_msg);
  auto s3 = join(s1,s2);

  CHECK( tg.size() == 6 );

  // TODO doctest deadlocks with two SUBCASE (even if empty)
  //SUBCASE("seq") {
  //  auto _ = std_e::stdout_time_logger("execute seq");
  //  CHECK( execute_seq(s3) == std::tuple{std::vector{2,1,0},std::vector{5,4,3}} );
  //}
  SUBCASE("comm") {
    thread_pool comm_tp(1);
    auto _ = std_e::stdout_time_logger("execute async comm");
    CHECK( execute_async_comm(s3,comm_tp) == std::tuple{std::vector{2,1,0},std::vector{5,4,3}} );
  }
}
