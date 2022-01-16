#pragma once


// SLOG (synchronized log) is a debug utility that synchronizes prints in order
// It has to be called by all ranks of the communicator

#include "std_e/log.hpp"
#include "std_e/logging/console_color.hpp"
#include "std_e/parallel/mpi/point_to_point/point_to_point.hpp"


namespace std_e {

template<class... Ts> auto
slog(MPI_Comm comm, const Ts&... xs) -> void {
  int rk = rank(comm);
  int n_rk = n_rank(comm);
  using std::to_string;
  using std_e::to_string;
  std::string msg = (to_string(xs) + ...) + "\n";
  std::string rank_msg = to_color_string(console_color::blue,"Rank ",rk,": ") + msg;

  MPI_Barrier(comm);
  if (rk == 0) {
    std::cout << rank_msg;
    for (int i=1; i<n_rk; ++i) {
      int sz;
      int tag0 = i;
      int tag1 = n_rk+i;
      _recv(&sz,1,i,tag0,comm);
      std::string recv_msg(sz,'\0');
      _recv(recv_msg.data(),sz,i,tag1,comm);
      std::cout << recv_msg;
    }
  } else {
    int sz = rank_msg.size();
    int tag0 = rk;
    int tag1 = n_rk+rk;
    _send(&sz,1,0,tag0,comm);
    _send(rank_msg.data(),sz,0,tag1,comm);
  }
  MPI_Barrier(comm);
}

} // std_e

#define SLOG(comm,x) std_e::slog(comm,#x," = ",x)
