#pragma once


#include "std_e/parallel/struct/distributed_array.hpp"


namespace std_e {


template<class Distri_range, class I> auto
is_compatible_concat_distri(const Distri_range& distris, const std_e::interval_vector<I>& cat_distri) -> bool {
  I sz_tot = 0;
  for (const auto& distri : distris) {
    sz_tot += distri.back();
  }
  return sz_tot == cat_distri.back();
}


template<class Array_range, class Distri_range, class I> auto
concatenate_arrays(Array_range& arrays, const Distri_range& array_distris, const std_e::interval_vector<I>& cat_distri, MPI_Comm comm) {
  STD_E_ASSERT(arrays.size()==array_distris.size());
  STD_E_ASSERT(is_compatible_concat_distri(array_distris,cat_distri));

  // TODO Here we are reconstructing the concatenated array index by index using std_e::scatter
  //      But in reality we could transfer interval by interval (e.g. with a call to neighbor_all_to_all)
  std_e::dist_array<I> cat_array(cat_distri,comm);

  int n_arr = arrays.size();
  I acc_sz = 0;
  for (int i=0; i<n_arr; ++i) {
    std::vector<I> indices(arrays[i].size());
    I first_index = array_distris[i][std_e::rank(comm)];
    std::iota(begin(indices),end(indices),acc_sz+first_index);

    auto sp = std_e::create_exchange_protocol(cat_distri,std::move(indices));
    std_e::scatter(sp,arrays[i],cat_array);

    acc_sz += array_distris[i].back();
  }

  return std::vector<I>(begin(cat_array.local()),end(cat_array.local())); // Note: the copy is not necessary,
                                                                          // but it allows to return a normal std::vector
                                                                          // (not one with a non-std allocator)
}


} // std_e
