#pragma once

#include <tuple>
#include "std_e/base/macros.hpp"


namespace std_e {


template<class... Ts>
class hzip {
  public:
  // traits
    using impl_type = std::tuple<Ts...>;
  // ctor
    template<class... Ts0>
    hzip(Ts0&&... xs)
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
get(hzip<Ts...>& x) -> T& {
  return std::get<T>(x.impl());
}
template<class T, class... Ts> constexpr auto
get(const hzip<Ts...>& x) -> const T& {
  return std::get<T>(x.impl());
}
template<size_t I, class... Ts> constexpr auto
get(hzip<Ts...>& x) -> auto& {
  return std::get<I>(x.impl());
}
template<size_t I, class... Ts> constexpr auto
get(const hzip<Ts...>& x) -> const auto& {
  return std::get<I>(x.impl());
}


template<class... Ts> constexpr auto
hzip_view(Ts&... xs) {
  return hzip<Ts&...>(xs...);
}
template<class... Ts> constexpr auto
hzip_const_view(const Ts&... xs) {
  return hzip<const Ts&...>(xs...);
}

template<size_t I, class hzip_type>
struct hzip_element {
  using impl_type = typename hzip_type::impl_type;
  using type = std::tuple_element_t<I, impl_type>;
};

template<size_t I, class hzip_type>
using hzip_element_t = typename hzip_element<I, hzip_type>::type;


template<size_t I, class tuple_hzip_type>
struct tuple_hzip_size_at {
  using tuple_type_I = typename hzip_element<I, tuple_hzip_type>::type;
  static constexpr size_t value = std::tuple_size_v<std::decay_t<tuple_type_I>>;
};

template<size_t I, class hzip_type>
inline constexpr size_t tuple_hzip_size_at_v = tuple_hzip_size_at<I, hzip_type>::value;


template<int I, class hzip_type> constexpr auto
same_tuple_size_in_hzip__impl0(const size_t s0) -> bool {
  constexpr size_t sz = hzip_type::size();
  if constexpr (I<sz) {
    if ( tuple_hzip_size_at_v<I, hzip_type> != s0 ){
      return false;
    } else {
      return same_tuple_size_in_hzip__impl0<I+1, hzip_type>(s0);
    }
  }
  return true;
};

template<class hzip_type> constexpr auto
same_tuple_size_in_hzip__impl() -> bool {
  constexpr size_t sz = hzip_type::size();
  if constexpr ( sz == 0 ){
    return true;
  } else {
    constexpr size_t s0 = tuple_hzip_size_at_v<0, hzip_type>;
    return same_tuple_size_in_hzip__impl0<1, hzip_type>(s0);
  }
};


template<class hzip_type> constexpr bool same_tuple_size_in_hzip = same_tuple_size_in_hzip__impl<hzip_type>();


template<class tuple_hzip_type>
struct tuple_hzip_size {
  static_assert(same_tuple_size_in_hzip<tuple_hzip_type>);
  static constexpr size_t value = tuple_hzip_size_at_v<0, tuple_hzip_type>;
};

template<class hzip_type>
inline constexpr size_t tuple_hzip_size_v = tuple_hzip_size<hzip_type>::value;


template<class... Ts, class Projection> constexpr auto
zip_projection(hzip<Ts...>& hzip, Projection proj) {
  return hzip_view( proj(get<Ts>(hzip.impl())) ... ); // TODO get<Ts> -> get<I> (here, if several times same type: WRONG behavior)
}

template<class... Ts, class Projection> constexpr auto
zip_projection2(hzip<Ts...>& hzip, Projection proj) {
  return std::make_tuple(1);
  // return hzip_view( proj(get<0>(hzip.impl())) ... ); // TODO get<Ts> -> get<I> (here, if several times same type: WRONG behavior)
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

template<int I, class hzip_of_tuple_type, class F>  constexpr auto
for_each_element__impl_hzip_0(hzip_of_tuple_type&& x, F f) -> void {
  constexpr int sz = tuple_hzip_size_v<std::decay_t<hzip_of_tuple_type>>;
  if constexpr (I<sz) {
    auto proj_I = [](auto&& y)->auto&{return get<I>(y);};
    f(zip_projection2(x,proj_I));
    for_each_element__impl_hzip_0<I+1>(x,f);
  }
}


template<class hzip_of_tuple_type, class F> constexpr auto
for_each_element__impl_hzip(hzip_of_tuple_type&& zhv, F f) -> void {
  auto f_tuple = [&f](auto&& hiera_zip_of_vec){
    apply(f, hiera_zip_of_vec);
  };
  // for_each_element__impl_hzip_0<0>(FWD(zhv),f_tuple);
  for_each_element__impl_hzip_0<0>(zhv,f_tuple);
}

template<class... Ts, class F> constexpr auto
for_each_element(hzip<Ts...>& x, F f) -> void {
  // for_each_element__impl_hzip(FWD(x),f);
  for_each_element__impl_hzip(x,f);
}

template<class... Ts, class F> constexpr auto
for_each_element(const hzip<Ts...>& x, F f) -> void {
  for_each_element__impl_hzip(x,f);
}


} // std_e
