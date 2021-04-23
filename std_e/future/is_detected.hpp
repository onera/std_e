#pragma once

// from https://en.cppreference.com/w/cpp/experimental/is_detected
#include "std_e/future/contract.hpp"
#include <type_traits>

namespace std_e {

struct any_return_type {
  template<class T>
  operator T() {
    STD_E_ASSERT(false); // only here for compilation
  };
};
struct any_return_type2 {
  template<class T>
  operator T() {
    STD_E_ASSERT(false); // only here for compilation
  };
};

struct nonesuch {
  ~nonesuch() = delete;
  nonesuch(nonesuch const&) = delete;
  void operator=(nonesuch const&) = delete;
};

namespace detail {
  template <class Default, class AlwaysVoid,
            template<class...> class Op, class... Args>
  struct detector {
    using value_t = std::false_type;
    using type = Default;
  };

  template <class Default, template<class...> class Op, class... Args>
  struct detector<Default, std::void_t<Op<Args...>>, Op, Args...> {
    using value_t = std::true_type;
    using type = Op<Args...>;
  };
} // namespace detail

template <template<class...> class Op, class... Args>
using is_detected = typename detail::detector<nonesuch, void, Op, Args...>::value_t;

template <template<class...> class Op, class... Args>
constexpr bool is_detected_v = is_detected<Op,Args...>::value;

template <template<class...> class Op, class... Args>
using detected_t = typename detail::detector<nonesuch, void, Op, Args...>::type;

template <class Default, template<class...> class Op, class... Args>
using detected_or = detail::detector<Default, void, Op, Args...>;


namespace detail {
  template <class Default, class AlwaysVoid, class Op>
  struct detector2 {
    using value_t = std::false_type;
    using type = Default;
  };

  template <class Default, class Op>
  struct detector2<Default, std::void_t<Op>, Op> {
    using value_t = std::true_type;
    using type = Op;
  };
} // namespace detail

template <class Op>
using is_detected2 = typename detail::detector2<nonesuch, void, Op>::value_t;

template <class Op>
constexpr bool is_detected_v2 = is_detected2<Op>::value;

} // std_e
