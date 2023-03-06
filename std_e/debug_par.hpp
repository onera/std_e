#pragma once

// SLOG (synchronized log) is a debug utility that synchronizes prints in order
// It has to be called by all ranks of the communicator

#include "std_e/parallel/string.hpp"
#include <iostream>


namespace std_e {
  template<class... Ts> auto
  slog(const Ts&... xs, MPI_Comm comm) -> void {
    int rk = rank(comm);
    using std::to_string;
    using std_e::to_string;
    std::string msg = (to_string(xs) + ...) + "\n";
    std::string rank_msg = to_color_string(console_color::blue,"Rank ",rk,": ") + msg;

    std::cout << cat_on_rank_0(rank_msg, comm);
  }
} // std_e

#define SLOG(comm,x) std_e::slog(comm,#x," = ",x)
