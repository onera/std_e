#pragma once


#include "std_e/base/msg_exception.hpp"


namespace std_e {


class mpi_exception : public msg_exception {
  public:
    using base = msg_exception;

    mpi_exception() noexcept = default;

    mpi_exception(int mpi_error_code, std::string desc)
      : base("MPI error ["+std::to_string(mpi_error_code)+"] "+std::move(desc))
    {}
};


} // std_e
