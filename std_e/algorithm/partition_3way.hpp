#pragma once


#include <utility>


namespace std_e {


// From Stepanov & Marcus:
//   Notes for the Foundations of Programming course at Adobe
//     Part 3: Partition algorithms
template <class I, class F> constexpr auto
// requires I is Bidirectional Iterator
// requires F is Unary Function
partition_3way(I f, I l, F f) -> std::pair<I,I> {
  using std::swap;
  I n = f;
  while (n != l) {
    int key = f(*n);
    if (key == 0) swap(*f++,*n++);
    else if (key == 2) swap(*--l, *n);
    else ++n;
  }
  return std::make_pair(f, l);
}


} // std_e
