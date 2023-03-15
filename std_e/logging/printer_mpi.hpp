#pragma once


#include "std_e/logging/printer.hpp"
#include <iostream>
#include <fstream>
#include "std_e/logging/console_color.hpp"
#include "std_e/parallel/string.hpp"
#include "std_e/parallel/mpi.hpp"
#include "std_e/utils/string.hpp"


namespace std_e {


class mpi_console_printer : public printer {
  public:
    mpi_console_printer(std::ostream* os)
      : os(os)
    {}
    auto log(const std::string& msg) -> void override {
      std::string rank_msg = to_color_string(console_color::blue,"Rank ",mpi_comm_world_rank(),": ") + msg;
      std::cout << rank_msg << std::flush;
    }
  private:
    std::ostream* os;
};
class mpi_stdout_printer : public mpi_console_printer {
  public:
    mpi_stdout_printer()
      : mpi_console_printer(&std::cout)
    {}
};
class mpi_stderr_printer : public mpi_console_printer {
  public:
    mpi_stderr_printer()
      : mpi_console_printer(&std::cerr)
    {}
};


class mpi_rank_0_console_printer : public printer {
  public:
    mpi_rank_0_console_printer(std::ostream* os)
      : os(os)
    {}
    auto log(const std::string& msg) -> void override {
      if (mpi_comm_world_rank()==0) {
        (*os) << msg << std::flush;
      }
    }
  private:
    std::ostream* os;
};
class mpi_rank_0_stdout_printer : public mpi_rank_0_console_printer {
  public:
    mpi_rank_0_stdout_printer()
      : mpi_rank_0_console_printer(&std::cout)
    {}
};
class mpi_rank_0_stderr_printer : public mpi_rank_0_console_printer {
  public:
    mpi_rank_0_stderr_printer()
      : mpi_rank_0_console_printer(&std::cerr)
    {}
};


class mpi_sync_stdout_printer : public printer {
  public:
    mpi_sync_stdout_printer() = default;
    mpi_sync_stdout_printer(MPI_Comm comm)
      : comm(comm)
    {}

    auto log(const std::string& msg) -> void override {
      int rk = rank(comm);
      std::string rank_msg = to_color_string(console_color::blue,"Rank ",rk,": ") + msg;
      std::cout << cat_on_rank_0(rank_msg,comm) << std::flush;
    }
  private:
    MPI_Comm comm;
};




class mpi_file_printer : public printer {
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
      file_name = join(tokens,".")+'.'+std::to_string(mpi_comm_world_rank())+'.'+extension;
    }

    auto log(const std::string& msg) -> void override {
      if (!log_init) {
        file = std::ofstream(file_name, std::fstream::out);
        log_init = true;
      }
      file << msg << std::flush;
    }
  private:
    std::string file_name;
    std::ofstream file;
    bool log_init = false;
};


} // std_e
