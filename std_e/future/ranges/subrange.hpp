#pragma once

#include <ranges>
#include "std_e/future/ranges/concept.hpp"
#include "std_e/future/ranges/view_interface.hpp"


namespace std_e {
namespace ranges {

#if defined(REAL_GCC) && defined(__GLIBCXX__) && __cplusplus > 201703L
  template<
    std::input_or_output_iterator I,
    std::sentinel_for<I> S = I,
    std::ranges::subrange_kind K = std::sized_sentinel_for<S, I> ?
      std::ranges::subrange_kind::sized : std::ranges::subrange_kind::unsized
  >
  using subrange = std::ranges::subrange<I,S,K>;
#else
namespace detail {

    template<typename _From, typename _To>
      concept __uses_nonqualification_pointer_conversion
  = std::is_pointer_v<_From> && std::is_pointer_v<_To>
  ;
    //&& !std::convertible_to<remove_pointer_t<_From>(*)[],
    //       std::remove_pointer_t<_To>(*)[]>;

    template<typename _From, typename _To>
      concept __convertible_to_non_slicing = std::convertible_to<_From, _To>
  && !__uses_nonqualification_pointer_conversion<std::decay_t<_From>,
                   std::decay_t<_To>>;
} // detail

enum class subrange_kind : bool { unsized, sized };

/// The ranges::subrange class template
template<std::input_or_output_iterator It, std::sentinel_for<It> S = It,
         subrange_kind Kind = std::sized_sentinel_for<S, It>
           ? subrange_kind::sized : subrange_kind::unsized>
  requires (Kind == subrange_kind::sized || !std::sized_sentinel_for<S, It>)
  class subrange : public view_interface<subrange<It, S, Kind>>
  {
  private:
    static constexpr bool _S_store_size
      = Kind == subrange_kind::sized && !std::sized_sentinel_for<S, It>;

    It _M_begin = It();
    [[no_unique_address]] S _M_end = S();

    using __size_type = std::size_t;

    template<typename, bool = _S_store_size>
      struct _Size
      { };

    template<typename _Tp>
      struct _Size<_Tp, true>
      { _Tp _M_size; };

    [[no_unique_address]] _Size<__size_type> _M_size = {};

  public:
    subrange() requires std::default_initializable<It> = default;

    constexpr
    subrange(detail::__convertible_to_non_slicing<It> auto __i, S __s)
    noexcept(std::is_nothrow_constructible_v<It, decltype(__i)>
             && std::is_nothrow_constructible_v<S, S&>)
      requires (!_S_store_size)
    : _M_begin(std::move(__i)), _M_end(__s)
    { }

    constexpr
    subrange(detail::__convertible_to_non_slicing<It> auto __i, S __s,
             __size_type __n)
    noexcept(std::is_nothrow_constructible_v<It, decltype(__i)>
             && std::is_nothrow_constructible_v<S, S&>)
      requires (Kind == subrange_kind::sized)
    : _M_begin(std::move(__i)), _M_end(__s)
    {
      if constexpr (_S_store_size)
        _M_size._M_size = __n;
    }

    template<detail::__different_from<subrange> _Rng>
      requires borrowed_range<_Rng>
        && detail::__convertible_to_non_slicing<std::ranges::iterator_t<_Rng>, It>
        && std::convertible_to<std::ranges::sentinel_t<_Rng>, S>
      constexpr
      subrange(_Rng&& __r)
      noexcept(noexcept(subrange(__r, std::size(__r))))
      requires _S_store_size && sized_range<_Rng>
      : subrange(__r, std::size(__r))
      { }

    template<detail::__different_from<subrange> _Rng>
      requires borrowed_range<_Rng>
        && detail::__convertible_to_non_slicing<std::ranges::iterator_t<_Rng>, It>
        && std::convertible_to<std::ranges::sentinel_t<_Rng>, S>
      constexpr
      subrange(_Rng&& __r)
      noexcept(noexcept(subrange(std::ranges::begin(__r), std::ranges::end(__r))))
      requires (!_S_store_size)
      : subrange(std::ranges::begin(__r), std::ranges::end(__r))
      { }

    template<borrowed_range _Rng>
      requires detail::__convertible_to_non_slicing<std::ranges::iterator_t<_Rng>, It>
        && std::convertible_to<std::ranges::sentinel_t<_Rng>, S>
      constexpr
      subrange(_Rng&& __r, __size_type __n)
      noexcept(noexcept(subrange(std::ranges::begin(__r), std::ranges::end(__r), __n)))
      requires (Kind == subrange_kind::sized)
      : subrange{std::ranges::begin(__r), std::ranges::end(__r), __n}
      { }

    template<detail::__different_from<subrange> _PairLike>
      //requires detail::__pair_like_convertible_from<_PairLike, const It&,
      //                                                const S&>
      constexpr
      operator _PairLike() const
      { return _PairLike(_M_begin, _M_end); }

    constexpr It
    begin() const requires std::copyable<It>
    { return _M_begin; }

    [[nodiscard]] constexpr It
    begin() requires (!std::copyable<It>)
    { return std::move(_M_begin); }

    constexpr S end() const { return _M_end; }

    constexpr bool empty() const { return _M_begin == _M_end; }

    constexpr __size_type
    size() const requires (Kind == subrange_kind::sized)
    {
      if constexpr (_S_store_size)
        return _M_size._M_size;
      else
        //return detail::__to_unsigned_like(_M_end - _M_begin);
        return _M_end - _M_begin;
    }

    [[nodiscard]] constexpr subrange
    next(std::iter_difference_t<It> __n = 1) const &
      requires std::forward_iterator<It>
    {
      auto __tmp = *this;
      __tmp.advance(__n);
      return __tmp;
    }

    [[nodiscard]] constexpr subrange
    next(std::iter_difference_t<It> __n = 1) &&
    {
      advance(__n);
      return std::move(*this);
    }

    [[nodiscard]] constexpr subrange
    prev(std::iter_difference_t<It> __n = 1) const
      requires std::bidirectional_iterator<It>
    {
      auto __tmp = *this;
      __tmp.advance(-__n);
      return __tmp;
    }

    //constexpr subrange&
    //advance(std::iter_difference_t<It> __n)
    //{
    //  // _GLIBCXX_RESOLVE_LIB_DEFECTS
    //  // 3433. subrange::advance(n) has UB when n < 0
    //  if constexpr (std::bidirectional_iterator<It>)
    //    if (__n < 0)
    //      {
    //        std::ranges::advance(_M_begin, __n);
    //        if constexpr (_S_store_size)
    //          _M_size._M_size += detail::__to_unsigned_like(-__n);
    //        return *this;
    //      }

    //  __glibcxx_assert(__n >= 0);
    //  auto __d = __n - std::ranges::advance(_M_begin, __n, _M_end);
    //  if constexpr (_S_store_size)
    //    _M_size._M_size -= detail::__to_unsigned_like(__d);
    //  return *this;
    //}
  };

template<std::input_or_output_iterator It, std::sentinel_for<It> S>
  subrange(It, S) -> subrange<It, S>;

//template<std::input_or_output_iterator It, std::ranges::sentinel_for<It> S>
//  subrange(It, S,
//           detail::__make_unsigned_like_t<std::iter_difference_t<It>>)
//    -> subrange<It, S, subrange_kind::sized>;

template<borrowed_range _Rng>
  subrange(_Rng&&)
    -> subrange<std::ranges::iterator_t<_Rng>, std::ranges::sentinel_t<_Rng>,
               (sized_range<_Rng>
                || std::sized_sentinel_for<std::ranges::sentinel_t<_Rng>, std::ranges::iterator_t<_Rng>>)
               ? subrange_kind::sized : subrange_kind::unsized>;

template<borrowed_range _Rng>
  subrange(_Rng&&,
           size_t)
    -> subrange<std::ranges::iterator_t<_Rng>, std::ranges::sentinel_t<_Rng>, subrange_kind::sized>;

template<size_t _Num, class It, class S, subrange_kind Kind>
  requires (_Num < 2)
  constexpr auto
  get(const subrange<It, S, Kind>& __r)
  {
    if constexpr (_Num == 0)
      return __r.begin();
    else
      return __r.end();
  }

template<size_t _Num, class It, class S, subrange_kind Kind>
  requires (_Num < 2)
  constexpr auto
  get(subrange<It, S, Kind>&& __r)
  {
    if constexpr (_Num == 0)
      return __r.begin();
    else
      return __r.end();
  }

//template<typename It, typename S, subrange_kind Kind>
//  inline constexpr bool
//    enable_borrowed_range<subrange<It, S, Kind>> = true;
//
//template<range _Range>
//  using borrowed_subrange_t = __conditional_t<borrowed_range<_Range>,
//                                              subrange<std::ranges::iterator_t<_Range>>,
//                                              dangling>;
#endif
} // ranges
} // std_e
