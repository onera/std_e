#pragma once


#include "std_e/base/macros.h"
#include <exception>


namespace std_e {


class assertion_failure {};


class precondition_violation : public std::exception {
  public:
    precondition_violation() noexcept
      : msg(nullptr)
    {}

    precondition_violation(const char* msg) noexcept
      : msg(msg)
    {}

    const char* what() const noexcept override {
      return msg;
    }
  private:
    const char* msg;
};


FORCE_INLINE constexpr auto
ASSERT(bool b) {
  if (!b) {
    throw assertion_failure();
  }
}


} // std_e
