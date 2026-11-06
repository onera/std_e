#pragma once


#include "std_e/base/msg_exception.hpp"
#include "std_e/base/stacktrace_to_string.hpp"


namespace std_e {


class stacktrace_exception : public ::std_e::msg_exception {
  public:
    stacktrace_exception() = default;

    explicit
    stacktrace_exception(auto&&... msg_parts)
      : msg_exception(msg_parts...,'\n',stacktrace_to_string())
    {}
};


} // std_e
