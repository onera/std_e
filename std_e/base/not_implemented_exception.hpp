#pragma once


#include <exception>
#include <string>


namespace std_e {


class not_implemented_exception : public std::exception {
  public:
    not_implemented_exception() = default;
    
    not_implemented_exception(std::string desc)
      : desc(std::move(desc))
    {}

    const char* what() const noexcept override {
      return desc.c_str();
    }
  private:
    std::string desc;
};


} // std_e
