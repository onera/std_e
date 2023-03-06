#pragma once


#include "std_e/logging/log.hpp"
#include "std_e/base/system/timer.hpp"


namespace std_e {


class time_logger {
  public:
    time_logger(std::string logger_name, std::string s)
      : name(std::move(logger_name))
      , msg(std::move(s))
      , done(false)
      // note: the timer t is started here by default construction
    {
      log(name,msg+"\n");
    }

    auto stop() -> void {
      log(name,msg + " - done. [" + std::to_string(t.elapsed()) + "s]\n");
      done = true;
    }

    ~time_logger() {
      if (!done) {
        stop();
      }
    }

  private:
    std::string name;
    std::string msg;
    bool done;
    timer t;
};


inline auto
stdout_time_logger(const std::string& msg) -> time_logger {
  return time_logger("std_e",msg);
}


} // std_e
