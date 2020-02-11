#pragma once


#include <vector>
#include "std_e/algorithm/permutation.hpp"
#include "std_e/future/contract.hpp"

namespace std_e {


template<class KT, class VT>
class frozen_flat_map {
  public:
    using key_type = KT;
    using mapped_type = VT;

    frozen_flat_map() = default;
    frozen_flat_map(std::vector<KT> ks, std::vector<VT> vs)
      : keys(std::move(ks))
      , values(std::move(vs))
    {
      auto perm = std_e::sort_permutation(keys);
      permute(keys,perm);
      permute(values,perm);
    }

    constexpr auto
    operator[](const KT& key) const -> const VT& {
      auto it = std::lower_bound(begin(keys),end(keys),key);
      STD_E_ASSERT(*it == key);
      int idx = it-begin(keys);
      return values[idx];
    }

  private:
    std::vector<KT> keys;
    std::vector<VT> values;
};


template<class KT> auto
permutation_frozen_flat_map(std::vector<KT> keys) -> frozen_flat_map<KT,int> {
  std::vector<int> values(keys.size());
  std::iota(begin(values),end(values),0);
  return {std::move(keys),std::move(values)};
}


} // std_e
