#pragma once


#include <exception>
#include <string>


namespace std_e {


class msg_exception : public std::exception {
  public:
    msg_exception() = default;
    
    msg_exception(std::string desc)
      : desc(std::move(desc))
    {}

    const char* what() const noexcept override {
      return desc.c_str();
    }
  private:
    std::string desc;
};


} // std_e
