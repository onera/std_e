#pragma once


#include "std_e/base/msg_exception.hpp"
#include "std_e/base/stacktrace_to_string.hpp"


namespace std_e {


class stacktrace_exception : public msg_exception {
  public:
    stacktrace_exception() = default;

    explicit
    stacktrace_exception(std::string msg)
      : msg_exception(std::move(msg) + "\n" + stacktrace_to_string())
    {}
};


} // std_e
