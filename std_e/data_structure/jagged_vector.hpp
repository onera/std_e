#pragma once


#include "std_e/future/span.hpp"
#include "std_e/interval/knot_sequence.hpp"
#include <vector>


namespace std_e {

// TODO jagged_table, jagged_array_3 ...
template<class T, class I=int>
class jagged_vector {
  public:
    jagged_vector()
      : _offsets({0})
    {}

    jagged_vector(std::vector<T> flat_values, knot_vector<I>_offsets)
      : flat_values(std::move(flat_values))
      , _offsets(std::move(_offsets))
    {}
    
    jagged_vector(std::initializer_list<std::initializer_list<T>> ll)
      : jagged_vector()
    {
      for (const auto& l : ll) {
        push_back();
        for (const T& v : l) {
          push_back(v);
        }
      }
    }

    auto size() const -> I {
      return _offsets.nb_intervals();
    }
    auto data() const -> const T* {
      return flat_values.data();
    }
    auto data() -> T* {
      return flat_values.data();
    }

    auto flat_view() const -> const std::vector<T>& { // TODO? return span
      return flat_values;
    }
    auto offsets() const -> const knot_vector<I>& { // TODO? return span
      return _offsets;
    }

    auto push_back() {
      _offsets.push_back(flat_values.size());
    }
    auto push_back(const T& x) -> T& {
      flat_values.push_back(x);
      ++_offsets.back();
      return flat_values.back();
    }
    auto operator[](I i) {
      return span(data()+_offsets[i],data()+_offsets[i+1]); // TODO wrong because returns a view
                                       // but should return a proxy reference
    }
  private:
    std::vector<T> flat_values;
    knot_vector<I> _offsets;
};

template<class T, class I> auto
operator==(const jagged_vector<T,I>& x, const jagged_vector<T,I>& y) -> bool {
  return x.flat_view()==y.flat_view() && x.offsets()==y.offsets();
}
template<class T, class I> auto
operator!=(const jagged_vector<T,I>& x, const jagged_vector<T,I>& y) -> bool {
  return !(x==y);
}


} // std_e
