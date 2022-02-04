#pragma once


#include <exception>
#include <string>


namespace std_e {


class msg_exception : public std::exception {
  public:
    msg_exception() = default;

    msg_exception(std::string msg)
      : msg(std::move(msg))
    {}

    const char* what() const noexcept override {
      return msg.c_str();
    }
  private:
    std::string msg;
};


} // std_e
