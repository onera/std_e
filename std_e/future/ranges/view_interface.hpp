#pragma once


#include <ranges>
#include "std_e/future/ranges/concept.hpp"
#include <concepts>


namespace std_e {
namespace ranges {

  template<typename _Derived>
    requires std::is_class_v<_Derived> && std::same_as<_Derived, std::remove_cv_t<_Derived>>
    class view_interface;

  namespace detail
  {
    template<typename _Tp, typename _Up>
      requires (!std::same_as<_Tp, view_interface<_Up>>)
      void __is_derived_from_view_interface_fn(const _Tp&,
					       const view_interface<_Up>&); // not defined

    // Returns true iff _Tp has exactly one public base class that's a
    // specialization of view_interface.
    template<typename _Tp>
      concept __is_derived_from_view_interface
	= requires (_Tp __t) { __is_derived_from_view_interface_fn(__t, __t); };
  } // detail


  namespace detail
  {
    template<typename _Range>
      concept __simple_view = view<_Range> && range<const _Range>
        && std::same_as<std::ranges::iterator_t<_Range>, std::ranges::iterator_t<const _Range>>
        && std::same_as<std::ranges::sentinel_t<_Range>, std::ranges::sentinel_t<const _Range>>;

    template<typename _It>
      concept __has_arrow = std::input_iterator<_It>
        && (std::is_pointer_v<_It> || requires(_It __it) { __it.operator->(); });

    template<typename _Tp, typename _Up>
      concept __different_from
        = !std::same_as<std::remove_cvref_t<_Tp>, std::remove_cvref_t<_Up>>;
  } // namespace detail

  /// The ranges::view_interface class template
  template<typename _Derived>
    requires std::is_class_v<_Derived> && std::same_as<_Derived, std::remove_cv_t<_Derived>>
    class view_interface
    {
    private:
      constexpr _Derived& _M_derived() noexcept
      {
        static_assert(std::derived_from<_Derived, view_interface<_Derived>>);
        static_assert(view<_Derived>);
        return static_cast<_Derived&>(*this);
      }

      constexpr const _Derived& _M_derived() const noexcept
      {
        static_assert(std::derived_from<_Derived, view_interface<_Derived>>);
        static_assert(view<_Derived>);
        return static_cast<const _Derived&>(*this);
      }

      static constexpr bool
      _S_bool(bool) noexcept; // not defined

      template<typename _Tp>
        static constexpr bool
        _S_empty(_Tp& __t)
        noexcept(noexcept(_S_bool(std::ranges::begin(__t) == std::ranges::end(__t))))
        { return std::ranges::begin(__t) == std::ranges::end(__t); }

      template<typename _Tp>
        static constexpr auto
        _S_size(_Tp& __t)
        noexcept(noexcept(std::ranges::end(__t) - std::ranges::begin(__t)))
        { return std::ranges::end(__t) - std::ranges::begin(__t); }

    public:
      constexpr bool
      empty()
      noexcept(noexcept(_S_empty(_M_derived())))
      //requires forward_range<_Derived>
      { return _S_empty(_M_derived()); }

      constexpr bool
      empty() const
      noexcept(noexcept(_S_empty(_M_derived())))
      //requires forward_range<const _Derived>
      { return _S_empty(_M_derived()); }

      //constexpr explicit
      //operator bool() noexcept(noexcept(std::ranges::empty(_M_derived())))
      //requires requires { std::ranges::empty(_M_derived()); }
      //{ return !std::ranges::empty(_M_derived()); }

      //constexpr explicit
      //operator bool() const noexcept(noexcept(std::ranges::empty(_M_derived())))
      //requires requires { std::ranges::empty(_M_derived()); }
      //{ return !std::ranges::empty(_M_derived()); }

      constexpr auto
      data() noexcept(noexcept(std::ranges::begin(_M_derived())))
      //requires std::contiguous_iterator<std::ranges::iterator_t<_Derived>>
      { return std::to_address(std::ranges::begin(_M_derived())); }

      constexpr auto
      data() const noexcept(noexcept(std::ranges::begin(_M_derived())))
      //requires range<const _Derived>
        //&& std::contiguous_iterator<std::ranges::iterator_t<const _Derived>>
      { return std::to_address(std::ranges::begin(_M_derived())); }

      constexpr auto
      size() noexcept(noexcept(_S_size(_M_derived())))
      //requires forward_range<_Derived>
        //&& std::sized_sentinel_for<std::ranges::sentinel_t<_Derived>, std::ranges::iterator_t<_Derived>>
      { return _S_size(_M_derived()); }

      constexpr auto
      size() const noexcept(noexcept(_S_size(_M_derived())))
      //requires forward_range<const _Derived>
        //&& std::sized_sentinel_for<std::ranges::sentinel_t<const _Derived>,
        //                      std::ranges::iterator_t<const _Derived>>
      { return _S_size(_M_derived()); }

      constexpr decltype(auto)
      front() //requires forward_range<_Derived>
      {
        __glibcxx_assert(!empty());
        return *std::ranges::begin(_M_derived());
      }

      constexpr decltype(auto)
      front() const //requires forward_range<const _Derived>
      {
        __glibcxx_assert(!empty());
        return *std::ranges::begin(_M_derived());
      }

      constexpr decltype(auto)
      back()
      //requires bidirectional_range<_Derived> && common_range<_Derived>
      {
        __glibcxx_assert(!empty());
        return *std_e::ranges::prev(std::ranges::end(_M_derived()));
      }

      constexpr decltype(auto)
      back() const
      //requires bidirectional_range<const _Derived>
      //  && common_range<const _Derived>
      {
        __glibcxx_assert(!empty());
        return *std_e::ranges::prev(std::ranges::end(_M_derived()));
      }

      template<class Int,/*random_access_range*/class _Range = _Derived>
        constexpr decltype(auto)
        //operator[](std::ranges::range_difference_t<_Range> __n)
        operator[](Int __n)
        { return std::ranges::begin(_M_derived())[__n]; }

      template<class Int,/*random_access_range*/class _Range = const _Derived>
        constexpr decltype(auto)
        //operator[](std::ranges::range_difference_t<_Range> __n) const
        operator[](Int __n) const
        { return std::ranges::begin(_M_derived())[__n]; }
    };


} // ranges
} // std_e
