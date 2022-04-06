#pragma once


#include "std_e/data_structure/block_range/block_iterator.hpp"
#include "std_e/meta/meta.hpp"
#include "std_e/utils/to_string.hpp"


namespace std_e {


template<class Rng, int N>
// requires Rng is a Contiguous_range
class block_range {
  public:
    using T = std_e::add_other_type_constness<std_e::element_type<Rng>,Rng>; // T is the value_type of Rng, but with constness of Rng itself
    using I = typename std::remove_cvref_t<Rng>::difference_type;

    static constexpr auto
    block_size() -> int {
      return N;
    }

    using scalar_type = T;
    using iterator = block_iterator<T,N,I>;
    using const_iterator = block_iterator<const T,N,I>;
    using value_type = typename iterator::value_type;
    using reference = typename iterator::reference;
    using const_reference = typename const_iterator::reference;

    block_range() = default;

    block_range(Rng&& rng)
      : rng(std::forward<Rng>(rng)) // copy reference if Rng is ref, move if Rng is not ref
    {
      STD_E_ASSERT(rng.size()%block_size()==0);
    }

    auto total_size() const -> ptrdiff_t {
      return rng.size();
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
    auto push_back(const block_type& b) { // requires Rng is a Container
      //static_assert(std::is_same_v<block_type,reference> || std::is_same_v<block_type,const_reference>);
      auto old_tot_sz = total_size();
      rng.resize( old_tot_sz + b.size() );

      auto c_position_in_cs = rng.begin() + old_tot_sz;
      std::copy(b.begin(),b.end(),c_position_in_cs);
    }

    auto data()       ->       T* { return rng.data(); }
    auto data() const -> const T* { return rng.data(); }

    auto underlying_range()       ->       Rng& { return rng; }
    auto underlying_range() const -> const Rng& { return rng; }
  private:
    Rng rng;
};

template<class Rng, int N> auto
begin(block_range<Rng,N>& x) {
  return x.begin();
}
template<class Rng, int N> auto
begin(const block_range<Rng,N>& x) {
  return x.begin();
}
template<class Rng, int N> auto
end(block_range<Rng,N>& x) {
  return x.end();
}
template<class Rng, int N> auto
end(const block_range<Rng,N>& x) {
  return x.end();
}


template<int N, class Rng> constexpr auto
view_as_block_range(Rng& rng) {
  return block_range<Rng&,N>(rng);
}
template<int N, class T> constexpr auto
view_as_block_range2(span<T>& rng) { // TODO proper overload with is_view
  return block_range<span<T>,N>(rng);
}

template<class Rng, int N> constexpr auto
// requires Rng&& is a rvalue ref
deep_copy(const block_range<Rng,N>& x) {
  using T = typename block_range<Rng,N>::scalar_type;
  std::vector<T> v(x.data() , x.data()+x.total_size());
  return block_range<std::vector<T>,N>(std::move(v));
}

template<class Rng, int N> auto
to_string(const block_range<Rng,N>& x) -> std::string {
  if (x.size()==0) return "";
  std::string s;
  for (auto b : x) {
    s += range_to_lite_string(b) + '|';
  }
  s.resize(s.size()-1);
  return s;
}



} // std_e
