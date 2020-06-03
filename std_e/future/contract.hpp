#pragma once


#include "std_e/base/macros.hpp"
#include <exception>
#include <string>


namespace std_e {


class assertion_failure : public std::exception {
  public:
    assertion_failure(const std::string& file, int line, const std::string& assertion_test) noexcept
    {
      msg = "assertion failure at file " + file + ", line " + std::to_string(line) + "\nfailed assertion: " + assertion_test;
    }

    const char* what() const noexcept override {
      return msg.c_str();
    }
  private:
    std::string msg;
};


} // std_e

#ifdef NDEBUG
#define STD_E_ASSERT(b) (void(0))
#else
#define STD_E_ASSERT(b) if (!(b)) throw ::std_e::assertion_failure(__FILE__,__LINE__,#b);
#endif
