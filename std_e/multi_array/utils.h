#pragma once


namespace std_e {


template<class Integer> inline auto
reshape(dyn_rank_shape<Integer>& x, const dyn_multi_index<Integer>& dims) {
  std_e::ASSERT(x.offset()==zero_dyn_multi_index<Integer>(x.offset().size()));
  x.extent() = dims;
  x.offset() = zero_dyn_multi_index<Integer>(dims.size());
}

template<class Memory_ressource> auto
resize_memory(Memory_ressource& x, size_t n) -> void {
  x.resize(n);
}
template<class T_ptr> auto
resize_memory(memory_view<T_ptr>& x, size_t n) -> void {
  // ASSERT same as old size
}

template<class Memory_ressource, class Multi_array_shape> auto
reshape(multi_array<Memory_ressource,Multi_array_shape>& x, const typename Multi_array_shape::multi_index_type& dims) {
  reshape(x.shape(),dims);
  size_t total_size = std_e::cartesian_product(dims);
  resize_memory(x.memory(),total_size);
}


} // std_e
