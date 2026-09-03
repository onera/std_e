#pragma once


#include <concepts>
#include <array>


namespace std_e {


template<class T>
concept Field = requires
{
  { T::rank    } -> std::convertible_to<int>;
  { T::dim_tot } -> std::convertible_to<int>;
  { T::dims    } -> std::convertible_to<std::array<int, T::rank>>;
  // n_element ? (uniform_field does not have that: should we refine the concept? or niform_field.n_element==1?)
  // operator() ? (uniform_field does not have that: should we refine the concept?)
  { T::is_owner } -> std::convertible_to<bool>;
};


template<class T> concept Scalar_field = Field<T> && std::decay_t<T>::rank==0;
template<class T> concept Vector_field = Field<T> && std::decay_t<T>::rank==1;
template<class T> concept Tensor_field = Field<T> && std::decay_t<T>::rank==2;

template<class T, int N> concept Vector_field_of_dim = Vector_field<T> && T::dims[0]==N;
template<class T, int N0, int N1> concept Tensor_field_of_dims = Tensor_field<T> && T::dims[0]==N0 && T::dims[1]==N1;


} // std_e
