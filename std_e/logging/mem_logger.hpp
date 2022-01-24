#pragma once


#include "std_e/logging/log.hpp"
#include "std_e/base/system/mem_usage.hpp"
#include "std_e/utils/pretty_print.hpp"


namespace std_e {


class mem_logger {
  public:
    mem_logger(logger* l_ptr, std::string s)
      : l_ptr(l_ptr)
      , msg(std::move(s))
      , done(false)
      , rss_mem_init(resident_memory_in_bytes())
    {
      log(*l_ptr,msg+"\n");
    }

    auto stop() -> void {
      long rss_mem_now = resident_memory_in_bytes();
      log(*l_ptr,msg + " - done. [" + pretty_print_bytes(rss_mem_now-rss_mem_init) + "]\n");
      done = true;
    }

    ~mem_logger() {
      if (!done) {
        stop();
      }
    }

  private:
    logger* l_ptr;
    std::string msg;
    bool done;
    long rss_mem_init;
};


inline auto
stdout_mem_logger(const std::string& msg) -> mem_logger {
  return mem_logger(&get_logger("terminal"),msg);
}


} // std_e
