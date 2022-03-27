#include "std_e/data_structure/block_range/vblock_range.hpp"

#include "std_e/algorithm/permutation.hpp"
#include "std_e/algorithm/mismatch_points.hpp"
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
  std::ranges::stable_sort(perm,{},proj_to_block_sizes);
  auto partition_indices = std_e::mismatch_indices(perm,{},proj_to_block_sizes,I{});

  return {std::move(perm),std::move(partition_indices)};
}


template<class Rng, class Offset_rng, class Permutation_rng> auto
permute_copy_vblock_range(Rng&& values, Offset_rng&& offsets, const Permutation_rng& permutation) {
  using T = typename std::remove_cvref_t<Rng>::value_type;
  using I = typename std::remove_cvref_t<Offset_rng>::value_type;
  // prepare accessors
  auto old_vbr = std_e::view_as_vblock_range(values,offsets);

  std::vector<T> new_values(values.size());
  std::vector<I> new_offsets(offsets.size());
  auto new_vbr = std_e::view_as_vblock_range(new_values,new_offsets);

  // permute
  std_e::permute_copy(old_vbr,new_vbr,permutation);

  return std::make_pair(std::move(new_values),std::move(new_offsets));
}

template<class Rng, class Offset_rng, class Permutation_rng> auto
permute_vblock_range(Rng&& values, Offset_rng&& offsets, const Permutation_rng& permutation) -> void {
  auto [new_values,new_offsets] = permute_copy_vblock_range(FWD(values),FWD(offsets),permutation);
  std::ranges::copy(new_values ,values .begin());
  std::ranges::copy(new_offsets,offsets.begin());
}
template<class T, class I, class Permutation_rng> auto
permute_vblock_range(vblock_range<T,I>& x, const Permutation_rng& permutation) -> void {
  return permute_vblock_range(x.values(),x.offsets(),permutation);
}


} // std_e
