#pragma once


#include <vector>
#include "std_e/algorithm/permutation.hpp"
#include "std_e/future/contract.hpp"
#include "std_e/data_structure/heterogenous_vector.hpp"


namespace std_e {


template<class... Ts>
class table {
  public:
  // type traits
    using row_ref = std::tuple<Ts&...>;
    using row_const_ref = std::tuple<const Ts&...>;
    using index_type = std::ptrdiff_t;
    using impl_type = hvector<Ts...>;

  // ctors
    table() = default;

    table(index_type sz)
      : table(sz,std::make_index_sequence<nb_cols()>())
    {}

  // size
    static constexpr auto
    nb_cols() {
      return sizeof...(Ts);
    }
    auto
    nb_rows() const {
      return get<0>(_impl).size();
    }

    template<int col_index> auto
    col() const -> const auto& {
      return get<col_index>(_impl);
    }
    template<int col_index> auto
    col() -> auto& {
      return get<col_index>(_impl);
    }
    template<class T> auto
    col() const -> const auto& {
      return get<T>(_impl);
    }
    template<class T> auto
    col() -> auto& {
      return get<T>(_impl);
    }

    auto
    operator[](index_type i) const -> row_const_ref {
      return this->subscript_op__impl(i,std::make_index_sequence<nb_cols()>());
    }
    auto
    operator[](index_type i) -> row_ref {
      return this->subscript_op__impl(i,std::make_index_sequence<nb_cols()>());
    }

  // low-level access
    constexpr auto
    impl() -> impl_type& {
      return _impl;
    }
    constexpr auto
    impl() const -> const impl_type& {
      return _impl;
    }

    template<int col_index, class T> constexpr auto
    // requires T==Ts[col_index]
    find_row_index(const T& x) const -> index_type {
      static_assert(col_index < nb_cols());
      const auto& col = get<col_index>(_impl);
      auto it = std::find(begin(col),end(col),x);
      return it-begin(col);
    }
    template<int col_index, class T> constexpr auto
    // requires T==Ts[col_index]
    find_row(const T& x) const -> row_const_ref {
      static_assert(col_index < nb_cols());
      index_type i = this->template find_row_index<col_index>(x);
      return (*this)[i];
    }
    template<int col_index, class T> constexpr auto
    // requires T==Ts[col_index]
    find_row(T& x) -> row_ref {
      static_assert(col_index < nb_cols());
      index_type i = this->template find_row_index<col_index>(x);
      return (*this)[i];
    }
    template<int search_col_index, int found_col_index, class T> constexpr auto
    // requires T==Ts[search_col_index]
    find_cell(const T& x) const -> const auto& {
      static_assert(search_col_index < nb_cols());
      static_assert(found_col_index < nb_cols());
      index_type i = find_row_index<search_col_index>(x);
      return get<found_col_index>(_impl)[i];
    }
    template<int search_col_index, int found_col_index, class T> constexpr auto
    // requires T==Ts[search_col_index]
    find_cell(const T& x) -> auto& {
      static_assert(search_col_index < nb_cols());
      static_assert(found_col_index < nb_cols());
      index_type i = find_row_index<search_col_index>(x);
      return get<found_col_index>(_impl)[i];
    }

  // vector-like interface
    template<class... Ts0> auto
    // requires Ts0 is Ts
    push_back(const Ts0&... elts) {
      constexpr size_t nnn = size_t(nb_cols());
      return push_back__impl(std::forward_as_tuple(elts...),std::make_index_sequence<nnn>());
    }
  private:
  // methods
    template<size_t... Is>
    table(index_type sz, std::index_sequence<Is...>)
      : _impl(std::vector<std::tuple_element_t<Is,std::tuple<Ts...>>>(sz)...)
    {}
    template<size_t... Is> constexpr auto
    // requires T==Ts[col_index]
    subscript_op__impl(index_type i, std::index_sequence<Is...>) const -> row_const_ref {
      return {get<Is>(_impl)[i]...};
    }
    template<size_t... Is> constexpr auto
    // requires T==Ts[col_index]
    subscript_op__impl(index_type i, std::index_sequence<Is...>) -> row_ref {
      return {get<Is>(_impl)[i]...};
    }
    template<class args_tuple_type, size_t... Is> auto
    // requires args_tuple_type is tuple<Ts...>
    push_back__impl(const args_tuple_type& elts, std::index_sequence<Is...>) {
      (get<Is>(_impl).push_back(std::get<Is>(elts)),...);
    }
  // data member
    hvector<Ts...> _impl;
};


template<int j, class... Ts> auto
col(const table<Ts...>& x) -> const auto& {
  return x.template col<j>();
}
template<int j, class... Ts> auto
col(table<Ts...>& x) -> auto& {
  return x.template col<j>();
}
template<class T, class... Ts> auto
col(const table<Ts...>& x) -> const auto& {
  return x.template col<T>();
}
template<class T, class... Ts> auto
col(table<Ts...>& x) -> auto& {
  return x.template col<T>();
}

template<class... Ts, class I> auto
row(const table<Ts...>& x, I i) {
  return x[i];
}
template<class... Ts, class I> auto
row(table<Ts...>& x, I i) {
  return x[i];
}

template<int j, class... Ts, class I> auto
cell(const table<Ts...>& x, I i) -> const auto& {
  return col<j>(x)[i];
}
template<int j, class... Ts, class I> auto
cell(table<Ts...>& x, I i) -> auto& {
  return col<j>(x)[i];
}
template<class T, class... Ts, class I> auto
cell(const table<Ts...>& x, I i) -> const auto& {
  return col<T>(x)[i];
}
template<class T, class... Ts, class I> auto
cell(table<Ts...>& x, I i) -> auto& {
  return col<T>(x)[i];
}

template<int col_index, class... Ts, class T> constexpr auto
// requires T==Ts[col_index]
find_row(const table<Ts...>& x, const T& value) {
  return x.template find_row<col_index>(value);
}
template<int col_index, class... Ts, class T> constexpr auto
// requires T==Ts[col_index]
find_row(table<Ts...>& x, const T& value) {
  return x.template find_row<col_index>(value);
}

template<int search_col_index, int found_col_index, class... Ts, class T> constexpr auto
// requires T==Ts[col_index]
find_cell(const table<Ts...>& x, const T& value) -> const auto& {
  return x.template find_cell<search_col_index,found_col_index>(value);
}
template<int search_col_index, int found_col_index, class... Ts, class T> constexpr auto
// requires T==Ts[col_index]
find_cell(table<Ts...>& x, const T& value) -> auto& {
  return x.template find_cell<search_col_index,found_col_index>(value);
}


template<int found_col_index>
struct Find_type {
  template<int search_col_index, class table_type, class T> auto
  from(table_type& x, const T& value) -> auto& {
    return find_cell<search_col_index,found_col_index>(x,value);
  }
};
template<int found_col_index> Find_type<found_col_index> find;



} // std_e

//#define STD_E_GEN_UTILITY(table_type_name,table_type,__VA_ARGS__) \
//  using table_type_name = table_type;

