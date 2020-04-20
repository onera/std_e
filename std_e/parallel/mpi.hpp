#pragma once


#include "std_e/utils/tuple.hpp"
#include "std_e/future/make_array.hpp"


namespace std_e {


inline auto
mpi_init_if_needed() -> bool {
  int flag;
  MPI_Initialized(&flag);
  bool already_init = bool(flag);

  if (!already_init) {
    MPI_Init(NULL, NULL);
  }
  return already_init;
}


// to_mpi_type {
using          mpi_primitive_types        =        std::tuple<  char  ,  int32_t  ,  int64_t  ,  uint32_t  ,  uint64_t  ,  float  ,  double  >;
constexpr auto mpi_primitive_types_values = std_e::make_array(MPI_CHAR,MPI_INT32_T,MPI_INT64_T,MPI_UINT32_T,MPI_UINT64_T,MPI_FLOAT,MPI_DOUBLE);

template<class T> constexpr auto
to_mpi_type__impl() -> MPI_Datatype {
  static_assert(std_e::tuple_contains_type<T,mpi_primitive_types>,"unknown MPI type");
  constexpr int index = std_e::index_of_type_in_tuple<T,mpi_primitive_types>;
  return mpi_primitive_types_values[index];
}

template<class T> constexpr MPI_Datatype to_mpi_type = to_mpi_type__impl<T>();
// to_mpi_type }


} // std_e
