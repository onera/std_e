#pragma once

// SLOG (synchronized log) is a debug utility that synchronizes prints in order
// It has to be called by all ranks of the communicator

#include "std_e/parallel/string.hpp"
#include <iostream>


namespace std_e {
  template<class... Ts> auto
  _rank_color_msg(MPI_Comm comm, const Ts&... xs) -> std::string {
    int rk = rank(comm);
    using std::to_string;
    using std_e::to_string;
    std::string msg = (to_string(xs) + ...) + "\n";
    std::string rank_msg = to_color_string(console_color::blue,"Rank ",rk,": ") + msg;

    return rank_msg;
  }

  template<class... Ts> auto
  async_log(MPI_Comm comm, const Ts&... xs) -> void {
    std::string rank_msg = _rank_color_msg(comm, xs...);
    std::cout << rank_msg;
  }
  template<class... Ts> auto
  sync_log(MPI_Comm comm, const Ts&... xs) -> void {
    std::string rank_msg = _rank_color_msg(comm, xs...);
    std::cout << cat_on_rank_0(rank_msg, comm);
  }
} // std_e

#define ASYNC_LOG(comm,x) std_e::async_log(comm,#x," = ",x)
#define SYNC_LOG (comm,x) std_e::sync_log(comm,#x," = ",x)

// For lazy people (only usable if the communicator is called `comm`)
#define ALOG(msg) std_e::async_log(comm,msg)
#define EALOG(x)  std_e::async_log(comm,#x," = ",x)

#define SLOG(msg) std_e::sync_log(comm,msg)
#define ESLOG(x)  std_e::sync_log(comm,#x," = ",x)
