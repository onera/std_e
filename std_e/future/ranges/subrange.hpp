#pragma once

#include <ranges>
#include "std_e/future/ranges/concept.hpp"
#include "std_e/future/ranges/view_interface.hpp"


namespace std_e {
namespace ranges {


namespace detail {

    template<typename _From, typename _To>
      concept __uses_nonqualification_pointer_conversion
	= std::is_pointer_v<_From> && std::is_pointer_v<_To>
  ;
	  //&& !std::convertible_to<remove_pointer_t<_From>(*)[],
		//	     std::remove_pointer_t<_To>(*)[]>;

    template<typename _From, typename _To>
      concept __convertible_to_non_slicing = std::convertible_to<_From, _To>
	&& !__uses_nonqualification_pointer_conversion<std::decay_t<_From>,
						       std::decay_t<_To>>;
} // detail

enum class subrange_kind : bool { unsized, sized };

/// The ranges::subrange class template
template<std::input_or_output_iterator _It, std::sentinel_for<_It> _Sent = _It,
         subrange_kind _Kind = std::sized_sentinel_for<_Sent, _It>
           ? subrange_kind::sized : subrange_kind::unsized>
  requires (_Kind == subrange_kind::sized || !std::sized_sentinel_for<_Sent, _It>)
  class subrange : public view_interface<subrange<_It, _Sent, _Kind>>
  {
  private:
    static constexpr bool _S_store_size
      = _Kind == subrange_kind::sized && !std::sized_sentinel_for<_Sent, _It>;

    _It _M_begin = _It();
    [[no_unique_address]] _Sent _M_end = _Sent();

    using __size_type = std::size_t;

    template<typename, bool = _S_store_size>
      struct _Size
      { };

    template<typename _Tp>
      struct _Size<_Tp, true>
      { _Tp _M_size; };

    [[no_unique_address]] _Size<__size_type> _M_size = {};

  public:
    subrange() requires std::default_initializable<_It> = default;

    constexpr
    subrange(detail::__convertible_to_non_slicing<_It> auto __i, _Sent __s)
    noexcept(std::is_nothrow_constructible_v<_It, decltype(__i)>
             && std::is_nothrow_constructible_v<_Sent, _Sent&>)
      requires (!_S_store_size)
    : _M_begin(std::move(__i)), _M_end(__s)
    { }

    constexpr
    subrange(detail::__convertible_to_non_slicing<_It> auto __i, _Sent __s,
             __size_type __n)
    noexcept(std::is_nothrow_constructible_v<_It, decltype(__i)>
             && std::is_nothrow_constructible_v<_Sent, _Sent&>)
      requires (_Kind == subrange_kind::sized)
    : _M_begin(std::move(__i)), _M_end(__s)
    {
      if constexpr (_S_store_size)
        _M_size._M_size = __n;
    }

    template<detail::__different_from<subrange> _Rng>
      requires borrowed_range<_Rng>
        && detail::__convertible_to_non_slicing<std::ranges::iterator_t<_Rng>, _It>
        && std::convertible_to<std::ranges::sentinel_t<_Rng>, _Sent>
      constexpr
      subrange(_Rng&& __r)
      noexcept(noexcept(subrange(__r, std::size(__r))))
      requires _S_store_size && sized_range<_Rng>
      : subrange(__r, std::size(__r))
      { }

    template<detail::__different_from<subrange> _Rng>
      requires borrowed_range<_Rng>
        && detail::__convertible_to_non_slicing<std::ranges::iterator_t<_Rng>, _It>
        && std::convertible_to<std::ranges::sentinel_t<_Rng>, _Sent>
      constexpr
      subrange(_Rng&& __r)
      noexcept(noexcept(subrange(std::ranges::begin(__r), std::ranges::end(__r))))
      requires (!_S_store_size)
      : subrange(std::ranges::begin(__r), std::ranges::end(__r))
      { }

    template<borrowed_range _Rng>
      requires detail::__convertible_to_non_slicing<std::ranges::iterator_t<_Rng>, _It>
        && std::convertible_to<std::ranges::sentinel_t<_Rng>, _Sent>
      constexpr
      subrange(_Rng&& __r, __size_type __n)
      noexcept(noexcept(subrange(std::ranges::begin(__r), std::ranges::end(__r), __n)))
      requires (_Kind == subrange_kind::sized)
      : subrange{std::ranges::begin(__r), std::ranges::end(__r), __n}
      { }

    template<detail::__different_from<subrange> _PairLike>
      //requires detail::__pair_like_convertible_from<_PairLike, const _It&,
      //                                                const _Sent&>
      constexpr
      operator _PairLike() const
      { return _PairLike(_M_begin, _M_end); }

    constexpr _It
    begin() const requires std::copyable<_It>
    { return _M_begin; }

    [[nodiscard]] constexpr _It
    begin() requires (!std::copyable<_It>)
    { return std::move(_M_begin); }

    constexpr _Sent end() const { return _M_end; }

    constexpr bool empty() const { return _M_begin == _M_end; }

    constexpr __size_type
    size() const requires (_Kind == subrange_kind::sized)
    {
      if constexpr (_S_store_size)
        return _M_size._M_size;
      else
        //return detail::__to_unsigned_like(_M_end - _M_begin);
        return _M_end - _M_begin;
    }

    [[nodiscard]] constexpr subrange
    next(std::iter_difference_t<_It> __n = 1) const &
      requires std::forward_iterator<_It>
    {
      auto __tmp = *this;
      __tmp.advance(__n);
      return __tmp;
    }

    [[nodiscard]] constexpr subrange
    next(std::iter_difference_t<_It> __n = 1) &&
    {
      advance(__n);
      return std::move(*this);
    }

    [[nodiscard]] constexpr subrange
    prev(std::iter_difference_t<_It> __n = 1) const
      requires std::bidirectional_iterator<_It>
    {
      auto __tmp = *this;
      __tmp.advance(-__n);
      return __tmp;
    }

    //constexpr subrange&
    //advance(std::iter_difference_t<_It> __n)
    //{
    //  // _GLIBCXX_RESOLVE_LIB_DEFECTS
    //  // 3433. subrange::advance(n) has UB when n < 0
    //  if constexpr (std::bidirectional_iterator<_It>)
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

template<std::input_or_output_iterator _It, std::sentinel_for<_It> _Sent>
  subrange(_It, _Sent) -> subrange<_It, _Sent>;

//template<std::input_or_output_iterator _It, std::ranges::sentinel_for<_It> _Sent>
//  subrange(_It, _Sent,
//           detail::__make_unsigned_like_t<std::iter_difference_t<_It>>)
//    -> subrange<_It, _Sent, subrange_kind::sized>;

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

template<size_t _Num, class _It, class _Sent, subrange_kind _Kind>
  requires (_Num < 2)
  constexpr auto
  get(const subrange<_It, _Sent, _Kind>& __r)
  {
    if constexpr (_Num == 0)
      return __r.begin();
    else
      return __r.end();
  }

template<size_t _Num, class _It, class _Sent, subrange_kind _Kind>
  requires (_Num < 2)
  constexpr auto
  get(subrange<_It, _Sent, _Kind>&& __r)
  {
    if constexpr (_Num == 0)
      return __r.begin();
    else
      return __r.end();
  }

//template<typename _It, typename _Sent, subrange_kind _Kind>
//  inline constexpr bool
//    enable_borrowed_range<subrange<_It, _Sent, _Kind>> = true;
//
//template<range _Range>
//  using borrowed_subrange_t = __conditional_t<borrowed_range<_Range>,
//                                              subrange<std::ranges::iterator_t<_Range>>,
//                                              dangling>;
} // ranges
} // std_e
