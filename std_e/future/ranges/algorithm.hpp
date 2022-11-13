#pragma once

#include <algorithm>
#include "std_e/future/ranges/subrange.hpp"


namespace std_e {
namespace ranges {

template<class It0, class It1>
struct copy_result {
  It0 in;
  It1 out;
};

template<class R, class It> auto
copy(R&& r, It&& it) {
  #if defined(__GLIBCXX__)
    return std::ranges::copy(r,it);
  #else
    auto out = std::copy(std::ranges::begin(r),std::ranges::end(r),it);
    auto in  = std::ranges::end(r);
    using It0 = decltype(in);
    using It1 = decltype(out);
    return copy_result<It0,It1>{in,out};
  #endif
}
auto
move(auto&& r, auto&& it) {
  #if defined(__GLIBCXX__)
    return std::ranges::move(r,it);
  #else
    return std::move(begin(r),end(r),it);
  #endif
}

auto
find(auto&& r, auto&& val) {
  #if defined(__GLIBCXX__)
    return std::ranges::find(r,val);
  #else
    return std::find(begin(r),end(r),val);
  #endif
}
auto
find_if(auto&& r, auto&& pred) {
  #if defined(__GLIBCXX__)
    return std::ranges::find_if(r,pred);
  #else
    return std::find_if(begin(r),end(r),pred);
  #endif
}

auto
any_of(auto&& r, auto&& pred) {
  #if defined(__GLIBCXX__)
    return std::ranges::any_of(r,pred);
  #else
    return std::any_of(begin(r),end(r),pred);
  #endif
}
auto
partition_point(auto&& r, auto&& pred) {
  #if defined(__GLIBCXX__)
    return std::ranges::partition_point(r,pred);
  #else
    return std::partition_point(begin(r),end(r),pred);
  #endif
}
auto
lower_bound(auto&& r, auto&& pred) {
  #if defined(__GLIBCXX__)
    return std::ranges::lower_bound(r,pred);
  #else
    return std::lower_bound(begin(r),end(r),pred);
  #endif
}

auto
min_element(auto&& r) {
  #if defined(__GLIBCXX__)
    return std::ranges::min_element(r);
  #else
    return std::min_element(begin(r),end(r));
  #endif
}
auto
max_element(auto&& r) {
  #if defined(__GLIBCXX__)
    return std::ranges::max_element(r);
  #else
    return std::max_element(begin(r),end(r));
  #endif
}

auto
rotate(auto&& r, auto&& mid) {
  #if defined(__GLIBCXX__)
    return std::ranges::rotate(r,mid);
  #else
    return std::rotate(begin(r),mid,end(r));
  #endif
}
auto
partition(auto&& r, auto&& comp) {
  #if defined(__GLIBCXX__)
    return std::ranges::partition(r,comp);
  #else
    return std::partition(begin(r),end(r),comp);
  #endif
}
auto
reverse(auto&& r) {
  #if defined(__GLIBCXX__)
    return std::ranges::reverse(r);
  #else
    return std::reverse(begin(r),end(r));
  #endif
}
auto
stable_sort(auto&& r, auto&& comp, auto&& proj) {
  #if defined(__GLIBCXX__)
    return std::ranges::stable_sort(r,comm,proj);
  #else
    auto cmp = [comp,proj](const auto& x, const auto& y){ return comp(proj(x),proj(y)); };
    return std::reverse(begin(r),end(r),cmp);
  #endif
}

#if defined(__GLIBCXX__)
  inline constexpr auto unique = std::ranges::unique;
#else
  struct __adjacent_find_fn
  {
    template<std::forward_iterator _Iter, std::sentinel_for<_Iter> _Sent,
             typename _Proj = std::identity,
             std::indirect_binary_predicate<std::projected<_Iter, _Proj>,
                                       std::projected<_Iter, _Proj>> _Pred
               = std::equal_to<>>
      constexpr _Iter
      operator()(_Iter __first, _Sent __last,
                 _Pred __pred = {}, _Proj __proj = {}) const
      {
        if (__first == __last)
          return __first;
        auto __next = __first;
        for (; ++__next != __last; __first = __next)
          {
            if (std::invoke(__pred,
                              std::invoke(__proj, *__first),
                              std::invoke(__proj, *__next)))
              return __first;
          }
        return __next;
      }

    template<forward_range _Range, typename _Proj = std::identity,
             std::indirect_binary_predicate<
               std::projected<std::ranges::iterator_t<_Range>, _Proj>,
               std::projected<std::ranges::iterator_t<_Range>, _Proj>> _Pred = std::equal_to<>>
      constexpr std::ranges::borrowed_iterator_t<_Range>
      operator()(_Range&& __r, _Pred __pred = {}, _Proj __proj = {}) const
      {
        return (*this)(std::ranges::begin(__r), std::ranges::end(__r),
                       std::move(__pred), std::move(__proj));
      }
  };

  inline constexpr __adjacent_find_fn adjacent_find{};

  struct unique_fn {
    template</*std::permutable*/class I, std::sentinel_for<I> S, class Proj = std::identity,
             std::indirect_equivalence_relation<std::projected<I, Proj>> C = std::equal_to<>>
      constexpr subrange<I>
        operator() ( I first, S last, C comp = {}, Proj proj = {} ) const {
            first = adjacent_find(first, last, comp, proj);
            if (first == last)
                return {first, first};
            auto i {first};
            ++first;
            while (++first != last)
                if (!std::invoke(comp, std::invoke(proj, *i), std::invoke(proj, *first)))
                    *++i = std::ranges::iter_move(first);
            return {++i, first};
        }
   
    template<forward_range R, class Proj = std::identity,
             std::indirect_equivalence_relation<std::projected<std::ranges::iterator_t<R>, Proj>>
               C = std::equal_to<>>
      //requires std::permutable<std::ranges::iterator_t<R>>
        //constexpr borrowed_subrange_t<R>
        constexpr auto
          operator() ( R&& r, C comp = {}, Proj proj = {} ) const {
              return (*this)(std::ranges::begin(r), std::ranges::end(r),
                             std::move(comp), std::move(proj));
          }
  };
   
  inline constexpr unique_fn unique{};
#endif


} // ranges
} // std_e

