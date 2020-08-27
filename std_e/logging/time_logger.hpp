#pragma once


#include <chrono>
#include "std_e/logging/log.hpp"


namespace std_e {


class time_logger {
  public:
    using time_type = std::chrono::time_point<std::chrono::system_clock>;
    time_logger(logger* l_ptr, std::string s)
      : l_ptr(l_ptr)
      , msg(std::move(s))
      , start_time(std::chrono::system_clock::now())
    { 
      log(*l_ptr,msg+"\n");
    }

    ~time_logger() {
      auto finish_time = std::chrono::system_clock::now();
      std::chrono::duration<double> elapsed = finish_time-start_time;
      log(*l_ptr,msg + " - done. [" + std::to_string(elapsed.count()) + "s]\n");
    }

  private:
    logger* l_ptr;
    std::string msg;
    time_type start_time;
};


} // std_e
