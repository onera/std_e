#include "std_e/data_structure/block_range/vblock_range.hpp"

#include "std_e/algorithm/permutation.hpp"
#include "std_e/algorithm/mismatch_points.hpp"
#include "std_e/future/ranges/algorithm.hpp"
#include <ranges>


namespace std_e {


// TODO unit test
template<class Rng, class I = typename Rng::value_type> auto
indirect_partition_by_block_size(const Rng& block_offsets) -> std::pair<std::vector<I>,std::vector<I>> {
  // 0. precompute sizes
  auto block_sizes = std_e::interval_lengths(block_offsets);

  // 1. init permutation
  I n_block = block_sizes.size();
  auto perm = std_e::iota_vector(n_block);

  // 2. permute // TODO replace by std_e::partition_pivot_stable
  auto proj_to_block_sizes = [&block_sizes](I i){ return block_sizes[i]; };
  //std_e::ranges::stable_sort(perm,std::ranges::less{},proj_to_block_sizes);
  std_e::ranges::stable_sort(perm,std::less{},proj_to_block_sizes);
  auto partition_indices = std_e::mismatch_indices(perm,{},proj_to_block_sizes,I{});

  return {std::move(perm),std::move(partition_indices)};
}


template<class vblock_range_type, class Permutation_rng> auto
permute_copy_vblock_range(vblock_range_type&& old_vbr, const Permutation_rng& permutation) {
  auto new_vbr = vblock_vector_with_same_size(old_vbr);
  std_e::permute_copy(old_vbr,new_vbr,permutation);
  return new_vbr;
}

template<class T, class I, class Permutation_rng> auto
permute_vblock_range(vblock_range<T,I>& x, const Permutation_rng& permutation) -> void {
  auto x_new = permute_copy_vblock_range(x,permutation);

  x.assign(std::move(x_new));
}


} // std_e
