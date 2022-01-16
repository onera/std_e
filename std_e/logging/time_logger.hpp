#pragma once


#include "std_e/logging/log.hpp"
#include "std_e/utils/timer.hpp"


namespace std_e {


class time_logger {
  public:
    time_logger(logger* l_ptr, std::string s)
      : l_ptr(l_ptr)
      , msg(std::move(s))
      , done(false)
    {
      log(*l_ptr,msg+"\n");
    }

    auto stop() -> void {
      log(*l_ptr,msg + " - done. [" + std::to_string(t.elapsed()) + "s]\n");
      done = true;
    }

    ~time_logger() {
      if (!done) {
        stop();
      }
    }

  private:
    logger* l_ptr;
    std::string msg;
    bool done;
    timer t;
};


inline auto
stdout_time_logger(const std::string& msg) -> time_logger {
  return time_logger(&get_logger("terminal"),msg);
}


} // std_e
