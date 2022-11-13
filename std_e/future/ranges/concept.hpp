#pragma once


#include <ranges>


namespace std_e {
namespace ranges {


#if defined(__GLIBCXX__) && __cplusplus > 201703L
  template<class T> concept range = std::ranges::range;
  template<class T> concept input_range = std::ranges::input_range;
  template<class T> concept forward_range = std::ranges::forward_range;
  template<class T> concept bidirectional_range = std::ranges::bidirectional_range;
  template<class T> concept random_access_range = std::ranges::random_access_range;
  template<class T> concept contiguous_range = std::ranges::contiguous_range;

  template<class T> concept sized_range = std::ranges::sized_range;
  template<class T> concept common_range = std::ranges::common_range;

  using less = std::ranges::less;


  inline constexpr auto next = std::ranges::next;
#else
  template<class T>
  concept range =
    requires(T& t) {
      { std::ranges::begin(t) };
      { std::ranges::end(t) };
    };

  template<class T>
  concept input_range = range<T> && std::input_iterator<std::ranges::iterator_t<T>>;

  template<class T>
  concept forward_range = input_range<T>;

  template<class T>
  concept bidirectional_range = forward_range<T> && std::bidirectional_iterator<std::ranges::iterator_t<T>>;

  template< class T >
  concept random_access_range =
    bidirectional_range<T> && std::random_access_iterator<std::ranges::iterator_t<T>>;

  template< class T >
  concept contiguous_range =
    random_access_range<T> &&
    std::contiguous_iterator<std::ranges::iterator_t<T>> &&
    requires(T& t) {
      { t.data() } ->
        std::same_as<std::add_pointer_t<std::ranges::range_reference_t<T>>>;
    };


  template<class T>
  concept sized_range =
    requires(T& t) {
      { begin(t) };
      { end(t) };
      { t.size() };
    };

  template<class T>
  concept common_range = range<T> &&
    std::same_as<std::ranges::iterator_t<T>, std::ranges::sentinel_t<T>>;

  using less = std::less<void>;




  struct advance_fn {
    template<std::input_or_output_iterator I>
    constexpr void operator()(I& i, std::iter_difference_t<I> n) const
    {
      if constexpr (std::random_access_iterator<I>) {
          i += n;
      }
      else {
          while (n > 0) {
              --n;
              ++i;
          }
          if constexpr (std::bidirectional_iterator<I>) {
              while (n < 0) {
                  ++n;
                  --i;
              }
          }
      }
    }
   
    template<std::input_or_output_iterator I, std::sentinel_for<I> S>
    constexpr void operator()(I& i, S bound) const
    {
      if constexpr (std::assignable_from<I&, S>) {
          i = std::move(bound);
      }
      else if constexpr (std::sized_sentinel_for<S, I>) {
          (*this)(i, bound - i);
      }
      else {
          while (i != bound) {
              ++i;
          }
      }
    }
   
    template<std::input_or_output_iterator I, std::sentinel_for<I> S>
    constexpr std::iter_difference_t<I>
    operator()(I& i, std::iter_difference_t<I> n, S bound) const
    {
      if constexpr (std::sized_sentinel_for<S, I>) {
          // std::abs isn't constexpr until C++23
          auto abs = [](const std::iter_difference_t<I> x) { return x < 0 ? -x : x; };
   
          const auto dist = abs(n) - abs(bound - i);
          if (dist < 0) {
              (*this)(i, bound);
              return -dist;
          }
   
          (*this)(i, n);
          return 0;
      }
      else {
          while (n > 0 && i != bound) {
              --n;
              ++i;
          }
   
          if constexpr (std::bidirectional_iterator<I>) {
              while (n < 0 && i != bound) {
                  ++n;
                  --i;
              }
          }
   
          return n;
      }
    }
  };
 
  inline constexpr auto advance = advance_fn();


  struct prev_fn {
    template<std::bidirectional_iterator I>
    constexpr I operator()(I i) const {
      --i;
      return i;
    }
  };

  struct next_fn {
    template<std::input_or_output_iterator I>
    constexpr I operator()(I i) const {
      ++i;
      return i;
    }

    template<std::input_or_output_iterator I>
    constexpr I operator()(I i, std::iter_difference_t<I> n) const {
      std_e::ranges::advance(i, n);
      return i;
    }
   
    template<std::input_or_output_iterator I, std::sentinel_for<I> S>
    constexpr I operator()(I i, S bound) const {
      std_e::ranges::advance(i, bound);
      return i;
    }
   
    template<std::input_or_output_iterator I, std::sentinel_for<I> S>
    constexpr I operator()(I i, std::iter_difference_t<I> n, S bound) const {
      std_e::ranges::advance(i, n, bound);
      return i;
    }
  };
 
  inline constexpr auto prev = prev_fn();
  inline constexpr auto next = next_fn();

  template<typename _Derived>
    requires std::is_class_v<_Derived> && std::same_as<_Derived, std::remove_cv_t<_Derived>>
    class view_interface;

  template<typename _Tp>
    inline constexpr bool enable_view = std::derived_from<_Tp, std::ranges::view_base>
      || std::derived_from<_Tp, view_interface<_Tp>>;

  template<class T>
  concept view = range<T> && std::movable<T> && std_e::ranges::enable_view<T>;

  template<class R>
  concept borrowed_range =
      range<R> &&
      (std::is_lvalue_reference_v<R> ||
       std::ranges::enable_borrowed_range<std::remove_cvref_t<R>>);

  template<class T>
  concept viewable_range =
    range<T> &&
    ((view<std::remove_cvref_t<T>> &&
      std::constructible_from<std::remove_cvref_t<T>, T>) ||
     (!view<std::remove_cvref_t<T>> &&
      (std::is_lvalue_reference_v<T> ||
       (std::movable<std::remove_reference_t<T>>))));
#endif


} // ranges
} // std_e
