#pragma once


#include <ranges>
#include <array>
#include "std_e/future/dynarray.hpp"
#include "std_e/multi_index/fortran_order.hpp"
#include "std_e/future/span_ref.hpp"
#include "std_e/field/concepts.hpp"
#include "std_e/debug.hpp"


namespace std_e {


template<class T, int... Ns> 
struct md_field_uniform {
  public:
    // TODO factor with md_field_base
    using value_type = T;
    static constexpr int rank = sizeof...(Ns);
    static constexpr std::array<int,rank> dims = {Ns...};
    static constexpr int dim_tot = (Ns * ... * 1);

    md_field_uniform(std::initializer_list<T>&& l)
    {
      static_assert(this->rank==1);
      int i=0;
      for (const value_type& x : l) {
        (*this)(i) = x;
        ++i;
      }
    }
    md_field_uniform(std::initializer_list<std::initializer_list<T>>&& ll)
    {
      static_assert(this->rank==2);
      int i=0;
      for (const auto& l : ll) {
        int j=0;
        for (const value_type& x : l) {
          (*this)(i,j) = x;
          ++j;
        }
        ++i;
      }
    }

    template<class... Is> auto
    operator()(Is... is) const -> const value_type& {
      static_assert(sizeof...(Is) == this->rank);
      auto i = index_of_field(is...);
      return _values[i];
    }
    template<class... Is> auto
    operator()(Is... is) -> value_type& {
      static_assert(sizeof...(Is) == this->rank);
      auto i = index_of_field(is...);
      return _values[i];
    }

  private:
  // member functions
    template<class I, class... Is> auto
    index_of_field(I i, Is... is) const {
      return std_e::fortran_order_from_dimensions(dims, std_e::multi_index<I>{i,is...});
    }

  // data members
    std::array<T, dim_tot> _values;
};

template<class T> class field_view;

template<class Array_type>
class field_base : public Array_type {
  public:
    using base = Array_type;
    using base::base;
    using value_type = typename base::value_type;
    
    static constexpr int rank = 0;
    static constexpr int dim_tot = 1;
    static constexpr std::array<int,0> dims = {};

    field_base(base x)
      : base(std::move(x))
    {}

    auto
    n_element() const -> int64_t {
      return this->size();
    }

    template<class I> auto
    operator()(I i) const -> const value_type& {
      return (*this)[i];
    }
    template<class I> auto
    operator()(I i) -> value_type& {
      return (*this)[i];
    }

    template<class I> auto
    sub(I n) -> field_view<value_type> {
      value_type* ptr = this->data();
      return std_e::make_span(ptr, ptr+n);
    }
    template<class I> auto
    sub(I n) const -> field_view<const value_type> {
      const value_type* ptr = this->data();
      return std_e::make_span(ptr, ptr+n);
    }

};
template<class T, class A = mallocator>
class field : public field_base<std_e::dynarray<T,A>> {
  public:
    using base = field_base<std_e::dynarray<T,A>>;
    using base::base;
};

template<class T>
class field_view : public field_base<std_e::span<T>> {
    using base = field_base<std_e::span<T>>;
    using base::base;

    field_view(base x)
      : base(x)
    {}
};
template<class T>
class field_ref : public field_base<std_e::span_ref<T>> {
    using base = field_base<std_e::span_ref<T>>;
    using base::base;
};


template<class Array_type, int... Ns>
class md_field_base {
  public:
    using T = typename Array_type::value_type;
    using value_type = T;
    static constexpr int rank = sizeof...(Ns);
    static constexpr std::array<int,rank> dims = {Ns...};
    static constexpr int dim_tot = (Ns * ... * 1);

    template<class... Is> auto
    underlying(Is... is)       ->       auto& {
      static_assert(sizeof...(Is) == this->rank);
      auto i = index_of_field(is...);
      return _arrays[i];
    }
    template<class... Is> auto
    underlying(Is... is) const -> const auto& {
      static_assert(sizeof...(Is) == this->rank);
      auto i = index_of_field(is...);
      return _arrays[i];
    }

    template<class... Is> auto
    field(Is... is) -> field_ref<T> {
      static_assert(sizeof...(Is) == this->rank);
      auto i = index_of_field(is...);
      return std_e::make_span(_arrays[i].begin(), _arrays[i].end());
    }
    template<class... Is> auto
    field(Is... is) const -> field_ref<const T> {
      static_assert(sizeof...(Is) == this->rank);
      auto i = index_of_field(is...);
      return std_e::make_span(_arrays[i].begin(), _arrays[i].end());
    }
    template<class... Is> auto
    data(Is... is) -> T* {
      static_assert(sizeof...(Is) == this->rank);
      return this->field(is...).data();
    }
    template<class... Is> auto
    data(Is... is) const -> const T* {
      static_assert(sizeof...(Is) == this->rank);
      return this->field(is...).data();
    }

    template<class I, class... Is> auto
    operator()(I fld_idx, Is... is) const -> const T& {
      static_assert(sizeof...(Is) == this->rank);
      return field(is...)[fld_idx];
    }
    template<class I, class... Is> auto
    operator()(I fld_idx, Is... is) -> T& {
      static_assert(sizeof...(Is) == this->rank);
      return field(is...)[fld_idx];
    }

    auto
    n_element() const -> int64_t {
      return _arrays[0].size();
    }

    auto underlying()       ->       auto& { return _arrays; }
    auto underlying() const -> const auto& { return _arrays; }

    auto underlying_linear(int i)       ->       auto& { return _arrays[i]; }
    auto underlying_linear(int i) const -> const auto& { return _arrays[i]; }

    auto begin()       { return _arrays.begin(); }
    auto begin() const { return _arrays.begin(); }
    auto end  ()       { return _arrays.end  (); }
    auto end  () const { return _arrays.end  (); }

  private:
  // member functions
    template<class I, class... Is> auto
    index_of_field(I i, Is... is) const {
      return std_e::fortran_order_from_dimensions(dims, std_e::multi_index<I>{i,is...});
    }

  // data members
    std::array<Array_type, dim_tot> _arrays;
};

template<class AT, int... Ns> auto
to_string(const md_field_base<AT,Ns...>& x) -> std::string {
  return to_string(x.underlying());
}

template<class T, class A, int... Ns>
class md_field : public md_field_base<std_e::dynarray<T,A>, Ns...> {
  public:
    using array_1d_t = std_e::dynarray<T,A>;
    using base = md_field_base<array_1d_t, Ns...>;

    md_field() = default;

    md_field(size_t n)
    {
      std::ranges::fill(this->underlying(), array_1d_t(n));
    }

    md_field(size_t n, T value)
    {
      for (int i=0; i<this->dim_tot; ++i) {
        this->underlying_linear(i) = array_1d_t(n,value);
      }
    }

    md_field(size_t n, const md_field_uniform<T,Ns...>& uni_fld)
    {
      for (int i=0; i<this->dim_tot; ++i) {
        this->underlying_linear(i) = array_1d_t(n,uni_fld(i));
      }
    }

    template<class... Is> auto
    reclaim(Is... is) -> array_1d_t&& {
      return std::move(this->underlying(is...));
    }
};

template<class T, int... Ns>
class md_field_view : public md_field_base<std_e::span<T>, Ns...> {
  public:
    using array_1d_t = std_e::span<T>;
    using base = md_field_base<array_1d_t, Ns...>;

    md_field_view() = default;

    template<class Md_field_type>
    md_field_view(Md_field_type& x)
    {
      for (int i=0; i<this->dim_tot; ++i) {
        this->underlying_linear(i) = std_e::make_span(x.underlying_linear(i));
      }
    }
};

template<class Md_field> auto
row(Md_field& x, int i) {
  using FT = std::decay_t<Md_field>;
  using T = typename FT::value_type;
  if constexpr (x.rank == 1) {
    return x.field(i);
  } else if constexpr (x.rank == 2) {
    md_field_view<T, x.dims[1]> res;
    for (int j=0; j<x.dims[1]; ++j) {
      res.underlying(j) = x.underlying(i,j);
    }
    return res;
  } else {
    throw std_e::msg_exception("Not implemented");
  }
}

template<class Md_field_0, class Md_field_1, class Md_field_2> auto
v_stack(Md_field_0& x, Md_field_1& y, Md_field_2& z) {
  using FT0 = std::decay_t<Md_field_0>;
  using FT1 = std::decay_t<Md_field_1>;
  using FT2 = std::decay_t<Md_field_2>;
  using T0 = typename FT0::value_type;
  using T1 = typename FT1::value_type;
  using T2 = typename FT2::value_type;
  static_assert(std::is_same_v<T0,T1>);
  static_assert(std::is_same_v<T0,T2>);
  //static_assert(x.rank <= 2);
  //static_assert(y.rank <= 2);
  if constexpr (x.rank == 0 && y.rank == 0 && z.rank == 0) {
    md_field_view<T0, 3> res;
    res.underlying(0) = x;
    res.underlying(1) = y;
    res.underlying(2) = z;
    return res;
  } else if constexpr (x.rank == 0 && y.rank == 1 && z.rank == 0) {
    constexpr int n_row = 1+y.dims[0]+1;
    md_field_view<T0, n_row> res;
    res.underlying(0) = x;
    for (int i=0; i<n_row-2; ++i) {
      res.underlying(1+i) = y.underlying(i);
    }
    res.underlying(n_row-1) = z;
    return res;
  } else if constexpr (x.rank == 1 && y.rank == 2 && z.rank == 1) {
    static_assert(x.dims[0] == y.dims[1]);
    static_assert(z.dims[0] == y.dims[1]);
    constexpr int n_row = 1+y.dims[0]+1;
    constexpr int n_col = x.dims[0];
    md_field_view<T0, n_row, n_col> res;
    for (int j=0; j<n_col; ++j) {
      res.underlying(0,j) = x.underlying(j);
    }
    for (int j=0; j<n_col; ++j) {
      for (int i=0; i<n_row-2; ++i) {
        res.underlying(1+i,j) = y.underlying(i,j);
      }
    }
    for (int j=0; j<n_col; ++j) {
      res.underlying(n_row-1,j) = z.underlying(j);
    }
    return res;
  } else {
    throw std_e::msg_exception("Not implemented");
  }
}

// Double precision types
struct scalar_field : field<double, mallocator> {
  using base = field<double, mallocator>;
  using base::base;
};
template<int N> 
struct vector_field : md_field<double, mallocator, N> {
  using base = md_field<double, mallocator, N>;
  using base::base;
};

template<int N0, int N1> 
struct tensor_field : md_field<double, mallocator, N0, N1> {
  using base = md_field<double, mallocator, N0, N1>;
  using base::base;
};
struct scalar_field_view : field_view<double> {
  using base = field_view<double>;
  using base::base;
};
template<int N> 
struct vector_field_view : md_field_view<double, N> {
  using base = md_field_view<double, N>;
  using base::base;
  vector_field_view(base x)
    : base(std::move(x))
  {}
};
template<int N0, int N1> 
struct tensor_field_view : md_field_view<double, N0, N1> {
  using base = md_field_view<double, N0, N1>;
  using base::base;
};

template<int N> 
struct vector_field_uniform : md_field_uniform<double, N> {
  using base = md_field_uniform<double, N>;
  using base::base;
};
template<int N0, int N1> 
struct tensor_field_uniform : md_field_uniform<double, N0, N1> {
  using base = md_field_uniform<double, N0, N1>;
  using base::base;
};

//// Simple precision types
//struct scalar_field_f : field<float> {
//  using base = field<float>;
//  using base::base;
//};
//template<int N>
//struct vector_field_f : md_field<float, N> {
//  using base = md_field<float, N>;
//  using base::base;
//};
//template<int N0, int N1> 
//struct tensor_field_f : md_field<float, N0, N1> {
//  using base = md_field<float, N0, N1>;
//  using base::base;
//};
//struct scalar_field_view_f : field_view<float> {
//  using base = field_view<float>;
//  using base::base;
//};
//template<int N> 
//struct vector_field_view_f : md_field_view<float, N> {
//  using base = md_field_view<float, N>;
//  using base::base;
//};
//template<int N0, int N1> 
//struct tensor_field_view_f : md_field_view<float, N0, N1> {
//  using base = md_field_view<float, N0, N1>;
//  using base::base;
//};

} // std_e
