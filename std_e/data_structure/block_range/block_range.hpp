#pragma once


#include "std_e/data_structure/block_range/block_iterator.hpp"
#include "std_e/meta/meta.hpp"
#include "std_e/utils/to_string.hpp"


namespace std_e {


template<class C, int N>
// requires C is a Contiguous_range
class block_range {
  public:
    using T = std_e::add_other_type_constness<std_e::element_type<C>,C>; // T is the value_type of C, but with constness of C itself

    //using index_type = int;
    static constexpr auto
    block_size() -> int {
      return N;
    }

    using scalar_type = T;
    using iterator = block_iterator<T,N>;
    using const_iterator = block_iterator<const T,N>;
    using value_type = typename iterator::value_type;
    using reference = typename iterator::reference;
    using const_reference = typename const_iterator::reference;

    block_range() = default;

    block_range(C&& cs)
      : cs(std::forward<C>(cs)) // copy reference if C is ref, move if C is not ref
    {
      STD_E_ASSERT(cs.size()%block_size()==0);
    }

    auto total_size() const -> ptrdiff_t {
      return cs.size();
    }
    auto size() const -> ptrdiff_t {
      return total_size()/block_size();
    }

    auto begin()       ->       iterator { return {data()}; }
    auto begin() const -> const_iterator { return {data()}; }
    auto end()         ->       iterator { return {data() + total_size()}; }
    auto end()   const -> const_iterator { return {data() + total_size()}; }

    template<class Integer> auto operator[](Integer i)       ->       reference { return       reference{data() + i*block_size()}; }
    template<class Integer> auto operator[](Integer i) const -> const_reference { return const_reference{data() + i*block_size()}; }

    template<class block_type>
    auto push_back(const block_type& c) { // requires C is a Container
      //static_assert(std::is_same_v<block_type,reference> || std::is_same_v<block_type,const_reference>);
      auto old_tot_sz = total_size();
      cs.resize( old_tot_sz + c.size() );

      auto c_position_in_cs = cs.begin() + old_tot_sz;
      std::copy(c.begin(),c.end(),c_position_in_cs);
    }

    auto data()       ->       T* { return cs.data(); }
    auto data() const -> const T* { return cs.data(); }

    auto underlying_range()       ->       C& { return cs; }
    auto underlying_range() const -> const C& { return cs; }
  private:
    C cs;
};

template<class C, int N> auto
begin(block_range<C,N>& x) {
  return x.begin();
}
template<class C, int N> auto
begin(const block_range<C,N>& x) {
  return x.begin();
}
template<class C, int N> auto
end(block_range<C,N>& x) {
  return x.end();
}
template<class C, int N> auto
end(const block_range<C,N>& x) {
  return x.end();
}


template<int N, class C> constexpr auto
view_as_block_range(C& c) {
  return block_range<C&,N>(c);
}
template<class C, int N> constexpr auto
// requires C&& is a rvalue ref
deep_copy(const block_range<C,N>& x) {
  using T = typename block_range<C,N>::scalar_type;
  std::vector<T> v(x.data() , x.data()+x.total_size());
  return block_range<std::vector<T>,N>(std::move(v));
}

template<class C, int N> auto
to_string(const block_range<C,N>& x) -> std::string {
  std::string s;
  for (auto b : x) {
    s += range_to_lite_string(b) + '|';
  }
  s.resize(s.size()-1);
  return s;
}



} // std_e
