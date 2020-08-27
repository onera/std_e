#pragma once


#include <ostream>


namespace std_e {


// https://stackoverflow.com/a/11826666/1583122
struct null_buffer : std::streambuf {
  auto overflow(int c) -> int override { return c; }
};

class null_stream : public std::ostream {
  public:
    null_stream()
      : std::ostream(&null_buf)
    {}
  private:
    null_buffer null_buf;
};


} // std_e
