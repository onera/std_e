#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/parallel/mpi/point_to_point/point_to_point.hpp"

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


//auto
//send_recv_msg_0() -> std::vector<int> {
//  auto comm = MPI_COMM_WORLD;
//  // 0. init
//  std::vector<int> buf;
//  if (test_rank == 0) {
//    buf = {0,1,2};
//  }
//  if (test_rank == 1) {
//    buf = {-1,-1,-1};
//  }
//
//  // 1. send and recv
//  int tag = 42;
//  if (test_rank == 0) {
//    send(buf,1,tag,comm);
//  }
//  if (test_rank == 1) {
//    recv(buf,0,tag,comm);
//  }
//  return buf;
//}
//auto
//send_recv_msg_1() -> std::vector<int> {
//  auto comm = MPI_COMM_WORLD;
//  // 0. init
//  std::vector<int> buf;
//  if (test_rank == 0) {
//    buf = {3,4,5};
//  }
//  if (test_rank == 1) {
//    buf = {-1,-1,-1};
//  }
//
//  // 1. send and recv
//  int tag = 43;
//  if (test_rank == 0) {
//    send(buf,1,tag,test_comm);
//  }
//  if (test_rank == 1) {
//    recv(buf,0,tag,test_comm);
//  }
//  return buf;
//}
//
//auto
//reverse_msg(std::vector<int>&& v) -> v {
//  std::reverse(begin(v),end(v));
//  return v;
//}
//
//
//then_comm(sender s) {
//  send(buf_0,1,tag_0,test_comm);
//}
//
//
//auto
//test_comm_overlap(sender s) -> sender {
//  //sender s0 = s | then_comm(send_recv_msg_0) | then(reverse_msg);
//  //sender s1 = s | then_comm(send_recv_msg_1) | then(reverse_msg);
//  //sender s0 = then( then_comm(s,send_recv_msg_0) , reverse_msg );
//  //sender s1 = then( then_comm(s,send_recv_msg_1) , reverse_msg );
//  sender s0 = then( then(s,send_recv_msg_0) , reverse_msg );
//  sender s1 = then( then(s,send_recv_msg_1) , reverse_msg );
//  return when_all(s0,s1)
//}

