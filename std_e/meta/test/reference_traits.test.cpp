#include "std_e/unit_test/doctest.hpp"
#include "std_e/meta/reference_traits.hpp"

namespace {
  struct my_val_type {};
  struct my_ref_type {};
}

// use std_e::reference_value as a customization point
template<>
struct std_e::reference_value<my_ref_type> {
  using type = my_val_type;
};


TEST_CASE("reference_value_t") {
  // Equivalent to std::remove_cvref if no specializatio point:
  CHECK(std::is_same_v< std_e::reference_value_t<      int&> , int  >);
  CHECK(std::is_same_v< std_e::reference_value_t<const int&> , int  >);
  CHECK(std::is_same_v< std_e::reference_value_t<      int > , int  >);
  CHECK(std::is_same_v< std_e::reference_value_t<      int*> , int* >);

  // Customization point
  CHECK(std::is_same_v< std_e::reference_value_t<my_ref_type> , my_val_type >);
}
