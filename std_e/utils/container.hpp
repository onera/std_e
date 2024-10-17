#pragma once


#include <algorithm>


namespace std_e {


template<class Rng, class Comp_pred = std::less<>> constexpr auto
sort(Rng& x, Comp_pred cmp = {}) -> void {
  std::sort(x.begin(),x.end(),cmp);
}
template<class Rng, class Comp_pred = std::less<>> constexpr auto
stable_sort(Rng& x, Comp_pred cmp = {}) -> void {
  std::stable_sort(x.begin(),x.end(),cmp);
}

template<class Container, class Equiv_pred = std::equal_to<>> constexpr auto
unique(Container& x, Equiv_pred eq = {}) -> void {
  auto new_end = std::unique(x.begin(),x.end(), eq);
  //auto new_sz = x.end() - new_end;
  x.erase(new_end,x.end());
}

template<class Container, class Equiv_pred = std::equal_to<>, class Comp_pred = std::less<>> constexpr auto
sort_unique(Container& x, Equiv_pred eq = {}, Comp_pred cmp = {}) -> void {
  sort(x,cmp);
  unique(x,eq);
}
template<class Container, class Equiv_pred = std::equal_to<>, class Comp_pred = std::less<>> constexpr auto
stable_sort_unique(Container& x, Equiv_pred eq = {}, Comp_pred cmp = {}) -> void {
  stable_sort(x,cmp);
  unique(x,eq);
}


template<class Container, class Unary_pred> auto
remove_if(Container& x, Unary_pred&& p) -> void {
  auto new_end = std::remove_if(x.begin(), x.end(), p);
  auto new_sz = new_end - x.begin();
  x.resize(new_sz);
}


} // std_e
