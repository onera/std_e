#pragma once


#include <vector>
#include <string>
#include "std_e/future/span.hpp"


// TODO factor with std_e/parallel/serialize_2
namespace std_e {


// with buffer provided {
inline auto
serialize_into_2(const std::string& x, std::vector<std::byte>& serial) -> size_t {
  size_t n_byte = x.size(); // since x::value_type==char and sizeof(char)==1
  auto first = reinterpret_cast<const std::byte*>(x.data());
  auto last  = first + n_byte;
  serial.insert(serial.end(), first, last);
  return n_byte;
}
inline auto
deserialize_into_2(std_e::span<const std::byte> serial, std::string& x) -> void {
  size_t n_char = serial.size(); // since x::value_type==char and sizeof(char)==1
  auto first = reinterpret_cast<const char*>(serial.data());
  auto last  = first + n_char;
  x.insert(x.end(), first, last);
}

template<class T>
  requires (!std::is_trivially_copyable_v<T>)
    auto
serialize_into_2(const std::vector<T>& x, std::vector<size_t>& x_szs, std::vector<std::byte>& x_serial) {
  auto previous_sz = x_szs.size();
  x_szs.resize( previous_sz + x.size() );
  for (size_t i=0; i<x.size(); ++i) {
    auto n_byte = serialize_into_2(x[i], x_serial);
    x_szs[previous_sz+i] = n_byte;
  }
}

template<class T>
  requires (!std::is_trivially_copyable_v<T>)
    auto
deserialize_into_2(const std::vector<size_t>& x_szs, const std::vector<std::byte>& x_serial, std::vector<T>& x) -> void {
  auto previous_sz = x.size();
  x.resize( previous_sz + x_szs.size() );
  size_t cnt = 0;
  for (size_t i=0; i<x_szs.size(); ++i) {
    size_t sz = x_szs[i];
    auto serial = std_e::make_span(x_serial.data()+cnt, x_serial.data()+cnt+sz);
    deserialize_into_2(serial, x[previous_sz+i]);
    cnt += sz;
  }
}
// with buffer provided }

// no buffer provided {
template<class T>
  requires (!std::is_trivially_copyable_v<T>)
    auto
serialize_2(const std::vector<T>& x) {
  std::vector<size_t> x_szs;
  std::vector<std::byte> x_serial;
  serialize_into_2(x, x_szs, x_serial);
  return std::make_pair(x_szs, x_serial);
}

template<class T>
    auto
deserialize_2(const std::vector<size_t>& x_szs, const std::vector<std::byte>& x_serial) {
  std::vector<std::string> res;
  deserialize_into_2(x_szs,x_serial,res);
  return res;
}
// no buffer provided }


} // std_e
