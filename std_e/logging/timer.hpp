#pragma once


#include <chrono>


namespace std_e {


class timer {
  public:
    using time_type = std::chrono::time_point<std::chrono::system_clock>;
    timer()
      : start_time(std::chrono::system_clock::now())
    {}

    auto elapsed() -> void {
      auto finish_time = std::chrono::system_clock::now();
      return (finish_time-start_time).count();
    }

  private:
    time_type start_time;
};


} // std_e
