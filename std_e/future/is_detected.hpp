#pragma once

// from https://en.cppreference.com/w/cpp/experimental/is_detected
#include "std_e/future/contract.hpp"
#include <type_traits>

namespace std_e {

struct any_return_type {
  template<class T>
  operator T() {
    STD_E_ASSERT(false); // only here for compilation
    return T{};
  };
};

struct never_used_type {};
template<class T> using never_enable = std::enable_if_t<std::is_same_v<never_used_type,T>>;


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


} // std_e
