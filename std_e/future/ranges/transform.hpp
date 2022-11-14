#pragma once


#include <ranges>
#include <functional>
#include "std_e/future/ranges/concept.hpp"
#include "std_e/future/ranges/view_interface.hpp"
#include "std_e/functional/pipeable.hpp"
#include "std_e/base/macros.hpp"


namespace std_e {
namespace ranges {


#if defined(REAL_GCC) && defined(__GLIBCXX__) && __cplusplus > 201703L
  namespace views {
    inline constexpr auto transform = std::views::transform;
  }
#else
  namespace detail {
    template<bool is_const, typename T>
      using maybe_const_t = std::conditional_t<is_const, const T, T>;
  } // namespace detail

  template<range _Range> requires std::is_object_v<_Range>
    class ref_view : public view_interface<ref_view<_Range>>
    {
    private:
      _Range* _M_r;

      static void _S_fun(_Range&); // not defined
      static void _S_fun(_Range&&) = delete;

    public:
      template</*detail::__different_from<ref_view>*/class _Tp>
        requires std::convertible_to<_Tp, _Range&>
          && requires { _S_fun(std::declval<_Tp>()); }
        constexpr
        ref_view(_Tp&& __t)
        noexcept(noexcept(static_cast<_Range&>(std::declval<_Tp>())))
          : _M_r(std::addressof(static_cast<_Range&>(std::forward<_Tp>(__t))))
        { }

      constexpr _Range&
      base() const
      { return *_M_r; }

      constexpr std::ranges::iterator_t<_Range>
      begin() const
      { return std::ranges::begin(*_M_r); }

      constexpr std::ranges::sentinel_t<_Range>
      end() const
      { return std::ranges::end(*_M_r); }

      //constexpr bool
      //empty() const requires requires { std::ranges::empty(*_M_r); }
      //{ return std::ranges::empty(*_M_r); }

      constexpr auto
      size() const requires sized_range<_Range>
      { return std::size(*_M_r); }

      constexpr auto
      data() const requires contiguous_range<_Range>
      { return std::data(*_M_r); }
    };

  template<typename _Range>
    ref_view(_Range&) -> ref_view<_Range>;

  //template<typename _Tp>
  //  inline constexpr bool enable_borrowed_range<ref_view<_Tp>> = true;

  template<range _Range>
    requires std::movable<_Range>
      //&& (!detail::__is_initializer_list<std::remove_cv_t<_Range>>)
    class owning_view : public view_interface<owning_view<_Range>>
    {
    private:
      _Range _M_r = _Range();

    public:
      owning_view() requires std::default_initializable<_Range> = default;

      constexpr
      owning_view(_Range&& __t)
      noexcept(std::is_nothrow_move_constructible_v<_Range>)
        : _M_r(std::move(__t))
      { }

      // TODO need that but not sure why the one above does not match (jagged_array)
      constexpr
      owning_view(auto __t)
      noexcept(std::is_nothrow_move_constructible_v<_Range>)
        : _M_r(std::move(__t))
      { }

      owning_view(owning_view&&) = default;
      owning_view& operator=(owning_view&&) = default;

      constexpr _Range&
      base() & noexcept
      { return _M_r; }

      constexpr const _Range&
      base() const& noexcept
      { return _M_r; }

      constexpr _Range&&
      base() && noexcept
      { return std::move(_M_r); }

      constexpr const _Range&&
      base() const&& noexcept
      { return std::move(_M_r); }

      constexpr std::ranges::iterator_t<_Range>
      begin()
      { return std::ranges::begin(_M_r); }

      constexpr std::ranges::sentinel_t<_Range>
      end()
      { return std::ranges::end(_M_r); }

      constexpr auto
      begin() const requires range<const _Range>
      { return std::ranges::begin(_M_r); }

      constexpr auto
      end() const requires range<const _Range>
      { return std::ranges::end(_M_r); }

      //constexpr bool
      //empty() requires requires { ranges::empty(_M_r); }
      //{ return std::ranges::empty(_M_r); }

      //constexpr bool
      //empty() const requires requires { ranges::empty(_M_r); }
      //{ return std::ranges::empty(_M_r); }

      constexpr auto
      size() requires sized_range<_Range>
      { return std::size(_M_r); }

      constexpr auto
      size() const requires sized_range<const _Range>
      { return std::size(_M_r); }

      constexpr auto
      data() requires contiguous_range<_Range>
      { return _M_r.data(); }

      constexpr auto
      data() const requires contiguous_range<const _Range>
      { return _M_r.data(); }
    };


  namespace detail {
      template<typename _Range>
        concept __can_ref_view = requires { ref_view{std::declval<_Range>()}; };

      template<typename _Range>
        concept __can_owning_view = requires { owning_view{std::declval<_Range>()}; };
  } // namespace detail



  struct _All
    {
      template<typename _Range>
        static constexpr bool
        _S_noexcept()
        {
          return true;
        }

      template</*viewable_range*/class _Range>
        //requires view<decay_t<_Range>>
        //  || detail::__can_ref_view<_Range>
        //  || detail::__can_owning_view<_Range>
        constexpr auto
        operator() [[nodiscard]] (_Range&& __r) const
        noexcept(_S_noexcept<_Range>())
        {
          if constexpr (view<std::decay_t<_Range>>)
            return std::forward<_Range>(__r);
          else if constexpr (detail::__can_ref_view<_Range>)
            return ref_view{std::forward<_Range>(__r)};
          else
            return owning_view<std::remove_cvref_t<_Range>>{std::move(__r)};
        }

      static constexpr bool _S_has_simple_call_op = true;
    };

    inline constexpr _All all;

    template</*viewable_range*/class _Range>
      using all_t = decltype(all(std::declval<_Range>()));

  template<input_range _Vp, /*copy_constructible*/ class F>
    requires view<_Vp> && std::is_object_v<F>
      && std::regular_invocable<F&, std::ranges::range_reference_t<_Vp>>
    //  && std::detail::__can_reference<invoke_result_t<F&,
                //                                        std::ranges::range_reference_t<_Vp>>>
    class transform_view : public view_interface<transform_view<_Vp, F>>
    {
    private:
      template<bool is_const>
        using _Base = detail::maybe_const_t<is_const, _Vp>;

      template<bool is_const>
        struct __iter_cat
        { };

      template<bool is_const>
        requires forward_range<_Base<is_const>>
        struct __iter_cat<is_const>
        {
        private:
          static auto
          _S_iter_cat()
          {
            using _Base = transform_view::_Base<is_const>;
            using _Res = std::invoke_result_t<F&, std::ranges::range_reference_t<_Base>>;
            if constexpr (std::is_lvalue_reference_v<_Res>)
              {
                using _Cat
                  = typename std::iterator_traits<std::ranges::iterator_t<_Base>>::iterator_category;
                if constexpr (std::derived_from<_Cat, std::contiguous_iterator_tag>)
                  return std::random_access_iterator_tag{};
                else
                  return _Cat{};
              }
            else
              return std::input_iterator_tag{};
          }
        public:
          using iterator_category = decltype(_S_iter_cat());
        };

      template<bool is_const>
        struct _Sentinel;

      template<bool is_const>
        struct _Iterator : __iter_cat<is_const>
        {
        private:
          using _Parent = detail::maybe_const_t<is_const, transform_view>;
          using _Base = transform_view::_Base<is_const>;

          static auto
          _S_iter_concept()
          {
            if constexpr (random_access_range<_Base>)
              return std::random_access_iterator_tag{};
            else if constexpr (bidirectional_range<_Base>)
              return std::bidirectional_iterator_tag{};
            else if constexpr (forward_range<_Base>)
              return std::forward_iterator_tag{};
            else
              return std::input_iterator_tag{};
          }

          using _Base_iter = std::ranges::iterator_t<_Base>;

          _Base_iter _M_current = _Base_iter();
          _Parent* _M_parent = nullptr;

        public:
          using iterator_concept = decltype(_S_iter_concept());
          // iterator_category defined in __transform_view_iter_cat
          using value_type
            = std::remove_cvref_t<std::invoke_result_t<F&, std::ranges::range_reference_t<_Base>>>;
          using difference_type = std::ranges::range_difference_t<_Base>;

          _Iterator() /*requires default_initializable<_Base_iter>*/ = default;

          constexpr
          _Iterator(_Parent* __parent, _Base_iter __current)
            : _M_current(std::move(__current)),
              _M_parent(__parent)
          { }

          constexpr
          _Iterator(_Iterator<!is_const> __i)
            requires is_const
              && std::convertible_to<std::ranges::iterator_t<_Vp>, _Base_iter>
            : _M_current(std::move(__i._M_current)), _M_parent(__i._M_parent)
          { }

          constexpr const _Base_iter&
          base() const & noexcept
          { return _M_current; }

          constexpr _Base_iter
          base() &&
          { return std::move(_M_current); }

          constexpr decltype(auto)
          operator*() const
            noexcept(noexcept(std::__invoke(_M_parent->_M_fun, *_M_current)))
          { return std::__invoke(_M_parent->_M_fun, *_M_current); }

          constexpr _Iterator&
          operator++()
          {
            ++_M_current;
            return *this;
          }

          constexpr void
          operator++(int)
          { ++_M_current; }

          constexpr _Iterator
          operator++(int) requires forward_range<_Base>
          {
            auto tmp = *this;
            ++*this;
            return tmp;
          }

          constexpr _Iterator&
          operator--() requires bidirectional_range<_Base>
          {
            --_M_current;
            return *this;
          }

          constexpr _Iterator
          operator--(int) requires bidirectional_range<_Base>
          {
            auto tmp = *this;
            --*this;
            return tmp;
          }

          constexpr _Iterator&
          operator+=(difference_type n) requires random_access_range<_Base>
          {
            _M_current += n;
            return *this;
          }

          constexpr _Iterator&
          operator-=(difference_type n) requires random_access_range<_Base>
          {
            _M_current -= n;
            return *this;
          }

          constexpr decltype(auto)
          operator[](difference_type n) const
            requires random_access_range<_Base>
          { return std::invoke(_M_parent->_M_fun, _M_current[n]); }
          constexpr decltype(auto)
          toto(difference_type n) const
            requires random_access_range<_Base>
          { return std::invoke(_M_parent->_M_fun, _M_current[n]); }

          friend constexpr bool
          operator==(const _Iterator& x, const _Iterator& y)
            //requires equality_comparable<_Base_iter>
          { return x._M_current == y._M_current; }

          friend constexpr bool
          operator<(const _Iterator& x, const _Iterator& y)
            requires random_access_range<_Base>
          { return x._M_current < y._M_current; }

          friend constexpr bool
          operator>(const _Iterator& x, const _Iterator& y)
            requires random_access_range<_Base>
          { return y < x; }

          friend constexpr bool
          operator<=(const _Iterator& x, const _Iterator& y)
            requires random_access_range<_Base>
          { return !(y < x); }

          friend constexpr bool
          operator>=(const _Iterator& x, const _Iterator& y)
            requires random_access_range<_Base>
          { return !(x < y); }

//#ifdef __cpp_lib_three_way_comparison
//          friend constexpr auto
//          operator<=>(const _Iterator& x, const _Iterator& y)
//            requires random_access_range<_Base>
//              && three_way_comparable<_Base_iter>
//          { return x._M_current <=> y._M_current; }
//#endif

          friend constexpr _Iterator
          operator+(_Iterator __i, difference_type n)
            requires random_access_range<_Base>
          { return {__i._M_parent, __i._M_current + n}; }

          friend constexpr _Iterator
          operator+(difference_type n, _Iterator __i)
            requires random_access_range<_Base>
          { return {__i._M_parent, __i._M_current + n}; }

          friend constexpr _Iterator
          operator-(_Iterator __i, difference_type n)
            requires random_access_range<_Base>
          { return {__i._M_parent, __i._M_current - n}; }

          // _GLIBCXX_RESOLVE_LIB_DEFECTS
          // 3483. transform_view::iterator's difference is overconstrained
          friend constexpr difference_type
          operator-(const _Iterator& x, const _Iterator& y)
            //requires sized_sentinel_for<std::ranges::iterator_t<_Base>, std::ranges::iterator_t<_Base>>
          { return x._M_current - y._M_current; }

          friend constexpr decltype(auto)
          iter_move(const _Iterator& __i) noexcept(noexcept(*__i))
          {
            if constexpr (std::is_lvalue_reference_v<decltype(*__i)>)
              return std::move(*__i);
            else
              return *__i;
          }

          friend _Iterator<!is_const>;
          template<bool> friend struct _Sentinel;
        };

      template<bool is_const>
        struct _Sentinel
        {
        private:
          using _Parent = detail::maybe_const_t<is_const, transform_view>;
          using _Base = transform_view::_Base<is_const>;

          template<bool _Const2>
            constexpr auto
            __distance_from(const _Iterator<_Const2>& __i) const
            { return _M_end - __i._M_current; }

          template<bool _Const2>
            constexpr bool
            __equal(const _Iterator<_Const2>& __i) const
            { return __i._M_current == _M_end; }

          std::ranges::sentinel_t<_Base> _M_end = std::ranges::sentinel_t<_Base>();

        public:
          _Sentinel() = default;

          constexpr explicit
          _Sentinel(std::ranges::sentinel_t<_Base> __end)
            : _M_end(__end)
          { }

          constexpr
          _Sentinel(_Sentinel<!is_const> __i)
            requires is_const
              && std::convertible_to<std::ranges::sentinel_t<_Vp>, std::ranges::sentinel_t<_Base>>
            : _M_end(std::move(__i._M_end))
          { }

          constexpr std::ranges::sentinel_t<_Base>
          base() const
          { return _M_end; }

          template<bool _Const2>
            requires std::sentinel_for<std::ranges::sentinel_t<_Base>,
                       std::ranges::iterator_t<detail::maybe_const_t<_Const2, _Vp>>>
            friend constexpr bool
            operator==(const _Iterator<_Const2>& x, const _Sentinel& y)
            { return y.__equal(x); }

          template<bool _Const2,
                   typename _Base2 = detail::maybe_const_t<_Const2, _Vp>>
            //requires sized_sentinel_for<std::ranges::sentinel_t<_Base>, std::ranges::iterator_t<_Base2>>
            friend constexpr std::ranges::range_difference_t<_Base2>
            operator-(const _Iterator<_Const2>& x, const _Sentinel& y)
            { return -y.__distance_from(x); }

          template<bool _Const2,
                   typename _Base2 = detail::maybe_const_t<_Const2, _Vp>>
            //requires sized_sentinel_for<std::ranges::sentinel_t<_Base>, std::ranges::iterator_t<_Base2>>
            friend constexpr std::ranges::range_difference_t<_Base2>
            operator-(const _Sentinel& y, const _Iterator<_Const2>& x)
            { return y.__distance_from(x); }

          friend _Sentinel<!is_const>;
        };

      _Vp _M_base = _Vp();
      [[no_unique_address]] F _M_fun;

    public:
      transform_view() requires (std::default_initializable<_Vp>
                                 && std::default_initializable<F>)
        = default;

      constexpr
      transform_view(_Vp __base, F fun)
        : _M_base(std::move(__base)), _M_fun(std::move(fun))
      { }

      constexpr _Vp
      base() const& //requires copy_constructible<_Vp>
      { return _M_base ; }

      constexpr _Vp
      base() &&
      { return std::move(_M_base); }

      constexpr _Iterator<false>
      begin()
      { return _Iterator<false>{this, std::ranges::begin(_M_base)}; }

      constexpr _Iterator<true>
      begin() const
        requires range<const _Vp>
          && std::regular_invocable<const F&, std::ranges::range_reference_t<const _Vp>>
      { return _Iterator<true>{this, std::ranges::begin(_M_base)}; }

      constexpr _Sentinel<false>
      end()
      { return _Sentinel<false>{std::ranges::end(_M_base)}; }

      constexpr _Iterator<false>
      end() requires common_range<_Vp>
      { return _Iterator<false>{this, std::ranges::end(_M_base)}; }

      constexpr _Sentinel<true>
      end() const
        requires range<const _Vp>
          && std::regular_invocable<const F&, std::ranges::range_reference_t<const _Vp>>
      { return _Sentinel<true>{std::ranges::end(_M_base)}; }

      constexpr _Iterator<true>
      end() const
        requires common_range<const _Vp>
          && std::regular_invocable<const F&, std::ranges::range_reference_t<const _Vp>>
      { return _Iterator<true>{this, std::ranges::end(_M_base)}; }

      constexpr auto
      size() requires sized_range<_Vp>
      { return std::size(_M_base); }

      constexpr auto
      size() const requires sized_range<const _Vp>
      { return std::size(_M_base); }
    };

  template<typename Range, typename F>
    transform_view(Range&&, F) -> transform_view<all_t<Range>, F>;

  namespace views
  {
    //namespace detail
    //{
    //  template<typename Range, typename F>
    //    concept __can_transform_view
    //      = requires { transform_view(std::declval<Range>(), std::declval<F>()); };
    //} // namespace detail

    struct Transform //: __adaptor::_RangeAdaptor<Transform>
    {
      //template<viewable_range Range, typename F>
      template</*viewable_range*/class Range, typename F>
        //requires detail::__can_transform_view<Range, F>
        static constexpr auto
        call_impl [[nodiscard]] (Range&& r, F&& f)
        {
          return transform_view(std::forward<Range>(r), std::forward<F>(f));
        }

      template<class Range, typename F> constexpr auto
        operator() [[nodiscard]] (Range&& r, F&& f) const {
          return call_impl(FWD(r),FWD(f));
        }

      template<typename F> constexpr auto
        operator() [[nodiscard]] (F&& f) const {
          auto fn = [f](auto&& r){ return call_impl(FWD(r),f); };
          return make_pipeable(fn);
        }

      //using _RangeAdaptor<Transform>::operator();
      static constexpr int _S_arity = 2;
      static constexpr bool _S_has_simple_extra_args = true;
    };

    inline constexpr Transform transform;
  } // namespace views
#endif


} // ranges


namespace views = ranges::views;


} // std_e
