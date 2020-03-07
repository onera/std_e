#pragma once


#include "std_e/multi_index/concept.hpp"
#include "std_e/multi_index/cartesian_product.hpp"
#include "std_e/multi_index/increment_multi_index.hpp"
#include "std_e/multi_index/multi_index.hpp"
#include "std_e/multi_index/cx_multi_index.hpp"
#include "std_e/future/algorithm.hpp"


namespace std_e {


struct fortran_order_functor {
  template<class Multi_index_0, class Multi_index_1> static constexpr auto
  increment(const Multi_index_0& dims, Multi_index_1& indices) -> void {
    increment_multi_index_fortran_order(dims,indices);
  }
};

struct c_order_functor {
  template<class Multi_index_0, class Multi_index_1> static constexpr auto
  increment(const Multi_index_0& dims, Multi_index_1& indices) -> void {
    increment_multi_index_c_order(dims,indices);
  }
};


template<class Multi_index, class order_functor_type>
// requires order_functor_type has static method increment(const Multi_index_0& dims, Multi_index_1& indices)
class multi_index_generator {
  public:
    using multi_index_type = Multi_index;
    using index_type = index_type_of<multi_index_type>;

  // ctors
    constexpr
    multi_index_generator() = default;

    constexpr
    multi_index_generator(multi_index_type dims)
      : dims(std::move(dims))
      , current_indices{make_zero_multi_index<Multi_index>(dims.size())}
      , current_pos{0}
    {}

  // basic functions
    constexpr auto
    rank() const -> size_t {
      return dims.size();
    }
    constexpr auto
    dimensions() const -> const Multi_index& {
      return dims;
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
      order_functor_type::increment(dims,current_indices);
      return *this;
    }

    constexpr auto
    operator*() const -> const multi_index_type& {
      return current_indices;
    }
  private:
    multi_index_type dims;
    multi_index_type current_indices;
    int current_pos;
};

struct multi_index_generator_sentinel {
  int size;
};

template<class multi_index_generator_type> constexpr auto
operator==(const multi_index_generator_type& gen, multi_index_generator_sentinel sentinel) {
  return gen.current_position()==sentinel.size;
}
template<class multi_index_generator_type> constexpr auto
operator!=(const multi_index_generator_type& gen, multi_index_generator_sentinel sentinel) {
  return !(gen==sentinel);
}


template<class Multi_index, class order_functor_type>
class multi_index_range {
  public:
    using generator_type = multi_index_generator<Multi_index,order_functor_type>;

  // ctor
    constexpr
    multi_index_range() = default;

    constexpr
    multi_index_range(Multi_index dims)
      : generator(std::move(dims))
    {}

  // range interface
    constexpr auto
    begin() const -> const generator_type& {
      return generator;
    }
    constexpr auto
    end() const -> multi_index_generator_sentinel {
      return {cartesian_product(generator.dimensions())};
    }
  private:
    generator_type  generator;
};


template<class Multi_index> constexpr auto
fortran_multi_index_range(Multi_index dims) {
  return multi_index_range<Multi_index,fortran_order_functor>(std::move(dims));
}
template<class Multi_index> constexpr auto
c_multi_index_range(Multi_index dims) {
  return multi_index_range<Multi_index,c_order_functor>(std::move(dims));
}


} // std_e
