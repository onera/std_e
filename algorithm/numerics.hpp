#pragma once


namespace std_e {


/// Same as std::partial_sum, but with a given initial value, and one more output value
template<class InputIt, class OutputIt, class T> constexpr auto
partial_accumulate(InputIt first, InputIt last, OutputIt d_first, const T& init) -> OutputIt {
  // Precondition: 
  //  [first,last) valid range
  //  [d_first,d_first+n+1) valid range where n = last-first
  T acc = init;
  *d_first = acc;

  while (first != last) {
    acc += *first;
    *++d_first = acc;
    ++first;
  }
  return ++d_first;
}


template<class InputIt, class OutputIt, class T, class BinaryOperation> constexpr auto
partial_accumulate(InputIt first, InputIt last, OutputIt d_first, const T& init, BinaryOperation op) -> OutputIt {
  // Precondition: 
  //  [first,last) valid range
  //  [d_first,d_first+n+1) valid range where n = last-first
  T acc = init;
  *d_first = acc;

  while (first != last) {
    acc = op(acc, *first);
    *++d_first = acc;
    ++first;
  }
  return ++d_first;
}


} // std_e
