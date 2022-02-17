#pragma once


#include "std_e/data_structure/block_range/block_iterator.hpp"
#include "std_e/meta/meta.hpp"
#include "std_e/utils/to_string.hpp"


namespace std_e {


//TODO allow block_range to own
template<
  class C, int N,
  template<class,auto> class val_type = std_e::array,
  template<class,auto> class ref_type = std_e::span_ref
>
// requires C is a Contiguous_range
class block_range {
  public:
    using T = std_e::add_other_type_constness<std_e::element_type<C>,C>; // T is the value_type of C, but with constness of C itself

    //using index_type = int;
    static constexpr auto
    block_size() -> int {
      return N;
    }

    using value_type = val_type<T,N>;
    using reference = ref_type<T,N>;
    using const_reference = ref_type<const T,N>;
    using iterator = block_iterator<T,N,val_type,ref_type>;
    using const_iterator = block_iterator<const T,N,val_type,ref_type>;

    block_range() = default;
    block_range(C& cs)
      : cs_ptr(&cs)
    {
      STD_E_ASSERT(cs.size()%block_size()==0);
    }

    auto total_size() const -> ptrdiff_t {
      return cs_ptr->size();
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
      cs_ptr->resize( old_tot_sz + c.size() );

      auto c_position_in_cs = cs_ptr->begin() + old_tot_sz;
      std::copy(c.begin(),c.end(),c_position_in_cs);
    }

    auto data()       ->       T* { return cs_ptr->data(); }
    auto data() const -> const T* { return cs_ptr->data(); }

    auto underlying_range()       ->       C& { return *cs_ptr; }
    auto underlying_range() const -> const C& { return *cs_ptr; }
  private:
    C* cs_ptr;
};


template<int N, class C> constexpr auto
view_as_block_range(C& c) {
  return block_range<std::remove_reference_t<C>,N>(c);
}

template<class C, int N, template<class,auto> class VT, template<class,auto> class RT> auto
to_string(const block_range<C,N,VT,RT>& x) -> std::string {
  //std::string s = join(x,;
  std::string s;
  for (auto b : x) {
    s += range_to_lite_string(b) + '|';
  }
  s.resize(s.size()-1);
  return s;
}



} // std_e
