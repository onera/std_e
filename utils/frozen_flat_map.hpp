#pragma once


#include <vector>
#include "std_e/algorithm/permutation.hpp"


namespace std_e {


template<class KT, class VT>
class frozen_flat_map {
  public:
    using key_type = KT;
    using mapped_type = VT;

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
      int idx = it-begin(keys);
      return values[idx];
    }

  private:
    std::vector<KT> keys;
    std::vector<VT> values;
};


} // std_e
