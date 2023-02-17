#pragma once


#include "std_e/logging/log.hpp"
#include "std_e/logging/console_color.hpp"
#include "std_e/parallel/mpi.hpp"
#include "std_e/utils/string.hpp"


namespace std_e {


class mpi_stdout_printer : public log_printer {
  public:
    auto log(const std::string& msg) -> void override {
      std::string rank_msg = to_color_string(console_color::blue,"Rank ",mpi_comm_world_rank(),": ") + msg;
      std::cout << rank_msg << std::flush;
    }
};
class mpi_rank_0_stdout_printer : public log_printer {
  public:
    auto log(const std::string& msg) -> void override {
      if (mpi_comm_world_rank()==0) {
        std::cout << msg << std::flush;
      }
    }
};
class mpi_file_printer : public log_printer {
  public:
    mpi_file_printer() = default;
    mpi_file_printer(const std::string& file)
    {
      auto tokens = split(file,'.');
      std::string extension = "";
      if (tokens.size()>=2) { // there is an extension
        extension = tokens.back();
        tokens.pop_back();
      }
      file_name = join(tokens,".")+'_'+std::to_string(mpi_comm_world_rank())+'.'+extension;
    }

    auto log(const std::string& msg) -> void override {
      if (!log_init) {
        file = std::ofstream(file_name.c_str(), std::fstream::out);
        log_init = true;
      }
      file << msg << std::flush;
    }
  private:
    std::string file_name;
    std::ofstream file;
    bool log_init = false;
};


auto add_mpi_stdout_printer(const std::string& logger_name) -> void;
auto add_mpi_rank_0_stdout_printer(const std::string& logger_name) -> void;
auto add_mpi_file_printer(const std::string& logger_name, const std::string& file_name) -> void;

} // std_e
