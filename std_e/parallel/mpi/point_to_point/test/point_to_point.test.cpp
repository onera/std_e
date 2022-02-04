#if __cplusplus > 201703L


#include "std_e/unit_test/doctest_mpi.hpp"
#include "std_e/unit_test/thread.hpp"

#include "std_e/parallel/mpi/point_to_point/point_to_point.hpp"
#include "std_e/utils/timer.hpp"
#include "std_e/execution/execution.hpp"

#include "std_e/graph/adjacency_graph/graph_to_dot_string.hpp"
#include "std_e/utils/concatenate.hpp"

using namespace std_e;
using namespace std::string_literals;

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

constexpr double p_to_p_test_sleep_duration = 0.1;

auto
send_recv_msg_0(two_vec& x, MPI_Comm comm) -> std::vector<int> {
  sleep_for_seconds(p_to_p_test_sleep_duration);
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
  sleep_for_seconds(1.1*p_to_p_test_sleep_duration);
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
constexpr auto concatenate_vec = [](const std::vector<int>& x, const std::vector<int>& y){
  return concatenate(x,y);
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
  future s0 = input_data(tg,std::move(x));
  future s1 = s0 | then_comm2(send_recv_msg_0,test_comm) | then(reverse_msg);
  future s2 = s0 | then_comm2(send_recv_msg_1,test_comm) | then(reverse_msg);
  auto s3 = join(std::move(s1),std::move(s2)) | then(concatenate_vec);

  CHECK( tg.size() == 6 );

  SUBCASE("seq") {
    timer t;
    auto res = execute_seq(s3);
    double elaps = t.elapsed();

    CHECK( res == std::vector{2,1,0, 5,4,3} );
    CHECK( elaps - 2.*p_to_p_test_sleep_duration < 2.*p_to_p_test_sleep_duration / 10 );
  }
  SUBCASE("async comm") {
    // WARNING: This test seems to work consistently (in term of time taken) with 4 comm threads.
    //          Two comm threads should be enought, however, it result in no parallelisation most of the time
    //            (but it is inconsistent and sometimes, the timing is the one expected
    // TODO: investigate (thread_pool bug? MPI funny behavior? ...)
    thread_pool comm_tp(4);

    timer t;
    auto res = execute_async_comm(s3,comm_tp);
    double elaps = t.elapsed();

    CHECK( res == std::vector{2,1,0, 5,4,3} );
    // the two "heavy" communication tasks `send_recv_msg_0` and `send_recv_msg_1` are done in parallel
    CHECK( elaps - 1.1*p_to_p_test_sleep_duration < 1.1*p_to_p_test_sleep_duration / 10 );
  }
}

} // async_test


#endif // C++20
