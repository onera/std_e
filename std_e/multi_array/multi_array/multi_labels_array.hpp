#pragma once

#include "std_e/multi_array/multi_array/multi_array.hpp"
#include "std_e/multi_index/multi_index.hpp"


namespace std_e {


template<class T, class... Ts>
class multi_labels_array : public multi_array<std::vector<T> , dyn_shape<int,sizeof...(Ts)>> {
  public:
  // type traits
    static constexpr size_t fixed_rank = sizeof...(Ts);
    using value_type = T;

  // ctor
    multi_labels_array() = default;

  // construction
    template<int i, class labels_i_type> auto
    set_labels(labels_i_type&& labels_i) -> void {
      auto new_dims = this->extent();
      new_dims[i] = labels_i.size();

      std::get<i>(_labels) = FWD(labels_i);
      reshape(*this,new_dims);
    }

    template<int i, class label_i_type> auto
    increment_direction(label_i_type&& label_i) -> void {
      std::get<i>(_labels).push_back(FWD(label_i));
      auto new_dims = this->extent();
      ++new_dims[i];
      reshape(*this,new_dims);
    }

  // accessors
    template<int i> auto
    labels() const -> const auto& {
      return std::get<i>(_labels);
    }
    auto
    array() -> auto& {
      return *this;
    } 

  private:
    std::tuple<std::vector<Ts>...> _labels;
};


// 2D helper functions {
template<class row_label_type, class col_label_type, class T, class row_labels_type> auto
set_row_labels(multi_labels_array<T,row_label_type,col_label_type>& arr, row_labels_type&& row_labels) -> void {
  arr.template set_labels<0>(FWD(row_labels));
}
template<class row_label_type, class col_label_type, class T, class col_labels_type> auto
set_col_labels(multi_labels_array<T,row_label_type,col_label_type>& arr, col_labels_type&& col_labels) -> void {
  arr.template set_labels<1>(FWD(col_labels));
}

template<class row_label_type, class col_label_type, class T, class row_label_type_> auto
append_row(multi_labels_array<T,row_label_type,col_label_type>& arr, row_label_type_&& row_label) -> void {
  arr.template increment_direction<0>(FWD(row_label));
}
template<class row_label_type, class col_label_type, class T, class col_label_type_> auto
append_col(multi_labels_array<T,row_label_type,col_label_type>& arr, col_label_type_&& col_label) -> void {
  arr.template increment_direction<1>(FWD(col_label));
}

template<class row_label_type, class col_label_type, class T> auto
row_labels(const multi_labels_array<T,row_label_type,col_label_type>& arr) -> const std::vector<row_label_type>& {
  return arr.template labels<0>();
}
template<class row_label_type, class col_label_type, class T> auto
col_labels(const multi_labels_array<T,row_label_type,col_label_type>& arr) -> const std::vector<col_label_type>& {
  return arr.template labels<1>();
}
// 2D helper functions }


} // std_e
