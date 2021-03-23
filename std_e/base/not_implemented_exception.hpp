#pragma once


#include "std_e/base/msg_exception.hpp"


namespace std_e {


class not_implemented_exception
  : public msg_exception
{
  using msg_exception::msg_exception;
};


} // std_e
