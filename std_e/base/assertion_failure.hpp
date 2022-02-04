

#include "std_e/base/msg_exception.hpp"
#include "std_e/base/stacktrace_to_string.hpp"


namespace std_e {


class assertion_failure : public msg_exception {
  public:
    using base = msg_exception;

    assertion_failure() noexcept = default;

    assertion_failure(const std::string& file, int line, const std::string& assertion_test) noexcept
      : base("assertion failure at file " + file + ", line " + std::to_string(line) + '\n'
           + "failed assertion: " + assertion_test + '\n'
           + stacktrace_to_string() + '\n')
      {}
};


} // std_e
