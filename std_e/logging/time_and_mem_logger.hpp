#pragma once


#include "std_e/logging/log.hpp"
#include "std_e/base/system/timer.hpp"
#include "std_e/base/system/mem_usage.hpp"
#include "std_e/utils/pretty_print.hpp"


namespace std_e {


// TODO factor with time_logger and mem_logger
class time_and_mem_logger {
  public:
    time_and_mem_logger(std::string logger_name, std::string s)
      : name(std::move(logger_name))
      , msg(std::move(s))
      , done(false)
      // note: the timer t is started here by default construction
      , rss_mem_init(resident_memory_in_bytes())
    {
      log(name,msg+"\n");
    }

    auto stop() -> void {
      long rss_mem_now = resident_memory_in_bytes();
      log(name,msg + " - done. [" + std::to_string(t.elapsed()) + "s, " + pretty_print_bytes(rss_mem_now-rss_mem_init) + "]\n");
      done = true;
    }

    ~time_and_mem_logger() {
      if (!done) {
        stop();
      }
    }

  private:
    std::string name;
    std::string msg;
    bool done;
    timer t;
    long rss_mem_init;
};


} // std_e
