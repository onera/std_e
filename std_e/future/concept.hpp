#pragma once


#include <iterator>


namespace std_e {


#if defined(__GLIBCXX__) && __cplusplus > 201703L
  template<class I>
  concept sortable = std::sortable;
#else
  template<class I>
  concept permutable =
      std::forward_iterator<I> &&
      std::indirectly_movable_storable<I, I> &&
      std::indirectly_swappable<I, I>;

  template<class I, class Comp = std::less<>, class Proj = std::identity>
  concept sortable =
      permutable<I> &&
      std::indirect_strict_weak_order<Comp, std::projected<I, Proj>>;
#endif


} // std_e
