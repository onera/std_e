#pragma once


#include <iostream>
#include <tuple>

namespace std_e {

// Original impl : https://stackoverflow.com/questions/54931352/how-to-flatten-heterogeneous-lists-aka-tuples-of-tuples-of
namespace flatten_tuple {
  struct flat_t {};
#pragma GCC diagnostic push
#ifdef REAL_GCC
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#endif // REAL_GCC
  template<std::size_t... Is, class...As>
  constexpr auto flatten( std::index_sequence<Is...>, flat_t, std::tuple<As...> as ) {
    return std::tuple_cat( flatten(flat_t{}, std::get<Is>(as))... );
  }
#pragma GCC diagnostic pop
  template<class... As>
  constexpr auto flatten( flat_t, std::tuple<As...> as ) {
    return flatten( std::make_index_sequence<sizeof...(As)>{}, flat_t{}, as );
  }
  template<class T>
  constexpr std::tuple<T> flatten( flat_t, T t ) { return {t}; }

  template<class...Ts>
  constexpr auto flatten( flat_t, Ts... ts ) {
    return std::tuple_cat( flatten(flat_t{}, ts)... );
  }
  constexpr std::tuple<> flatten( flat_t ) { return {}; }
}
template<class...Ts>
constexpr auto sane_flatten( Ts...ts ) {
  return flatten_tuple::flatten(flatten_tuple::flat_t{}, ts...);
}

namespace unpack_tuple {
  template<class...Ts>
  constexpr auto unpack_single( std::tuple<Ts...> t ) {
    return t;
  }
  template<class T>
  constexpr auto unpack_single( std::tuple<T> t ) {
    // return std::get<0>(t);
    return t;
  }
}

template<class...Ts>
constexpr auto insane_flatten( Ts...ts ) {
  return unpack_tuple::unpack_single( sane_flatten(ts...) );
}

template<class...Ts>
constexpr auto flatten( Ts...ts ) {
  return insane_flatten(ts...);
}


// TODO -> Full forward !
// namespace flatten_tuple {
//   struct flat_t {};

//   template<std::size_t... Is, class...As>
//   constexpr auto flatten( std::index_sequence<Is...>, flat_t, std::tuple<As...> as ) {
//     return std::tuple_cat( flatten(flat_t{}, std::get<Is>(as))... );
//   }
//   template<class...As, class...Ts>
//   constexpr auto flatten( flat_t, std::tuple<As...> as ) {
//     return flatten( std::make_index_sequence<sizeof...(As)>{}, flat_t{}, as );
//   }
//   template<class T>
//   constexpr std::tuple<T> flatten( flat_t, T&& t ) { return {t}; }

//   template<class...Ts>
//   constexpr auto flatten( flat_t, Ts&&... ts ) {
//     // return std::tuple_cat( flatten(flat_t{}, ts)... );
//     return std::tuple_cat( flatten(flat_t{}, std::forward<Ts>(ts))... );
//   }
//   constexpr std::tuple<> flatten( flat_t ) { return {}; }
// }
// template<class...Ts>
// constexpr auto sane_flatten( Ts&&...ts ) {
//   return flatten_tuple::flatten(flatten_tuple::flat_t{}, std::forward<Ts>(ts)... );
//   // return flatten_tuple::flatten(flatten_tuple::flat_t{}, ts...);
// }
// namespace unpack_tuple {
//   template<class...Ts>
//   constexpr auto unpack_single( std::tuple<Ts...>&& t ) {return t;}
//   template<class T>
//   constexpr auto unpack_single( std::tuple<T>&& t ) {return std::get<0>(t);}
// }
// template<class...Ts>
// constexpr auto insane_flatten( Ts&&...ts ) {
//   // return unpack_tuple::unpack_single( sane_flatten(ts...) );
//   return unpack_tuple::unpack_single( sane_flatten(std::forward<Ts>(ts)...) );
// }
// template<class...Ts>
// constexpr auto flatten( Ts&&...ts ) {
//   return insane_flatten(std::forward<Ts>(ts)...);
//   // return insane_flatten(ts...);
// }

}
