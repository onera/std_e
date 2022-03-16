#pragma once


#include <type_traits>
#include "std_e/data_structure/block_range/vblock_ref.hpp"
#include "std_e/data_structure/block_range/vblock_val.hpp"


namespace std_e {


template<class T, class I>
class vblock_iterator {
  public:
  // type traits
    using scalar_type = T;
    using index_type = I;

    /// std::iterator type traits
    //using value_type = vblock_val<T>;
    using reference = vblock_ref<T,I>;
    using difference_type = index_type;
    using iterator_category = std::random_access_iterator_tag;

  // ctors
    vblock_iterator() = default;
    vblock_iterator(T* ptr, I* offsets_ptr)
      : ptr(ptr)
      , offsets_ptr(offsets_ptr)
    {}

    auto block_size() -> int {
      return *(offsets_ptr+1)-offsets_ptr;
    }

    auto operator++() -> vblock_iterator& {
      ++offsets_ptr;
      return *this;
    }
    auto operator--() -> vblock_iterator& {
      --offsets_ptr;
      return *this;
    }
    auto operator++(int) -> vblock_iterator {
      vblock_iterator tmp(*this);
      ++*this;
      return tmp;
    }
    auto operator--(int) -> vblock_iterator {
      vblock_iterator tmp(*this);
      --*this;
      return tmp;
    }

    auto operator+=(index_type i) -> vblock_iterator& {
      offsets_ptr += i;
      return *this;
    }
    auto operator-=(index_type i) -> vblock_iterator& {
      offsets_ptr -= i;
      return *this;
    }
    friend
    auto operator+(vblock_iterator x, index_type i) -> vblock_iterator {
      vblock_iterator res = x;
      res += i;
      return res;
    }
    friend
    auto operator+(index_type i, vblock_iterator x) -> vblock_iterator {
      return x+i;
    }
    friend
    auto operator-(vblock_iterator x, index_type i) -> vblock_iterator {
      vblock_iterator res = x;
      res -= i;
      return res;
    }
    friend
    auto operator-(vblock_iterator x, vblock_iterator y) -> difference_type {
      return x.offsets_ptr-y.offsets_ptr;
    }

    auto operator*() const {
      return reference(ptr+offsets_ptr[0],offsets_ptr);
    }
    auto
    operator[](index_type i) const -> reference {
      return reference(ptr+offsets_ptr[i],offsets_ptr+i);
    }

    auto data() const -> T* {
      return ptr+offsets_ptr[0];
    }

    constexpr auto
    operator<=>(const vblock_iterator& x) const = default;
  private:
    T* ptr;
    I* offsets_ptr;
};


} // std_e


template<class T, class I>
struct std::iterator_traits<std_e::vblock_iterator<T,I>> {
  using type = std_e::vblock_iterator<T,I>;
  //using value_type = typename type::value_type;
  using reference = typename type::reference;
  using difference_type = typename type::difference_type;
  using iterator_category = typename type::iterator_category;
};
