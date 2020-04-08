#pragma once


#include <type_traits>
#include <utility>
#include <exception>
#include <array>
#include "std_e/utils/index_sequence.hpp"
#include "std_e/future/make_array.hpp"


namespace std_e {


class case_is_not_available_error : std::exception {};


namespace detail {

template<class F, class... Args>
struct function_wrapper {
  template<class integer_constant_type>
  struct tagged_caller {
    static auto call(F&& f, Args&&... args) {
      return f(integer_constant_type(),std::forward<Args>(args)...);
    }
  };
};

} // detail


template<class int_seq_type>
struct switch_;

template<class Integer, Integer... Is>
struct switch_<std::integer_sequence<Integer,Is...>> {
  public:
  // type traits
    static constexpr std::integer_sequence<Integer,Is...> dispatching_indices = {};

    template<class fun_wrap_type, Integer I> using tagged_caller = 
      typename fun_wrap_type::template tagged_caller<std::integral_constant<Integer,I>>;
  // ctor
    constexpr
    switch_(Integer i)
      : i(i)
    {
      if (!(contains(i,dispatching_indices))) { throw case_is_not_available_error(); }
    }

    template<class F, class... Args> constexpr auto
    apply(F&& f, Args&&... args) {
      using function_wrapper_type = detail::function_wrapper<F,Args...>;

      auto lookup_table = make_array( &tagged_caller<function_wrapper_type,Is>::call... );

      auto index_of_i = position_in_seq(i,dispatching_indices);
      return lookup_table[index_of_i](std::forward<F>(f),std::forward<Args>(args)...);
    }
  private:
    Integer i;
};


} // std_e
