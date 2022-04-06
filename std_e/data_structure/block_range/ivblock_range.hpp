#pragma once


#include "std_e/data_structure/block_range/ivblock_iterator.hpp"
#include "std_e/meta/meta.hpp"
#include "std_e/utils/to_string.hpp"


// ivblock_range means interleaved variable-sized-block range
// Example: |3|1,2,3| 4|1,2,3,4|: the block |1,2,3| followed by the block |1,2,3,4|


namespace std_e {


template<class Rng>
// requires Rng is a Contiguous_range
class ivblock_range {
  public:
    using T = std_e::add_other_type_constness<std_e::element_type<Rng>,Rng>; // T is the value_type of Rng, but with constness of Rng itself

    using scalar_type = T;
    using iterator = ivblock_iterator<T>;
    using const_iterator = ivblock_iterator<const T>;
    using value_type = typename iterator::value_type;
    using reference = typename iterator::reference;
    using const_reference = typename const_iterator::reference;

    ivblock_range() = default;

    ivblock_range(Rng&& rng)
      : rng(std::forward<Rng>(rng)) // copy reference if Rng is ref, move if Rng is not ref
    {}

    auto total_size() const -> ptrdiff_t {
      return rng.size();
    }

    auto begin()       ->       iterator { return {data()}; }
    auto begin() const -> const_iterator { return {data()}; }
    auto end()         ->       iterator { return {data() + total_size()}; }
    auto end()   const -> const_iterator { return {data() + total_size()}; }

    template<class block_type>
    auto push_back(const block_type& b) { // requires Rng is a Container
      //static_assert(std::is_same_v<block_type,reference> || std::is_same_v<block_type,const_reference>);
      auto old_tot_sz = total_size();
      rng.resize( old_tot_sz + 1+b.size() );

      auto c_position_in_cs = rng.begin() + old_tot_sz;
      *c_position_in_cs = b.size();
      std::copy(b.begin(),b.end(),c_position_in_cs+1);
    }

    auto data()       ->       T* { return rng.data(); }
    auto data() const -> const T* { return rng.data(); }

    auto underlying_range()       ->       Rng& { return rng; }
    auto underlying_range() const -> const Rng& { return rng; }
  private:
    Rng rng;
};

template<class Rng> auto
begin(ivblock_range<Rng>& x) {
  return x.begin();
}
template<class Rng> auto
begin(const ivblock_range<Rng>& x) {
  return x.begin();
}
template<class Rng> auto
end(ivblock_range<Rng>& x) {
  return x.end();
}
template<class Rng> auto
end(const ivblock_range<Rng>& x) {
  return x.end();
}


template<class Rng> constexpr auto
view_as_ivblock_range(Rng& rng) {
  return ivblock_range<Rng&>(rng);
}
//template<class Rng, int> constexpr auto
//// requires Rng&& is a rvalue ref
//deep_copy(const ivblock_range<Rng>& x) {
//  using T = typename ivblock_range<Rng>::scalar_type;
//  std::vector<T> v(x.data() , x.data()+x.total_size());
//  return ivblock_range<std::vector<T>>(std::move(v));
//}

template<class Rng> auto
to_string(const ivblock_range<Rng>& x) -> std::string {
  if (x.size()==0) return "";
  std::string s;
  for (auto b : x) {
    s += range_to_lite_string(b) + '|';
  }
  s.resize(s.size()-1);
  return s;
}



} // std_e
