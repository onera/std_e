#pragma once


#include "std_e/functional/pipeable.hpp"
#include "std_e/future/ranges.hpp"
#include "std_e/meta/meta.hpp"
#include "std_e/utils/tuple.hpp"


// Efficient iteration over a concatenation of ranges needs *internal iteration*
// See e.g. https://youtu.be/95uT0RhMGwA?t=3728 (at the end of the talk)
// This means that `concat_view` can't be not a normal range (it does not have begin/end)
// Here, we only implement:
//   - std_e::views::concat(Rng...) to create a range concatenation
//        note: Rng can be 
//            - a std::ranges::range (i.e. with begin/end)
//            - another `concat_view` (i.e. we can nest `concat_views`)
//   - operator|(concat_view , to_vector)
namespace std_e {
namespace views {


template<class... Ts>
class concat_view;

// extended_range traits{
/// vocabulary : an extended range is either a std::ranges::range or a `concat_view`
template<class T>
struct extended_range_value {
  using type = std::ranges::range_value_t<T>;
};
template<class T, class... Ts>
struct extended_range_value<concat_view<T,Ts...>> {
  using type = typename extended_range_value<std::remove_cvref_t<T>>::type;
};
template<class T>
using extended_range_value_t = typename extended_range_value<std::remove_cvref_t<T>>::type;
// extended_range traits }


template<class... Ts>
class concat_view {
  private:
    std::tuple<std_e::remove_rvalue_reference<Ts>...> xs;
  public:
    static constexpr int n_range = sizeof...(Ts);
    concat_view(Ts&&... xs)
      : xs(FWD(xs)...)
    {}

    auto
    size() const {
      size_t sz = 0;
      accumulate(xs,sz,[](size_t i, const auto& x){ return i+x.size(); });
      return sz;
    }

    template<int I> auto get() const -> const auto& { return std::get<I>(xs); }
    template<int I> auto get()       ->       auto& { return std::get<I>(xs); }
};



template<int I, class... Ts> auto
get(const concat_view<Ts...>& x) -> const auto& {
  return x.template get<I>();
}
template<int I, class... Ts> auto
get(concat_view<Ts...>& x) -> auto& {
  return x.template get<I>();
}


template<class... Ts> auto
concat(Ts&&... xs) {
  return concat_view<Ts&&...>(FWD(xs)...);
}


template<std::ranges::range Rng, class It, size_t... Is> auto
concat_copy_impl(const Rng& x, It& first) -> void {
  first = std::ranges::copy(x,first).out;
}
// specialization handling the `concat_view<concat_view<Ts>>` case
template<class... Ts, class It, size_t... Is> auto
concat_copy_impl(const concat_view<Ts...>& x, It& first) -> void {
  concat_copy(x,first);
  first += x.size();
}

template<class... Ts, class It, size_t... Is> auto
concat_copy_index_seq(const concat_view<Ts...>& x, It first, std::index_sequence<Is...>) -> void {
  ( concat_copy_impl(get<Is>(x),first) , ... );
}

template<class... Ts, class It> auto
concat_copy(const concat_view<Ts...>& x, It first) -> void {
  constexpr int n_range = concat_view<Ts...>::n_range;
  concat_copy_index_seq(x,first,std::make_index_sequence<n_range>());
}

template<class... Ts> auto
concat_pipe_impl(const concat_view<Ts...>& x) {
  using T = extended_range_value_t<concat_view<Ts...>>;
  std::vector<T> res(x.size());
  concat_copy(x,begin(res));
  return res;
}

// Specialization of `concat_view | to_vector()`
// because concat_view is not a range (no begin/end)
// Generally `operator|` will not work with `concat_view`
// But we provide this specialization for genericity
template<class... Ts> auto
operator|(const concat_view<Ts...>& x, pipeable_wrapper<to_vector_closure>&&) {
  return concat_pipe_impl(x);
}
template<class... Ts> auto
operator|(concat_view<Ts...>& x, pipeable_wrapper<to_vector_closure>&&) {
  return concat_pipe_impl(x);
}
template<class... Ts> auto
operator|(concat_view<Ts...>&& x, pipeable_wrapper<to_vector_closure>&&) {
  return concat_pipe_impl(std::move(x));
}

} // views
} // std_e
