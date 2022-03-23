#include "std_e/data_structure/block_range/vblock_range.hpp"

#include "std_e/algorithm/permutation.hpp"
#include <ranges>


namespace std_e {


template<class Rng, class Offset_rng, class Permutation_rng> auto
permute_copy_vblock_range(Rng& values, Offset_rng& offsets, const Permutation_rng& permutation) {
  using T = typename Rng::value_type;
  using I = typename Offset_rng::value_type;
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
permute_vblock_range(Rng& values, Offset_rng& offsets, const Permutation_rng& permutation) -> void {
  auto [new_values,new_offsets] = permute_copy_vblock_range(values,offsets,permutation);
  std::ranges::copy(new_values ,values .begin());
  std::ranges::copy(new_offsets,offsets.begin());
}
template<class T, class I, class Permutation_rng> auto
permute_vblock_range(vblock_range<T,I>& x, const Permutation_rng& permutation) -> void {
  return permute_vblock_range(x.values(),x.offsets());
}


} // std_e
