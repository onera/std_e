#pragma once

#include <vector>
#include <array>
#include "std_e/future/array.hpp"


namespace std_e {


// impl {
/// Note: We basically assume that vector<array<T,N>> can be cast to vector<T>
///       This is not guaranteed by C++ (see https://stackoverflow.com/q/71229120/1583122)
template<class T> auto
flat_data_impl(auto& x) -> T* {
  return x.data()->data();
}
template<class T, size_t N> auto
flat_view_impl(auto& x) {
  T* first = flat_data_impl<T>(x);
  auto sz = x.size() * N;
  return std_e::span<T>(first,sz);
}
// impl }


// flat_data {
template<class T, size_t N> auto
flat_data(std::vector<std::array<T,N>>& x) -> T* {
  return flat_data_impl<T>(x);
}
template<class T, size_t N> auto
flat_data(const std::vector<std::array<T,N>>& x) -> const T* {
  return flat_data_impl<const T>(x);
}
template<class T, ptrdiff_t N> auto
flat_data(std::vector<std_e::array<T,N>>& x) -> T* {
  return flat_data_impl<T>(x);
}
template<class T, ptrdiff_t N> auto
flat_data(const std::vector<std_e::array<T,N>>& x) -> const T* {
  return flat_data_impl<const T>(x);
}
// flat_data }


// flat_view {
template<class T, size_t N> auto
flat_view(std::vector<std::array<T,N>>& x) -> std_e::span<T> {
  return flat_view_impl<T,N>(x);
}
template<class T, size_t N> auto
flat_view(const std::vector<std::array<T,N>>& x) -> std_e::span<const T> {
  return flat_view_impl<const T,N>(x);
}
template<class T, ptrdiff_t N> auto
flat_view(std::vector<std_e::array<T,N>>& x) -> std_e::span<T> {
  return flat_view_impl<T,N>(x);
}
template<class T, ptrdiff_t N> auto
flat_view(const std::vector<std_e::array<T,N>>& x) -> std_e::span<const T> {
  return flat_view_impl<const T,N>(x);
}
// flat_view }


} // std_e
