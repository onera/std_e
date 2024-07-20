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
};


template<class T> concept Scalar_field = Field<T> && T::rank==0;
template<class T> concept Vector_field = Field<T> && T::rank==1;
template<class T> concept Tensor_field = Field<T> && T::rank==2;

template<class T, int N> concept Vector_field_of_dim = Vector_field<T> && T::dims[0]==N;
template<class T, int N0, int N1> concept Tensor_field_of_dims = Tensor_field<T> && T::dims[0]==N0 && T::dims[1]==N1;


} // std_e
