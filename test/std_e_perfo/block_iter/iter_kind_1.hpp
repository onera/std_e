#include <vector>
#include "test/std_e_perfo/block_iter/int_t.hpp"

struct connectivity_ref_1 {
  public:
  // ctors
    connectivity_ref_1(int_t* ptr, int N)
      : ptr(ptr)
      , N(N)
    {}

  // reference semantics
    connectivity_ref_1() = delete;

    connectivity_ref_1& operator=(const connectivity_ref_1& other) {
      std::copy(other.ptr, other.ptr+other.size(), ptr);
      return *this;
    }

  // range interface
    constexpr auto
    size() const -> int {
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
    int N;
};

inline auto
operator==(const connectivity_ref_1& x, const connectivity_ref_1& y) {
  return std::equal( x.begin() , x.end() , y.begin() );
}
inline auto
operator!=(const connectivity_ref_1& x, const connectivity_ref_1& y) {
  return !(x == y);
}

class connectivity_it_1 {
  public:
    /// std::iterator type traits
    using value_type = connectivity_ref_1; // TODO connectivity_view
    using reference = connectivity_ref_1;
    using difference_type = int_t;
    using iterator_category = std::forward_iterator_tag; // TODO random

    connectivity_it_1() = default;
    connectivity_it_1(int_t* ptr, int N)
      : ptr(ptr)
      , N(N)
    {}

    auto operator++() -> connectivity_it_1& {
      ptr += N;
      return *this;
    }

    auto operator*() const {
      return reference(ptr,N);
    }

    auto data() const -> int_t* {
      return ptr;
    }

    friend constexpr auto
    operator==(const connectivity_it_1& it0, const connectivity_it_1& it1) {
      return it0.ptr == it1.ptr;
    }
    friend constexpr auto
    operator!=(const connectivity_it_1& it0, const connectivity_it_1& it1) {
      return !(it0 == it1);
    }
  private:
    int_t* ptr;
    int N;
};

template<>
struct std::iterator_traits<connectivity_it_1> {
  using type = connectivity_it_1;
  using value_type = typename type::value_type;
  using reference = typename type::reference;
  using difference_type = typename type::difference_type;
  using iterator_category = typename type::iterator_category;
};

class connectivity_range_1 {
  public:
    using iterator_type = connectivity_it_1;
    using const_iterator_type = connectivity_it_1;
    using reference_type = connectivity_ref_1;
    using const_reference_type = connectivity_ref_1;

    connectivity_range_1() = default;
    connectivity_range_1(std::vector<int_t>& cs, int N)
      : cs_ptr(&cs)
      , N(N)
    {}

    auto size() const -> int {
      return cs_ptr->size();
    }

    auto begin()       ->       iterator_type { return {data()         ,N}; }
    auto end()         ->       iterator_type { return {data() + size(),N}; }

    auto data()       ->       int_t* { return cs_ptr->data(); }
  private:
    std::vector<int_t>* cs_ptr;
    int N;
};
