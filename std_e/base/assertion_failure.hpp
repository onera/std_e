#pragma once


#include "std_e/base/stacktrace_exception.hpp"


namespace std_e {


class assertion_failure : public stacktrace_exception {
  public:
    using base = stacktrace_exception;

    assertion_failure() noexcept = default;

    explicit
    assertion_failure(const std::string& file, int line, const std::string& assertion_test) noexcept
      : base("assertion failure at file " + file + ", line " + std::to_string(line) + '\n'
           + "failed assertion: " + assertion_test)
      {}
};


} // std_e
