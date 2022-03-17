#pragma once

#include "std_e/future/span.hpp"
#include "std_e/interval/interval_sequence.hpp"
#include <vector>
#include <algorithm>
#include "std_e/data_structure/multi_range.hpp"
#include "std_e/data_structure/block_range/vblock_iterator.hpp"
#include "std_e/meta/meta.hpp"
// TODO clean up!
// TODO jagged -> compressed


namespace std_e {

template<class data_range_type, class indices_range_type, int rank>
class jagged_range;

template<class T, int rank=2, class I=int>
using jagged_span = jagged_range<span<T>,span<I>,rank>;
template<class T, int rank=2, class I=int>
using jagged_vector = jagged_range<std::vector<T>,std::vector<I>,rank>;

template<int rank, class... Ts>
using jagged_multi_vector = jagged_range<multi_vector<Ts...>,std::vector<int>,rank>;

template<class multi_vector_type, int rank=2>
using jagged_multi_vector2 = jagged_range<multi_vector_type,std::vector<int>,rank>;



struct unambiguous {};
// jagged_range of rank 1: just a regular range
template<class data_range_type, class indices_range_type>
class jagged_range<data_range_type,indices_range_type,1> : public data_range_type { // TODO clean
  public:
    static constexpr int rank = 1;
    using base = data_range_type;
    using base::base;
    jagged_range(base x)
      : base(std::move(x))
    {}
    auto total_size() const { return base::size(); }
    auto flat_view() const -> const base& {
      return *this;
    }
};
template<class R00, class R01, class R10, class R11> auto
operator==(const jagged_range<R00,R01,1>& x, const jagged_range<R10,R11,1>& y) -> bool {
  const R00& x_base = x;
  const R10& y_base = y;
  return x_base==y_base;
}
template<class R00, class R01, class R10, class R11> auto
operator!=(const jagged_range<R00,R01,1>& x, const jagged_range<R10,R11,1>& y) -> bool {
  return !(x==y);
}


template<class indices_range_type, int rank>
struct indices_array_type_impl {
  using type = jagged_range<indices_range_type,indices_range_type,rank-1>;
};
template<class indices_range_type>
struct indices_array_type_impl<indices_range_type,2> {
  using type = indices_range_type;
};

template<class data_range_type, class indices_range_type, int r>
class jagged_range {
  public:
    static constexpr int rank = r;
  private:
    //using indices_array_type = std::array<indices_range_type,rank-1>;
    using indices_array_type = typename indices_array_type_impl<indices_range_type,rank>::type;
    using T = typename std::remove_cvref_t<data_range_type>::value_type;
    using I = typename std::remove_cvref_t<indices_range_type>::value_type;
    using data_value_range_type = data_range_type;

    data_range_type flat_values;
    indices_array_type idx_array; // TODO rename idces
    I off = 0; // necessary to handle sub-ranges TODO maybe intermediate struct to store flat_values.data()-offset?
  public:
  // type traits
    using scalar_type = T;
    using indices_type = I;

    using iterator = vblock_iterator<T,I>;
    using const_iterator = vblock_iterator<const T,const I>;
    using value_type = typename iterator::value_type;
    using reference = typename iterator::reference;
    using const_reference = typename const_iterator::reference;

  // ctors
    jagged_range() requires (rank>2)
      : idx_array({0},unambiguous{})
    {}
    jagged_range() requires (rank==2)
      : idx_array({0})
    {}

    // TODO private (for default ctor only)
    jagged_range(data_range_type flat_values, unambiguous) requires (rank>2)
      : flat_values(std::forward<data_range_type>(flat_values))
      , idx_array({0},unambiguous{})
    {}
    jagged_range(data_range_type flat_values, unambiguous) requires (rank==2)
      : flat_values(std::forward<data_range_type>(flat_values))
      , idx_array({0})
    {}

    jagged_range(data_range_type flat_values, indices_range_type idx_array, I off = 0)
      : flat_values(std::forward<data_range_type>(flat_values))
      , idx_array(std::forward<indices_range_type>(idx_array))
      , off(off)
    {
      static_assert(rank==2);
    }
    // same as above, but by copy from different types (e.g. Range==span, data_range_type==vector)
    template<class Range0, class Range1>
    jagged_range(Range0&& flat_values, Range1&& idx_array, I off = 0)
      : flat_values(flat_values.begin(),flat_values.end())
      , idx_array(idx_array.begin(),idx_array.end())
      , off(off)
    {
      static_assert(rank==2);
    }
    template<class Range0, class Range1, class Range2>
    jagged_range(const Range0& flat_values, const Range1& separators0, const Range2& separators1, I off = 0)
      : flat_values(flat_values.begin(),flat_values.end())
      , idx_array(separators0,separators1)
      , off(off)
    {
      static_assert(rank==3);
    }

    jagged_range(std::initializer_list<std::initializer_list<T>> ll)
      : jagged_range()
    {
      static_assert(rank==2);
      for (const auto& l : ll) {
        separator<0>();
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
        separator<1>();
        for (const auto& l : ll) {
          separator<0>();
          for (const T& v : l) {
            push_back(v);
          }
        }
      }
    }

  // basic
    auto size() const -> I {
      return idx_array.size()-1;
    }
    auto data() const -> const T* {
      return flat_values.data();
    }
    auto data() -> T* {
      return flat_values.data();
    }
    auto total_size() const -> I {
      return flat_values.size();
    }

  // range
    auto begin()       ->       iterator { return { data() , idx_array.data()                    }; }
    auto begin() const -> const_iterator { return { data() , idx_array.data()                    }; }
    auto end()         ->       iterator { return { data() , idx_array.data()+idx_array.size()-1 }; }
    auto end()   const -> const_iterator { return { data() , idx_array.data()+idx_array.size()-1 }; }

    auto operator[](I i)       { return subscript_op_impl(*this,i); }
    auto operator[](I i) const { return subscript_op_impl(*this,i); }

  // accessors
    auto flat_view() {
      return make_span(flat_values);
    }
    auto flat_view() const {
      return make_span(flat_values);
    }
    auto flat_ref() -> auto& {
      return flat_values;
    }
    auto flat_ref() const -> const auto& {
      return flat_values;
    }
    auto retrieve_values() -> std::vector<T> {
      return std::move(flat_values);
    }

    auto index_array() const -> const auto& {
      return idx_array;
    }
    auto indices() const -> interval_span<const I> {
      static_assert(rank==2);
      return to_interval_span(make_span(idx_array));
    }
    template<int lvl>
    auto indices() const -> interval_span<const I> {
      static_assert(lvl>=0 && lvl<rank);
      return indices_impl<lvl+1>();
    }
    auto indices_ref() -> auto& {
      static_assert(rank==2);
      return idx_array;
    }
    template<int lvl>
    auto indices_ref() -> auto& {
      static_assert(lvl>=0 && lvl<rank);
      return indices_ref_impl<lvl+1>();
    }

    auto offset() const -> I {
      return off;
    }

  // container interface
    template<int lvl>
    auto separator() {
      static_assert(lvl>=0 && lvl<rank);
      if constexpr (lvl==0) {
        idx_array.push_back(idx_array.back());
      } else {
        idx_array.template separator<lvl-1>();
      }
    }
    auto push_level() { // TODO rename separator
      static_assert(rank==2);
      separator<0>();
    }

    auto back() -> decltype(auto) {
      return flat_values.back();
    }
    auto back() const -> decltype(auto) {
      return flat_values.back();
    }
    template<class... Ts0>
    auto push_back(const Ts0&... xs) -> decltype(auto) {
      flat_values.push_back(xs...);
      ++idx_array.back();
      return back();
    }

  private:
    template<class R0, class R1, int R> friend class jagged_range;
    template<int lvl>
    auto indices_impl() const -> interval_span<const I> {
      if constexpr (lvl==0) {
        return to_interval_span(flat_view());
      } else if constexpr (rank==2 && lvl==1) {
        return indices();
      } else {
        return idx_array.template indices_impl<lvl-1>();
      }
    }
    template<int lvl>
    auto indices_ref_impl() -> auto& {
      if constexpr (lvl==0) {
        return flat_ref();
      } else if constexpr (rank==2 && lvl==1) {
        return indices();
      } else {
        return idx_array.template indices_ref_impl<lvl-1>();
      }
    }

    template<class jagged_range_type>
    static auto subscript_op_impl(jagged_range_type& x, I i) {
      if constexpr (rank==2) {
        using data_value_type = typename jagged_range_type::data_value_range_type;
        if constexpr (!is_multi_range<data_value_type>) {
          if constexpr (std::is_const_v<jagged_range_type> || std::is_const_v<element_type<data_value_type>>) {
            return const_reference(x.flat_values.data()+x.idx_array[i]-x.off, x.idx_array.data()+i);
          } else {
            return       reference(x.flat_values.data()+x.idx_array[i]-x.off, x.idx_array.data()+i);
          }
        } else { // FIXME return as previous (not possible now because `reference` does not work with proxy-refs of multi_range
          return make_span_ref(x.flat_values, x.idx_array[i]-x.off, x.idx_array[i+1]-x.off);
          //return make_span_ref(x.flat_values.data()+x.idx_array[i]-x.off, x.idx_array[i+1]-x.idx_array[i]);
        }
      } else {
        static_assert(rank==3); // Other ranks are not implemented TODO
        auto idx_2 = x.idx_array.indices();
        auto idx_1 = x.idx_array.flat_view();
        jagged_range<span<I>,span<I>,rank-2> sub_idx_array( make_span(idx_1,idx_2[i],idx_2[i+1]+1) );
        auto sub_data = make_span(x.flat_values,sub_idx_array[0],sub_idx_array.back());
        return jagged_range<decltype(sub_data),span<I>,rank-1>(sub_data,sub_idx_array,idx_1[idx_2[i]]);
                                                                          // TODO wrong because returns a view
                                                                          // but should return a proxy reference
      }
    }
};

template<int lvl, class R00, class R01, int R0> auto
indices(const jagged_range<R00,R01,R0>& x) {
  return x.template indices<lvl>();
}
template<int lvl, class R00, class R01, int R0> auto
indices_ref(jagged_range<R00,R01,R0>& x) -> auto& {
  return x.template indices_ref<lvl>();
}
template<int lvl, class R00, class R01, int R0> auto
separator(jagged_range<R00,R01,R0>& x) {
  return x.template separator<lvl>();
}

template<int R, class T0, class T1, class I> auto
_equal_index_array(const T0& x, I off_x, const T1& y, I off_y) {
  if constexpr (R==1) {
    for (int j=0; j<(int)x.size(); ++j) {
      if ( x[j]-off_x != y[j]-off_y ) return false;
    }
    return true;
  } else {
    for (int j=0; j<(int)x.total_size(); ++j) {
      if ( x.flat_view()[j]-off_x != y.flat_view()[j]-off_y ) return false;
    }
    return x.index_array()==y.index_array();
  }
}

template<class R00, class R01, class R10, class R11, int R> auto
operator==(const jagged_range<R00,R01,R>& x, const jagged_range<R10,R11,R>& y) -> bool {
  return x.flat_view()==y.flat_view() && _equal_index_array<R-1>(x.index_array(),x.offset(),y.index_array(),y.offset());
}
template<class R00, class R01, class R10, class R11, int R> auto
operator!=(const jagged_range<R00,R01,R>& x, const jagged_range<R10,R11,R>& y) -> bool {
  return !(x==y);
}
template<class R0, class R1> auto
to_string(const jagged_range<R0,R1,2>& x) -> std::string {
  std::string s = "[";
  for (int i=0; i<x.size()-1; ++i) {
    s += to_string(x[i]) + ",";
  }
  if (x.size()>0) {
    s += to_string(x[x.size()-1]);
  }
  s += "]";
  return s;
}

// algorithm {
template<class Range0, class Range1> auto
upscale_separators(Range0& upper_separators, const Range1& lower_separators) -> void {
  for (auto& sep : upper_separators) {
    sep = lower_separators[sep];
  }
}
template<class Range0, class Range1> auto
downscale_separators(Range0& upper_separators, const Range1& lower_separators) -> void {
  auto first = begin(upper_separators);
  auto last = end(upper_separators);
  auto lfirst = begin(lower_separators);
  auto lcur = lfirst;
  auto llast = end(lower_separators);
  while (first!=last) {
    lcur = std::find(lcur,llast,*first);
    *first++ = lcur-lfirst;
  }
}
template<class Range0, class Range1, class T = typename Range0::value_type> auto
upscaled_separators(Range0& upper_separators, const Range1& lower_separators) -> interval_vector<T> {
  interval_vector<T> upscaled_seps(begin(upper_separators),end(upper_separators));
  upscale_separators(upscaled_seps,lower_separators);
  return upscaled_seps;
}
template<class Range0, class Range1, class T = typename Range0::value_type> auto
downscaled_separators(Range0& upper_separators, const Range1& lower_separators) -> interval_vector<T> {
  interval_vector<T> downscaled_seps(begin(upper_separators),end(upper_separators));
  downscale_separators(downscaled_seps,lower_separators);
  return downscaled_seps;
}

template<class R00, class R01, class T = typename R00::value_type, class I = typename R01::value_type> auto
flattened_last_level_view(const jagged_range<R00,R01,3>& x) -> jagged_span<const T,2,const I> {
  return jagged_span<const T,2,const I>(x.flat_view(),indices<0>(x));
}
template<class R00, class R01, class T = typename R00::value_type, class I = typename R01::value_type> auto
flatten_last_level(jagged_range<R00,R01,3> x) -> jagged_range<R00,R01,2> {
  return jagged_range<R00,R01,2>(std::move(x.flat_ref()),std::move(indices_ref<0>(x)));
}
template<class R00, class R01, class Knot_sequence, class T = typename R00::value_type, class I = typename R01::value_type> auto
view_with_new_level(const jagged_range<R00,R01,2>& x, const Knot_sequence& new_level) -> jagged_span<const T,3,const I> {
  return jagged_span<const T,3,const I>(x.flat_view(),x.indices(),to_interval_span(new_level));
}

template<
  class R00, class R01, int rank, class F,
  class T = typename R00::value_type, class I = typename R01::value_type,
  class RT = decltype(std::declval<F>()(std::declval<T>()))
> auto
transform(const jagged_range<R00,R01,rank>& x, F f) -> jagged_vector<RT,rank,I> {
  std::vector<RT> res(x.total_size());;
  std::transform(
    begin(x.flat_view()),end(x.flat_view()),
    begin(res),
    f
  );
  return {res,x.indices()};
}
// algorithm }

} // std_e
