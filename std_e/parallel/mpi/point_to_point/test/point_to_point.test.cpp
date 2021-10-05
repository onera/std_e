#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/parallel/mpi/point_to_point/point_to_point.hpp"
#include "std_e/logging/time_logger.hpp" // TODO
#include "std_e/execution/execution.hpp"

using namespace std_e;

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
  task_graph_handle auto s = input_data(tg,0);
  task_graph_handle auto s0 = s | then_comm([test_comm]{ send_recv_msg_0(test_comm); }) | then(reverse_msg);
  task_graph_handle auto s1 = s | then_comm([test_comm]{ send_recv_msg_1(test_comm); }) | then(reverse_msg);
  auto s2 = join(s0,s1);

  CHECK( tg.size() == 0 );

  SUBCASE("seq") {
    auto _ = std_e::stdout_time_logger("execute seq");
    CHECK( execute_seq(s2) == std::tuple{std::vector{0,1,2,3, 6,5,4,7}, 3} );
  }
  SUBCASE("comm") {
    thread_pool comm_tp(1);
    auto _ = std_e::stdout_time_logger("execute async comm");
    CHECK( execute_async_comm(s2,comm_tp) == std::tuple{std::vector{0,1,2,3, 6,5,4,7}, 3} );
  }
}
