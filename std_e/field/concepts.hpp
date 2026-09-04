#pragma once


#include <concepts>
#include <array>


namespace std_e {


template<class F>
concept Field = requires
{
  { F::rank    } -> std::convertible_to<int>;
  { F::dim_tot } -> std::convertible_to<int>;
  { F::dims    } -> std::convertible_to<std::array<int, F::rank>>;
  // n_element ? (uniform_field does not have that: should we refine the concept? or niform_field.n_element==1?)
  // operator() ? (uniform_field does not have that: should we refine the concept?)
  { F::is_owner } -> std::convertible_to<bool>;
};


template<class F> concept Scalar_field = Field<F> && std::decay_t<F>::rank==0;
template<class F> concept Vector_field = Field<F> && std::decay_t<F>::rank==1;
template<class F> concept Tensor_field = Field<F> && std::decay_t<F>::rank==2;

template<class F, int... Ns>      concept Field_of_dims        = Field<F>        && F::dims==std::array<int,sizeof...(Ns)>{Ns...};
template<class F, int N>          concept Vector_field_of_dim  = Vector_field<F> && F::dims[0]==N;
template<class F, int N0, int N1> concept Tensor_field_of_dims = Tensor_field<F> && F::dims[0]==N0 && F::dims[1]==N1;


template<class F, class T, int... Ns> constexpr auto
can_convert_field() -> bool {
  // can't make field view of non-field
  if constexpr (! Field_of_dims<F,Ns...>) return false;
  else {
    using FT = F::value_type;
    return std::convertible_to<FT, T>;
  }
}

template<class F, class T, int... Ns> constexpr auto
can_take_reference_of_field() -> bool {
  // can't make field view of non-field
  if constexpr (! Field_of_dims<F,Ns...>) return false;

  // can't take a view if the element types differ
  using FT = F::value_type;
  if (! std::same_as< std::remove_const_t<T> , std::remove_const_t<FT> >) return false;

  if (std::is_const_v<T>) {
     // the view we want to make will be const: accept const and mutable
     return true;
  } else {
     // the view we want to make will be mutable: accept only mutable
     if (F::is_owner) return !std::is_const_v<F>; // if owner, being mutable means the type itself is mutable
     else return !std::is_const_v<FT>; // if non-owner, being mutable means the value_type is mutable
  }
}

} // std_e
