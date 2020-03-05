#pragma once


#include <iostream>
#include <chrono>
#include <ctime>
#include <string>


namespace std_e {


class time_logger {
  public:
    time_logger(std::string s)
        : msg(std::move(s))
    { 
        std::cout << now() << " - " << msg << "...\n";
    }

    ~time_logger() {
        std::cout << now() << " - " << msg << " - done.\n";
    }

  private:
  // member function
    auto now() const -> std::string {
      auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::string formatted_now = std::ctime(&t);
      formatted_now.pop_back(); // remove newline 
      return formatted_now;
    }
  // data member
    std::string msg;
};


} // std_e
