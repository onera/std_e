#pragma once

namespace std_e {
namespace meta {

template<int I, class T> constexpr auto
find_idx_impl() -> int
{
  static_assert(0, "Could not find type in list");
  return 0;
}

template<int I, class T, class Head, class... Tail> constexpr auto
find_idx_impl() -> int
{
  if constexpr (std::is_same_v<T,Head>) return I;
  else return find_idx_impl<I+1,T,Tail...>();
}

template<class T, class... Ts> constexpr int find_idx = find_idx_impl<0, T, Ts...>();




template<int I, class T> constexpr auto
find_rng_idx_impl() -> int
{
  static_assert(0, "Could not find type in list");
  return 0;
}

template<int I, class T, class Head, class... Tail> constexpr auto
find_rng_idx_impl() -> int
{
  if constexpr (std::is_same_v<T,typename Head::value_type>) return I;
  else return find_rng_idx_impl<I+1,T,Tail...>();
}

template<class T, class... Rngs> constexpr int find_rng_idx = find_rng_idx_impl<0, T, Rngs...>();




template<int I, class T, template<class> class Proj> constexpr auto
find_proj_idx_impl() -> int
{
  static_assert(0, "Could not find type in list");
  return 0;
}

template<int I, class T, template<class> class Proj, class Head, class... Tail> constexpr auto
find_proj_idx_impl() -> int
{
  if constexpr (std::is_same_v<T, Proj<Head>>) return I;
  else return find_proj_idx_impl<I+1,T,Proj,Tail...>();
}

template<class T, template<class> class Proj, class... Ts> constexpr int find_proj_idx = find_proj_idx_impl<0, T, Proj, Ts...>();


} // meta
} // std_e
