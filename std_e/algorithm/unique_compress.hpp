#pragma once


#include <iterator>
#include <vector>


namespace std_e {


template<class Fwd_it, class S, class F> constexpr auto
// requires F(Fwd_it,S)->Fwd_it
// requires Sentinel<Fwd_it>==S
unique_compress(Fwd_it first, S last, F compress_while_eq) -> Fwd_it {
  Fwd_it result = first;
  Fwd_it next = compress_while_eq(first,last);
  while (next!=last && next==std::next(first)) {
    first = next;
    next = compress_while_eq(first,last);
    ++result;
  }
  while (next!=last) {
    first = next;
    next = compress_while_eq(first,last);
    ++result;
    *result = std::move(*first);
  }
  return ++result;
}
template<class Fwd_it, class S, class Output_it, class F> constexpr auto
// requires F(Fwd_it,S)->Fwd_it
// requires Sentinel<Fwd_it>==S
unique_compress_copy(Fwd_it first, S last, Output_it result, F compress_while_eq) -> Output_it {
  if (first==last) return result;

  *result = *first;
  Fwd_it next = compress_while_eq(first,last);
  while (next!=last && next==std::next(first)) {
    first = next;
    next = compress_while_eq(first,last);
    *result=*first;
    ++result;
  }
  while (next!=last) {
    first = next;
    next = compress_while_eq(first,last);
    ++result;
    *result = *first;
  }
  return ++result;
}

template<class Fwd_it, class S, class Output_it0, class I, class Output_it1, class F> constexpr auto
// requires F(Fwd_it,S)->Fwd_it
// requires Sentinel<Fwd_it>==S
// requires I is integer and I==Output_it1::value_type
unique_compress_copy_with_index_position(
    Fwd_it first, S last,
    Output_it0 result,
    I current_new_position,
    Output_it1 old_to_new_positions,
    F compress_while_eq
) -> Output_it0
{
  auto f = [&current_new_position,old_to_new_positions,compress_while_eq](auto f, auto l){
    auto next = compress_while_eq(f,l);
    auto n = next-f;
    std::fill_n(old_to_new_positions,n,current_new_position);
    ++current_new_position;
    return next;
  };
  return unique_compress_copy(first,last,result,f);
}


// TODO generalize int*, see if can be formulated as other of unique_compress family
template<
  class Fwd_it, class S, class Fwd_it2, class F,
  class T = decltype(*Fwd_it{}),
  std::enable_if_t<std::is_invocable_v<F,T>,int> =0
> constexpr auto
unique_compress_strides_copy(Fwd_it first, S last, Fwd_it2 d_first, F reduction, const int* strides) -> Fwd_it2 {
  while (first != last) {
    *d_first = reduction(*first++);
    int count = *strides++;
    while (--count) {
      *d_first = reduction(*d_first,*first++);
    }
    ++d_first;
  }
  return d_first;
}
template<
  class Fwd_it, class S, class Fwd_it2, class F,
  class T = decltype(*Fwd_it{}),
  std::enable_if_t<!std::is_invocable_v<F,T>,int> =0
> constexpr auto
unique_compress_strides_copy(Fwd_it first, S last, Fwd_it2 d_first, F reduction, const int* strides) -> Fwd_it2 {
  while (first != last) {
    *d_first = *first++;
    int count = *strides++;
    while (--count) {
      *d_first = reduction(*d_first,*first++);
    }
    ++d_first;
  }
  return d_first;
}


// std::vector interface {
template<class T, class A, class F> constexpr auto
unique_compress(std::vector<T,A>& v, F compress_while_eq) -> void {
  auto new_end = unique_compress(begin(v),end(v),compress_while_eq);
  v.erase(new_end,end(v));
}
template<class T, class A, class F> constexpr auto
unique_compress_copy(std::vector<T,A>& v, F compress_while_eq) -> std::vector<T> {
  std::vector<T> res;
  unique_compress_copy(begin(v),end(v),back_inserter(res),compress_while_eq);
  return res;
}
template<class I, class T, class A, class F> constexpr auto
unique_compress_copy_with_index_position(std::vector<T,A>& v, F compress_while_eq) -> std::pair<std::vector<T>,std::vector<I>> {
  std::vector<T> compress_res;
  std::vector<I> position_res;
  I position_init = 0;
  unique_compress_copy_with_index_position(
    begin(v),end(v),
    back_inserter(compress_res),
    position_init,back_inserter(position_res),
    compress_while_eq
  );
  return std::make_pair(compress_res,position_res);
}
// std::vector interface }


} // std_e
