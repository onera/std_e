#pragma once


#include <iterator>


namespace std_e {


#if defined(REAL_GCC) && defined(__GLIBCXX__) && __cplusplus > 201703L
  template<class I> concept permutable = std::permutable<I>;

  template<class I, class Comp = std::ranges::less, class Proj = std::identity>
  concept sortable = std::sortable<I,Comp,Proj>;
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
