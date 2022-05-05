#pragma once


#include "std_e/functional/pipeable.hpp"
#include "std_e/future/ranges.hpp"
#include "std_e/meta/meta.hpp"
#include "std_e/meta/pack.hpp"
#include "std_e/utils/tuple.hpp"


namespace std_e {

template<class... Ts>
class concat_view {
  private:
    std::tuple<std_e::remove_rvalue_reference<Ts>...> xs;
    using T = std_e::first_of_pack<Ts...>;
  public:
    static constexpr int n_range = sizeof...(Ts);
    using value_type = typename std::remove_cvref_t<T>::value_type;
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
  first = std::copy(begin(x),end(x),first);
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
  using T = typename concat_view<Ts...>::value_type;
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

} // std_e
