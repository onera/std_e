#pragma once


#include "std_e/algorithm/permutation.hpp"
#include "std_e/interval/interval.hpp"


namespace std_e {


// TODO deprecate {
template<class I> auto
permute_id(I& id, const std::vector<I>& permutation, I offset) -> void {
  // Precondition: offset <= id < offset + permutation.size()
  I old_index = id - offset;
  I new_index = permutation[old_index];
  id = new_index + offset;
}

template<class I> auto
permute_id_if_in_range(I& id, const std::vector<I>& permutation, interval<I> inter) -> void {
  // Precondition: size(inter) == permutation.size()
  if ( is_in(id,inter) ) {
    permute_id(id,permutation,inter.first);
  }
}


template<class I_range, class I> auto
// requires I_range is an integer range (i.e. begin(), end())
update_ids_after_permutation(I_range& ids, const std::vector<I>& permutation, I offset) {
  // Preconditions:
  //   - permutation is an index permutation (i.e. sort(permutation) == iota(permutation.size()))
  //   - for id in ids, offset <= id < offset + permutation.size()
  for (auto& id : ids) {
    permute_id(id,permutation,offset);
  }
}
template<class I_range, class I> auto
// requires I_range is an integer range (i.e. begin(), end())
update_ids_in_range_after_permutation(I_range& ids, const std::vector<I>& permutation, interval<I> inter) -> void {
  // Preconditions:
  //   - permutation is an index permutation (i.e. sort(permutation) == iota(permutation.size()))
  //   - size(inter) == permutation.size()
  for (auto& id : ids) {
    permute_id_if_in_range(id,permutation,inter);
  }
}
// TODO end deprecate }


/* concept Permutation: operator()(Integer)->Integer */

template<class I>
class offset_permutation {
  public:
    constexpr offset_permutation() = default;
    constexpr offset_permutation(I offset, std::vector<I> perm)
      : offset(offset)
      , perm(std::move(perm))
    {}

    constexpr auto
    operator()(I i) const {
      return perm[i-offset] + offset;
    }

    // for compatibility with integer ranges which are also permutations
    constexpr auto
    operator[](I i) const {
      return (*this)(i);
    }
  private:
    I offset;
    std::vector<I> perm;
};

template<class Permutation, class Range> constexpr auto
apply(const Permutation& p, Range& r) -> void {
  for (auto& i : r) {
    i = p(i);
  }
}


} // std_e
