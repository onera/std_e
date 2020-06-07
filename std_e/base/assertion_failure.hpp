

#include "std_e/base/macros.hpp"
#include <exception>
#include <string>
#include "std_e/base/stacktrace_to_string.hpp"


namespace std_e {


class assertion_failure : public std::exception {
  public:
    assertion_failure(const std::string& file, int line, const std::string& assertion_test) noexcept
    {
      msg = "assertion failure at file " + file + ", line " + std::to_string(line) + "\nfailed assertion: " + assertion_test + "\n";
      msg += stacktrace_to_string() + "\n";
    }

    const char* what() const noexcept override {
      return msg.c_str();
    }
  private:
    std::string msg;
};


} // std_e
