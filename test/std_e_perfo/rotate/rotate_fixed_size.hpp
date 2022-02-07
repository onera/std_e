#include <algorithm>
#include "std_e/future/span.hpp"

template<class I> auto
rotate_min_first_handwritten_ptr(I* x) -> void {
  auto min_pos = std::min_element(x,x+3);
  int i = min_pos-x;
  int v0 = x[i];
  int v1 = x[(i+1)%3];
  int v2 = x[(i+2)%3];
  x[0] = v0;
  x[1] = v1;
  x[2] = v2;
}
template<class I> auto
rotate_min_first_handwritten_ptr_int64(I* x) -> void {
  auto min_pos = std::min_element(x,x+3);
  auto i = min_pos-x; static_assert(std::is_same_v<decltype(i),std::ptrdiff_t>);
  int v0 = x[i];
  int v1 = x[(i+1)%3];
  int v2 = x[(i+2)%3];
  x[0] = v0;
  x[1] = v1;
  x[2] = v2;
}
template<class Fixed_size_range> auto
rotate_min_first_handwritten(Fixed_size_range& x) -> void {
  constexpr int N = Fixed_size_range::extent;
  static_assert(N==3);
  auto min_pos = std::min_element(begin(x),end(x));
  int i = min_pos-begin(x);
  int v0 = x[i];
  int v1 = x[(i+1)%3];
  int v2 = x[(i+2)%3];
  x[0] = v0;
  x[1] = v1;
  x[2] = v2;
}
template<class Fixed_size_range> auto
rotate_min_first_handwritten_int64(Fixed_size_range& x) -> void {
  constexpr int N = Fixed_size_range::extent;
  static_assert(N==3);
  auto min_pos = std::min_element(begin(x),end(x));
  auto i = min_pos-begin(x); static_assert(std::is_same_v<decltype(i),std::ptrdiff_t>);
  int v0 = x[i];
  int v1 = x[(i+1)%3];
  int v2 = x[(i+2)%3];
  x[0] = v0;
  x[1] = v1;
  x[2] = v2;
}
template<class Fixed_size_range> auto
rotate_min_first_handwritten_range(Fixed_size_range& x) -> void {
  constexpr int N = Fixed_size_range::extent;
  static_assert(N==3);
  auto min_pos = std::ranges::min_element(x);
  int i = min_pos-begin(x);
  int v0 = x[i];
  int v1 = x[(i+1)%3];
  int v2 = x[(i+2)%3];
  x[0] = v0;
  x[1] = v1;
  x[2] = v2;
}

template<class Fixed_size_range> auto
rotate_min_first_array(Fixed_size_range& x) -> void {
  using I = typename Fixed_size_range::value_type;
  constexpr int N = Fixed_size_range::extent;

  auto min_pos = std::ranges::min_element(x);
  int i = min_pos-begin(x);

  std::array<I,N> rotated_x;
  rotated_x[0] = x[(i+0)%3];
  rotated_x[1] = x[(i+1)%3];
  rotated_x[2] = x[(i+2)%3];
  std::copy_n(begin(rotated_x),N,begin(x));
}

template<class Fixed_size_range> auto
rotate_min_first_copy_n(Fixed_size_range& x) -> void {
  using I = typename Fixed_size_range::value_type;
  constexpr int N = Fixed_size_range::extent;

  auto min_pos = std::ranges::min_element(x);
  int i = min_pos-begin(x);
  int j = N-i;

  std::array<I,N> rotated_x;
  std::copy_n(min_pos,j,begin(rotated_x));
  std::copy_n(begin(x),i,begin(rotated_x)+j);
  std::copy_n(begin(rotated_x),N,begin(x));
}

template<class Fixed_size_range> auto
rotate_min_first_rotate_copy(Fixed_size_range& x) -> void {
  using I = typename Fixed_size_range::value_type;
  constexpr int N = Fixed_size_range::extent;

  auto min_pos = std::min_element(x.data(),x.data()+N);

  std::array<I,N> rotated_x;
  std::rotate_copy(x.data(),min_pos,x.data()+N,rotated_x.data());
  std::copy_n(rotated_x.data(),N,x.data());
}
template<class Fixed_size_range> auto
rotate_min_first_rotate_copy_range(Fixed_size_range& x) -> void {
  using I = typename Fixed_size_range::value_type;
  constexpr int N = Fixed_size_range::extent;

  auto min_pos = std::ranges::min_element(x);

  std::array<I,N> rotated_x;
  std::ranges::rotate_copy(x,min_pos,begin(rotated_x));
  std::ranges::copy(rotated_x,begin(x));
}

template<class Fixed_size_range> auto
rotate_min_first_rotate(Fixed_size_range& x) -> void {
  constexpr int N = Fixed_size_range::extent;
  auto min_pos = std::min_element(x.data(),x.data()+N);
  std::rotate(x.data(),min_pos,x.data()+N);
}
template<class Fixed_size_range> auto
rotate_min_first_rotate_range(Fixed_size_range& x) -> void {
  auto min_pos = std::ranges::min_element(x);
  std::ranges::rotate(x,min_pos);
}
