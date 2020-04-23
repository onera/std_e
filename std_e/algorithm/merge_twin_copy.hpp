#pragma once


#include <iterator>


namespace std_e {


// TODO remove Output_it parameters (can be inside converter functors)


template<class Fwd_it, class Output_it0, class Output_it1, class Bin_pred, class Unique_converter, class Twin_converter> auto
// requires Fwd_it is a forward iterator
// requires Output_it0, Output_it1 are output iterators
// requires Bin_pred(T,T) -> bool
//      where T == value_type(Fwd_it)
// requires Unique_converter(T) -> value_type(Output_it0)
// requires Twin_converter(T,T) -> value_type(Output_it1)
merge_twin_copy(Fwd_it first, Fwd_it last, Output_it0 d_first_unique, Output_it1 d_first_twins, Bin_pred p, Unique_converter un_conv, Twin_converter twin_conv) -> void {
  while (first!=last) {
    if (std::next(first)!=last && p(*first,*std::next(first))) {
      *d_first_twins = twin_conv(*first,*std::next(first));
      ++d_first_twins;
      std::advance(first,2);
    } else {
      *d_first_unique = un_conv(*first);
      ++d_first_unique;
      std::advance(first,1);
    }
  }
}


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


} // std_e
