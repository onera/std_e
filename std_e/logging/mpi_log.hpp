#pragma once


#include "std_e/logging/log.hpp"
#include "std_e/logging/console_color.hpp"
#include "std_e/parallel/mpi.hpp"


namespace std_e {


struct mpi_stdout_printer : log_printer {
  auto log(const std::string& msg) -> void override {
    std::string rank_msg = to_color_string(console_color::blue,"Rank ",rank(MPI_COMM_WORLD),": ") + msg;
    std::cout << rank_msg;
  }
};
struct mpi_rank_0_stdout_printer : log_printer {
  auto log(const std::string& msg) -> void override {
    if (rank(MPI_COMM_WORLD)==0) {
      std::cout << msg;
    }
  }
};
struct mpi_file_printer : log_printer {
  std::string file_name = "mpi_log_"+std::to_string(mpi_comm_world_rank())+".txt";
  std::ofstream file = std::ofstream(file_name.c_str(), std::fstream::out);

  auto log(const std::string& msg) -> void override {
    file << msg;
  }
};


// print only on proc 0 {
auto log_0(logger& l, const std::string& msg) -> void;
auto log_0(const std::string& log_name, const std::string& msg) -> void;
auto log_0(logging_level log_lvl, const std::string& msg) -> void;
auto log_0(const std::string& msg) -> void;
// print only on proc 0 }


} // std_e
