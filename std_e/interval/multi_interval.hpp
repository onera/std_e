#pragma once


#include "std_e/base/dynamic_size.hpp"
#include "std_e/multi_index/multi_index.hpp"
#include "std_e/interval/interval.hpp"
#include <bits/ranges_algo.h>


namespace std_e {


template<class Integer, int ct_rank = dynamic_size>
struct multi_interval {
  public:
    using multi_index_type = multi_index<Integer,ct_rank>;
    using interval_ref = std_e::interval<Integer&>;
    using interval_const_ref = std_e::interval<const Integer&>;

    multi_interval(int rk)
      : first_(rk)
      , last_(rk)
    {}
    multi_interval(multi_index_type first_, multi_index_type last_)
      : first_(std::move(first_))
      , last_(std::move(last_))
    {}

    auto
    rank() const -> int {
      return first_.size();
    }

    auto first() const -> const multi_index_type& { return first_; }
    auto first()       ->       multi_index_type& { return first_; }
    auto last () const -> const multi_index_type& { return last_ ; }
    auto last ()       ->       multi_index_type& { return last_ ; }

    auto
    operator[](int i) -> interval_ref {
      return {first_[i],last_[i]};
    }
    auto
    operator[](int i) const -> interval_const_ref {
      return {first_[i],last_[i]};
    }

  private:
    multi_index_type first_;
    multi_index_type last_;
};

template<class I, int ct_rank> auto
length(const multi_interval<I,ct_rank>& x) {
  using len_type = typename multi_interval<I,ct_rank>::multi_index_type;
  len_type res(x.rank());
  for (int i=0; i<x.rank(); ++i) {
    res[i] = length(x[i]);
  }
  return res;
}


} // std_e
