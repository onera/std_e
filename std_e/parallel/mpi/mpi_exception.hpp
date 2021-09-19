#pragma once


#include <exception>
#include <string>


namespace std_e {


class mpi_exception : public std::exception {
  public:
    mpi_exception() = default;

    mpi_exception(int mpi_error_code, std::string desc)
      : desc("MPI error ["+std::to_string(mpi_error_code)+"] "+std::move(desc))
    {}

    const char* what() const noexcept override {
      return desc.c_str();
    }
  private:
    std::string desc;
};


} // std_e
