#pragma once

#include <iostream>

#include "std_e/future/span.hpp"
#include "std_e/interval/knot_sequence.hpp"
#include <vector>
#include "std_e/data_structure/multi_range.hpp"
#include "std_e/log.hpp" // TODO


namespace std_e {

template<class data_range_type, class offsets_range_type, int rank>
class jagged_range;

template<class T, int rank=2, class I=int>
using jagged_span = jagged_range<span<T>,knot_span<I>,rank>;
template<class T, int rank=2, class I=int>
using jagged_vector = jagged_range<std::vector<T>,knot_vector<I>,rank>;

template<int rank, class... Ts>
using jagged_multi_vector = jagged_range<multi_vector<Ts...>,knot_vector<int>,rank>;

template<class data_range_type, class offsets_range_type, int rank>
class jagged_range {
  private:
    using offsets_array_type = std::array<offsets_range_type,rank-1>;
    using T = typename data_range_type::value_type;
    using I = typename offsets_range_type::value_type;

    data_range_type flat_values;
    offsets_array_type idx_array;
    I off = 0;
  public:
  // type traits
    using value_type = T;

  // ctors
    jagged_range()
    {
      for (int i=0; i<rank-1; ++i) {
        idx_array[i] = {0};
      }
    }

    jagged_range(data_range_type flat_values, offsets_range_type idx_array, I off)
      : flat_values(std::move(flat_values))
      , off(off)
    {
      idx_array[0] = std::move(idx_array);
      static_assert(rank==2);
    }

    jagged_range(data_range_type flat_values, offsets_array_type idx_array, I off)
      : flat_values(std::move(flat_values))
      , idx_array(std::move(idx_array))
      , off(off)
    {}

    jagged_range(std::initializer_list<std::initializer_list<T>> ll)
      : jagged_range()
    {
      static_assert(rank==2);
      for (const auto& l : ll) {
        push_level(1);
        for (const T& v : l) {
          push_back(v);
        }
      }
    }
    jagged_range(std::initializer_list<std::initializer_list<std::initializer_list<T>>> lll)
      : jagged_range()
    {
      static_assert(rank==3);
      for (const auto& ll : lll) {
        push_level(1);
        for (const auto& l : ll) {
          push_level(2);
          for (const T& v : l) {
            push_back(v);
          }
        }
      }
    }

  // basic
    auto size() const -> I {
      return idx_array[0].nb_intervals();
    }
    auto data() const -> const T* {
      return flat_values.data();
    }
    auto data() -> T* {
      return flat_values.data();
    }

  // accessors
    auto flat_view() const {
      return make_span(flat_values);
    }
    auto index_array() const -> const auto& {
      return idx_array;
    }
    auto indices() const -> knot_span<const I> {
      static_assert(rank==2);
      std::cout << " ---- " << __PRETTY_FUNCTION__ << std::endl;
      return to_knot_span(make_span(idx_array[0].as_base()));
    }
    // // TODO : FIX
    // auto indices2() const{
    //   static_assert(rank==2);
    //   std::cout << " ---- " << __PRETTY_FUNCTION__ << std::endl;
    //   // std::vector<int> tmp = idx_array[0].as_base();
    //   // return std_e::knot_sequence<std_e::span<const int>>(tmp.data(), tmp.size());
    //   // return std_e::knot_sequence<std_e::span<const int>>(my_type());
    //   // return std_e::knot_sequence<std_e::span<const int>>(my_type());
    //   return std_e::knot_sequence<std_e::span<const int>>(true);
    // }
    // auto indices() -> knot_span<I> {
    //   static_assert(rank==2);
    //   return to_knot_span(make_span(idx_array[0]));
    // }
    auto offset() const -> I {
      return off;
    }

  // container interface
    auto push_level(int lvl) {
      STD_E_ASSERT(lvl>=1 && lvl<rank);
      idx_array[lvl-1].push_back(idx_array.back().back());
    }
    auto push_level() {
      static_assert(rank==2);
      push_level(1);
    }

    template<class... Ts0>
    auto push_back(const Ts0&... xs) -> decltype(auto) {
      flat_values.push_back(xs...);
      for (auto& _offset : idx_array) {
        ++_offset.back();
      }
      return flat_values.back();
    }

  // random access
    auto operator[](I i) {
      if constexpr (rank==2) {
        return make_span(flat_values, idx_array[0][i]-off, idx_array[0][i+1]-off); // TODO wrong because returns a view
                                                              // but should return a proxy reference
      } else {
        std::array<knot_span<I>,rank-2> sub_offsets;
        for (int j=1; j<rank-1; ++j) {
          auto inf = std::lower_bound(idx_array[j].data(), idx_array[j].data()+idx_array[j].size(), idx_array[0][i]);
          auto sup = std::lower_bound(idx_array[j].data(), idx_array[j].data()+idx_array[j].size(), idx_array[0][i+1]);
          sub_offsets[j-1] = to_knot_span(span(inf,sup+1));
        }
        auto sub_data = make_span(flat_values,idx_array[0][i],idx_array[0][i+1]);
        return jagged_range<decltype(sub_data),knot_span<I>,rank-1>(sub_data,sub_offsets,idx_array[0][i]); // TODO wrong (same reason: not a proxy ref)
      }
    }
};


namespace detail {
template<class T0, class T1, class I, size_t N> auto
equal_index_array(const std::array<T0,N>& x, I off_x, const std::array<T1,N>& y, I off_y) {
  for (int i=0; i<(int)N; ++i) {
    for (int j=0; j<(int)x[i].size(); ++j) {
      if ( x[i][j]-off_x != y[i][j]-off_y ) return false;
    }
  }
  return true;
}
}

template<class R00, class R01, int R0, class R10, class R11, int R1> auto
operator==(const jagged_range<R00,R01,R0>& x, const jagged_range<R10,R11,R1>& y) -> bool {
  return x.flat_view()==y.flat_view() && detail::equal_index_array(x.index_array(),x.offset(),y.index_array(),y.offset());
}
template<class R00, class R01, int R0, class R10, class R11, int R1> auto
operator!=(const jagged_range<R00,R01,R0>& x, const jagged_range<R10,R11,R1>& y) -> bool {
  return !(x==y);
}


} // std_e
