#include <vector>
#include "std_e/__unused/int_t.hpp"

template<int N>
struct connectivity_ref_0 {
  public:
  // ctors
    connectivity_ref_0(int_t* ptr)
      : ptr(ptr)
    {}

  // reference semantics
    connectivity_ref_0() = delete;

    connectivity_ref_0& operator=(const connectivity_ref_0& other) {
      std::copy(other.ptr, other.ptr+other.size(), ptr);
      return *this;
    }

  // range interface
    static constexpr auto
    size() -> int {
      return N;
    }

    constexpr auto begin()       ->       int_t* { return ptr; }
    constexpr auto begin() const -> const int_t* { return ptr; }
    constexpr auto end()         ->       int_t* { return ptr + N; }
    constexpr auto end()   const -> const int_t* { return ptr + N; }

    constexpr auto operator[](int_t i)       ->       int_t& { return ptr[i]; }
    constexpr auto operator[](int_t i) const -> const int_t& { return ptr[i]; }
  private:
    int_t* ptr;
};

template<int N> inline auto
operator==(const connectivity_ref_0<N>& x, const connectivity_ref_0<N>& y) {
  return std::equal( x.begin() , x.end() , y.begin() );
}
template<int N> inline auto
operator!=(const connectivity_ref_0<N>& x, const connectivity_ref_0<N>& y) {
  return !(x == y);
}

template<int N>
class connectivity_it_0 {
  public:
    /// std::iterator type traits
    using value_type = connectivity_ref_0<N>; // TODO connectivity_view
    using reference = connectivity_ref_0<N>;
    using difference_type = int_t;
    using iterator_category = std::forward_iterator_tag; // TODO random

    connectivity_it_0() = default;
    connectivity_it_0(int_t* ptr)
      : ptr(ptr)
    {}

    auto operator++() -> connectivity_it_0& {
      ptr += N;
      return *this;
    }

    auto operator*() const {
      return reference(ptr);
    }

    auto data() const -> int_t* {
      return ptr;
    }

    friend constexpr auto
    operator==(const connectivity_it_0& it0, const connectivity_it_0& it1) {
      return it0.ptr == it1.ptr;
    }
    friend constexpr auto
    operator!=(const connectivity_it_0& it0, const connectivity_it_0& it1) {
      return !(it0 == it1);
    }
  private:
    int_t* ptr;
};

template<int N>
struct std::iterator_traits<connectivity_it_0<N>> {
  using type = connectivity_it_0<N>;
  using value_type = typename type::value_type;
  using reference = typename type::reference;
  using difference_type = typename type::difference_type;
  using iterator_category = typename type::iterator_category;
};

template<int N>
class connectivity_range_0 {
  public:
    using iterator_type = connectivity_it_0<N>;
    using const_iterator_type = connectivity_it_0<N>;
    using reference_type = connectivity_ref_0<N>;
    using const_reference_type = connectivity_ref_0<N>;

    connectivity_range_0() = default;
    connectivity_range_0(std::vector<int_t>& cs)
      : cs_ptr(&cs)
    {}

    auto size() const -> int {
      return cs_ptr->size();
    }

    auto begin()       ->       iterator_type { return {data()}; }
    auto begin() const -> const_iterator_type { return {data()}; }
    auto end()         ->       iterator_type { return {data() + size()}; }
    auto end()   const -> const_iterator_type { return {data() + size()}; }

    auto data()       ->       int_t* { return cs_ptr->data(); }
    auto data() const -> const int_t* { return cs_ptr->data(); }
  private:
    std::vector<int_t>* cs_ptr;
};
