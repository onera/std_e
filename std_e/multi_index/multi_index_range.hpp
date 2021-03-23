#pragma once


#include "std_e/multi_index/concept.hpp"
#include "std_e/multi_index/cartesian_product_size.hpp"
#include "std_e/multi_index/increment_multi_index.hpp"
#include "std_e/multi_index/multi_index.hpp"
#include "std_e/multi_index/cx_multi_index.hpp"
#include "std_e/base/not_implemented_exception.hpp"
#include "std_e/future/algorithm.hpp"
#include <iterator>


namespace std_e {


template<class Multi_index>
class fortran_order_functor {
  public:
  // type traits
    using I = index_type_of<Multi_index>;
    using multi_index_type = Multi_index;

  // ctors
    constexpr fortran_order_functor() = default;

    constexpr
    fortran_order_functor(Multi_index dims)
      : dims(std::move(dims))
    {}

  // accessors
    constexpr auto
    rank() const -> int {
      return dims.size();
    }
    constexpr auto
    dimensions() const -> const Multi_index& {
      return dims;
    }

  // increment
    template<class Multi_index_0> constexpr auto
    increment(Multi_index_0& indices) -> void {
      increment_multi_index_fortran_order(dims,indices);
    }

    friend constexpr auto
    operator==(const fortran_order_functor& x, const fortran_order_functor& y) {
      return x.dims==y.dims;
    }
    friend constexpr auto
    operator!=(const fortran_order_functor& x, const fortran_order_functor& y) {
      return !(x==y);
    }
  private:
    Multi_index dims;
};

template<class Multi_index>
class c_order_functor {
  public:
  // type traits
    using I = index_type_of<Multi_index>;
    using multi_index_type = Multi_index;

  // ctors
    constexpr c_order_functor() = default;

    constexpr
    c_order_functor(Multi_index dims)
      : dims(std::move(dims))
    {}

  // accessors
    constexpr auto
    rank() const -> int {
      return dims.size();
    }
    constexpr auto
    dimensions() const -> const Multi_index& {
      return dims;
    }

  // increment
    template<class Multi_index_0> constexpr auto
    increment(Multi_index_0& indices) -> void {
      increment_multi_index_c_order(dims,indices);
    }

    friend constexpr auto
    operator==(const c_order_functor& x, const c_order_functor& y) {
      return x.dims==y.dims;
    }
    friend constexpr auto
    operator!=(const c_order_functor& x, const c_order_functor& y) {
      return !(x==y);
    }
  private:
    Multi_index dims;
};

template<class Multi_index>
class general_order_functor {
  public:
  // type traits
    using multi_index_type = Multi_index;
    using I = index_type_of<Multi_index>;
    static constexpr int ct_rank = rank_of<Multi_index>;

  // ctors
    constexpr general_order_functor() = default;

    constexpr
    general_order_functor(Multi_index dims, multi_index<int,ct_rank> order)
      : dims(std::move(dims))
      , order(std::move(order))
    {}

  // accessors
    static constexpr auto
    rank() -> int {
      return ct_rank;
    }
    constexpr auto
    dimensions() const -> const Multi_index& {
      return dims;
    }

  // increment
    template<class Multi_index_0> constexpr auto
    increment(Multi_index_0& indices) -> void {
      increment_multi_index(dims,indices,order);
    }

    friend constexpr auto
    operator==(const general_order_functor& x, const general_order_functor& y) {
      return x.dims==y.dims  &&  x.order==y.order;
    }
    friend constexpr auto
    operator!=(const general_order_functor& x, const general_order_functor& y) {
      return !(x==y);
    }
  private:
    multi_index_type dims;
    multi_index<int,ct_rank> order;
};


template<class order_functor_type>
// requires order_functor_type has method increment(Multi_index_1& indices)
class multi_index_generator {
  public:
    using multi_index_type = typename order_functor_type::multi_index_type;
    using index_type = index_type_of<multi_index_type>;

  // std::iterator type traits
    using value_type = multi_index_type;
    using difference_type = index_type;
    using iterator_category = std::forward_iterator_tag;

  // ctors
    constexpr
    multi_index_generator() = default;

    constexpr
    multi_index_generator(order_functor_type func)
      : func(std::move(func))
      , current_indices{make_zero_multi_index<multi_index_type>(this->func.rank())}
      , current_pos{0}
    {}

  // basic functions
    constexpr auto
    rank() const -> int {
      return func.rank();
    }
    constexpr auto
    dimensions() const -> const multi_index_type& {
      return func.dimensions();
    }

  // accessor
    constexpr auto
    current_position() const -> int {
      return current_pos;
    }

  // generator interface
    constexpr auto
    operator++() -> multi_index_generator& {
      ++current_pos;
      func.increment(current_indices);
      return *this;
    }
    constexpr auto
    operator++(int) -> multi_index_generator {
      throw not_implemented_exception("don't use postfix operator++");
    }

    constexpr auto
    operator*() const -> const multi_index_type& {
      return current_indices;
    }

    friend constexpr auto
    operator==(const multi_index_generator& x, const multi_index_generator& y) {
      return x.func==y.func  &&  x.current_pos==y.current_pos;
    }
    friend constexpr auto
    operator!=(const multi_index_generator& x, const multi_index_generator& y) {
      return !(x==y);
    }
  private:
    order_functor_type func;
    multi_index_type current_indices;
    int current_pos;
};



template<class Integer>
struct multi_index_generator_sentinel {
  Integer size;
};

template<class multi_index_generator_type, class Integer> constexpr auto
operator==(const multi_index_generator_type& gen, multi_index_generator_sentinel<Integer> sentinel) {
  return gen.current_position()==sentinel.size;
}
template<class multi_index_generator_type, class Integer> constexpr auto
operator==(multi_index_generator_sentinel<Integer> sentinel, const multi_index_generator_type& gen) {
  return gen==sentinel;
}
template<class multi_index_generator_type, class Integer> constexpr auto
operator!=(const multi_index_generator_type& gen, multi_index_generator_sentinel<Integer> sentinel) {
  return !(gen==sentinel);
}
template<class multi_index_generator_type, class Integer> constexpr auto
operator!=(multi_index_generator_sentinel<Integer> sentinel, const multi_index_generator_type& gen) {
  return !(gen==sentinel);
}




template<class order_functor_type>
class multi_index_range {
  public:
    using generator_type = multi_index_generator<order_functor_type>;
    using index_type = typename generator_type::index_type;
    using sentinel_type = multi_index_generator_sentinel<index_type>;

  // ctor
    constexpr
    multi_index_range() = default;

    constexpr
    multi_index_range(order_functor_type func)
      : generator(std::move(func))
      , sz(cartesian_product_size(generator.dimensions()))
    {}

  // range interface
    constexpr auto
    size() const -> index_type {
      return sz;
    }
    constexpr auto
    begin() const -> const generator_type& {
      return generator;
    }
    constexpr auto
    begin() -> generator_type& {
      return generator;
    }
    constexpr auto
    end() const -> sentinel_type {
      return {sz};
    }
    constexpr auto
    end() -> sentinel_type {
      return {sz};
    }
  private:
    generator_type  generator;
    index_type sz;
};


template<
  class Multi_index,
  std::enable_if_t< is_multi_index<Multi_index> , int > =0
> constexpr auto
fortran_multi_index_range(Multi_index dims) {
  fortran_order_functor func(std::move(dims));
  return multi_index_range(std::move(func));
}
template<
  class Multi_index,
  std::enable_if_t< is_multi_index<Multi_index> , int > =0
> constexpr auto
c_multi_index_range(Multi_index dims) {
  c_order_functor func(std::move(dims));
  return multi_index_range(std::move(func));
}
template<
  class Multi_index,
  std::enable_if_t< is_multi_index<Multi_index> , int > =0,
  int rank
> constexpr auto
multi_index_range_with_order(Multi_index dims, multi_index<int,rank> order) {
  general_order_functor<Multi_index> func(std::move(dims),std::move(order));
  return multi_index_range(std::move(func));
}

// overloads matching initialization lists {
template<class Integer, int N> constexpr auto
fortran_multi_index_range(const Integer(&dims)[N]) {
  multi_index<Integer,N> ds = {};  std_e::copy_n(dims,N,begin(ds));
  return fortran_multi_index_range(ds);
}
template<class Integer, int N> constexpr auto
c_multi_index_range(const Integer(&dims)[N]) {
  multi_index<Integer,N> ds = {};  std_e::copy_n(dims,N,begin(ds));
  return c_multi_index_range(ds);
}
template<class Integer, int N> constexpr auto
multi_index_range_with_order(const Integer(&dims)[N], const int(&order)[N]) {
  multi_index<Integer,N> ds  = {};  std::copy_n(dims ,N,begin(ds ));
  multi_index<Integer,N> ord = {};  std::copy_n(order,N,begin(ord));
  return multi_index_range_with_order(ds,ord);
}
// overloads matching initialization lists }


} // std_e
