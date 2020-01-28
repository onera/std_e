#pragma once

#include <tuple>
#include "std_e/base/macros.hpp"


namespace std_e {


template<class... Ts>
class hierarchical_zip {
  public:
  // traits
    using impl_type = std::tuple<Ts...>;
  // ctor
    template<class... Ts0>
    hierarchical_zip(Ts0&&... xs)
      : _impl{FWD(xs)...}
    {}

  // low-level access
    constexpr auto
    impl() -> impl_type& {
      return _impl;
    }
    constexpr auto
    impl() const -> const impl_type& {
      return _impl;
    }

  // size
    static constexpr auto
    size() -> size_t {
      return sizeof...(Ts);
    }
  private:
    impl_type _impl;
};
template<class T, class... Ts> constexpr auto
get(hierarchical_zip<Ts...>& x) -> T& {
  return std::get<T>(x.impl());
}
template<class T, class... Ts> constexpr auto
get(const hierarchical_zip<Ts...>& x) -> const T& {
  return std::get<T>(x.impl());
}
template<size_t I, class... Ts> constexpr auto
get(hierarchical_zip<Ts...>& x) -> auto& {
  return std::get<I>(x.impl());
}
template<size_t I, class... Ts> constexpr auto
get(const hierarchical_zip<Ts...>& x) -> const auto& {
  return std::get<I>(x.impl());
}


template<class... Ts> constexpr auto
hierarchical_zip_view(Ts&... xs) {
  return hierarchical_zip<Ts&...>(xs...);
}
template<class... Ts> constexpr auto
hierarchical_zip_const_view(const Ts&... xs) {
  return hierarchical_zip<const Ts&...>(xs...);
}


template<class... Ts, class Projection> constexpr auto
zip_projection(hierarchical_zip<Ts...>& hzip, Projection proj) {
  return hierarchical_zip_view( proj(get<Ts>(hzip.impl())) ... ); // TODO get<Ts> -> get<I> (here, if several times same type: WRONG behavior)
}

//template<int I, class hiera_zip_of_tuple_type, class F>  constexpr auto
//for_each_until__impl_hiera_zip(hiera_zip_of_tuple_type&& x, F f) -> int {
//  constexpr int sz = hiera_zip_of_tuple_type::size();
//  if constexpr (I<sz) {
//    if (f(std::get<I>(get<0>(x.impl())),std::get<I>(get<1>(x.imp())))) {
//      return I;
//    } else {
//      return for_each_until__impl_hiera_zip<I+1>(x,f);
//    }
//  }
//  return sz;
//}
//
//
//template<class hiera_zip_of_hvector_type, class Unary_pred, class F> constexpr auto
//find_apply__impl4(hiera_zip_of_hvector_type&& zhv, Unary_pred p, F f) -> std::pair<int,int> {
//  int pos_in_vec = 0;
//  auto f_tuple = [&p,&f,&pos_in_vec](auto&& hiera_zip_of_vec){
//    auto it = std::find_if(begin(vec),end(vec),p);
//    pos_in_vec = it-begin(vec);
//    if (it!=end(vec)) {
//      f(*it);
//      return true;
//    } else {
//      return false;
//    }
//  };
//
//  int pos_in_tuple = for_each_until__impl_hiera_zip(zhv,f_tuple);
//  return std::make_pair(pos_in_tuple,pos_in_vec);
//}


template<int I, class hiera_zip_of_tuple_type, class Unary_pred, class F>  constexpr auto
find_apply__impl3(hiera_zip_of_tuple_type&& x, Unary_pred p, F f) -> int {
//  constexpr int sz = std::tuple_size_v<std::decay_t<tuple_type>>;
  constexpr int sz = std::decay_t<hiera_zip_of_tuple_type>::size();
  if constexpr (I<sz) {
    // std::cout << "ooo :: " << __PRETTY_FUNCTION__ << std::endl;
    //if (p(std::get<I>(std_e::get<0>(x)))) { // std::get works with tuple, here, we have an hvector TODO .impl()
    if (p(get<I>(std_e::get<0>(x)))) {
      //auto proj_I = [](auto&& y)->auto&{ return std::get<I>(y); }; // same
      auto proj_I = [](auto&& y)->auto&{ return get<I>(y); };
      f(zip_projection(x,proj_I));
      return I;
    } else {
      return find_apply__impl3<I+1>(x,p,f);
    }
  }
  return sz;
}
template<class hiera_zip_of_hvector_type, class Unary_pred, class F> constexpr auto
find_apply__impl4(hiera_zip_of_hvector_type&& zhv, Unary_pred p, F f) -> std::pair<int,int> {
  int pos_in_vec = 0;
  auto p_tuple = [&p,&pos_in_vec](auto&& vec){
    auto it = std::find_if(begin(vec),end(vec),p);
    pos_in_vec = it-begin(vec);
    if (it!=end(vec)) {
      return true;
    } else {
      return false;
    }
  };
  auto f_tuple = [&f,&pos_in_vec](auto&& hiera_zip_of_vec){
    auto proj_i = [&pos_in_vec](auto&& vec)->auto&{ return vec[pos_in_vec]; };
    // auto proj_i = [&pos_in_vec](auto&& vec)->auto&{ return vec; };
    // auto proj_i = [&pos_in_vec](auto&& vec)->auto&{ return vec.at(pos_in_vec); };
    auto zip_proj = zip_projection(hiera_zip_of_vec,proj_i);
    f(zip_proj);
  };

  int pos_in_tuple = find_apply__impl3<0>(zhv,p_tuple,f_tuple);
  return std::make_pair(pos_in_tuple,pos_in_vec);
}

template<class hiera_zip_of_hvector_type, class Unary_pred, class F> constexpr auto
find_fundamental_type_apply_all(hiera_zip_of_hvector_type&& zhv, Unary_pred p, F f) -> std::pair<int,int> {
  return find_apply__impl4(FWD(zhv),p,f);
}


template<int I, class hiera_zip_of_tuple_type, class F>  constexpr auto
apply__impl3(hiera_zip_of_tuple_type&& x, F f) -> void {
  constexpr int sz = std::decay_t<hiera_zip_of_tuple_type>::size();
  if constexpr (I<sz) {
    auto proj_I = [](auto&& y)->auto&{ return get<I>(y); };
    f(zip_projection(x,proj_I));
    apply__impl3<I+1>(x,f);
  }
}

template<class hiera_zip_of_hvector_type, class F> constexpr auto
apply__impl4(hiera_zip_of_hvector_type&& zhv, F f) -> void {
  auto f_tuple = [&f](auto&& hiera_zip_of_vec){
    std::apply(f, hiera_zip_of_vec.impl());
  };
  apply__impl3<0>(zhv,f_tuple);
}

template<class hiera_zip_of_hvector_type, class F> constexpr auto
apply_all(hiera_zip_of_hvector_type&& zhv, F f) -> void {
  apply__impl4(FWD(zhv),f);
}


} // std_e
