#pragma once


#include <type_traits>
#include "std_e/data_structure/block_range/ivblock_ref.hpp"
#include "std_e/data_structure/block_range/vblock_val.hpp"


namespace std_e {


template<class I>
class ivblock_iterator {
  public:
  // type traits
    using scalar_type = I;

    /// std::iterator type traits
    using value_type = vblock_val<I>;
    using reference = ivblock_ref<I>;
    using difference_type = I;
    using iterator_category = std::forward_iterator_tag;

  // ctors
    ivblock_iterator() = default;
    ivblock_iterator(I* ptr)
      : ptr(ptr)
    {}

    auto block_size() -> I {
      return *ptr;
    }

    auto operator++() -> ivblock_iterator& {
      ptr += 1+block_size();
      return *this;
    }
    auto operator++(int) -> ivblock_iterator {
      ivblock_iterator tmp(*this);
      ++*this;
      return tmp;
    }

    auto operator*() const {
      return reference(ptr);
    }

    auto data() const -> I* {
      return ptr;
    }

    constexpr auto
    operator<=>(const ivblock_iterator& x) const = default;
  private:
    I* ptr;
};


} // std_e


template<class I>
struct std::iterator_traits<std_e::ivblock_iterator<I>> {
  using type = std_e::ivblock_iterator<I>;
  using value_type = typename type::value_type;
  using reference = typename type::reference;
  using difference_type = typename type::difference_type;
  using iterator_category = typename type::iterator_category;
};
