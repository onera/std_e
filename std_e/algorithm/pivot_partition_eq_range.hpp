#pragma once


#include <algorithm>
#include "std_e/interval/interval_sequence.hpp"
#include "std_e/utils/functional.hpp"


namespace std_e {


// SEE also partition_3way
template <class I, class T, class Comp = std::less<>, class Proj = identity_closure> constexpr auto
// requires I is Bidirectional Iterator
// requires value_type<I> == T
// requires Comp =  function(T,T)
pivot_partition_eq(I f, I l, const T& piv, Comp comp = {}, Proj proj = {}) -> std::pair<I,I> {
  using std::swap;
  I n = f;
  while (n != l) {
    if (comp(proj(*n),piv)) swap(*f++,*n++);
    else if (comp(piv,proj(*n))) swap(*--l, *n);
    else ++n;
  }
  return std::make_pair(f, l);
}


template<class RA_it, class RA_it_piv, class RA_it_out, class Comp, class Proj> constexpr auto
pivot_partition_eq_i_aux(RA_it first, RA_it last, RA_it_piv piv_first, RA_it_piv piv_last, RA_it_out out_first, Comp comp, Proj proj, RA_it start) -> void {
  if (piv_first==piv_last) return;
  auto k = piv_last-piv_first;
  auto piv_mid = piv_first+k/2;
  auto pi_mid_inf = out_first+2*(k/2);
  auto pi_mid_sup = out_first+2*(k/2)+1;
  auto [pp_mid_inf,pp_mid_sup] = pivot_partition_eq(first,last,*piv_mid,comp,proj);
  *pi_mid_inf = pp_mid_inf - start;
  *pi_mid_sup = pp_mid_sup - start;
  if (k>1) {
    pivot_partition_eq_i_aux(first     ,pp_mid_inf,    piv_first,piv_mid ,    out_first   ,    comp,proj, start);
    pivot_partition_eq_i_aux(pp_mid_sup,last      ,    piv_mid+1,piv_last,    pi_mid_inf+2,    comp,proj, start);
  }
}

template<class RA_it, class RA_it_piv, class RA_it_out, class Comp = std::less<>, class Proj = identity_closure> constexpr auto
pivot_partition_eq_indices(RA_it first, RA_it last,
                           RA_it_piv piv_first, RA_it_piv piv_last,
                           RA_it_out out_first,
                           Comp comp = {},
                           Proj proj = {}) -> void {
  return pivot_partition_eq_i_aux(first,last,
                                  piv_first,piv_last,
                                  out_first,
                                  comp,proj,
                                  first);
}

template<
  class RA_rng, class RA_pivot_rng,
  class Comp = std::less<>,
  class Proj = identity_closure,
  class Return_container = interval_vector<int>
> constexpr auto
pivot_partition_eq_indices(RA_rng& x, const RA_pivot_rng& pivots, Comp comp = {}, Proj proj = {}, Return_container = {}) -> Return_container {
  int n = x.size();
  int k = pivots.size();
  Return_container partition_is(1+2*k); // add begin and end for convenience
  partition_is[0] = 0;
  pivot_partition_eq_indices(begin(x),end(x),
                             begin(pivots),end(pivots),
                             begin(partition_is)+1,
                             comp,proj);
  partition_is.back() = n;
  return partition_is;
}


} // std_e
