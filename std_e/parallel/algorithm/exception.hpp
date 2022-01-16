#pragma once


#include "std_e/base/msg_exception.hpp"


namespace std_e {


class par_algo_exception : public msg_exception {
  public:
    using base = msg_exception;
    using base::base;
};


} // std_e
