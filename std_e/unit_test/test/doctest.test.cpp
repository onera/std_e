#include "std_e/unit_test/doctest.hpp"

TEST_CASE("to_string") {
  std::array<int,3> a = {10,11,12};
  std::vector<int> v = {13,14,15,16};
  std_e::span<int> s(v.data(),2);

  CHECK( doctest::StringMaker<std::array<int,3>>::convert(a) == "{10,11,12}" );
  CHECK( doctest::StringMaker<std::vector<int>>::convert(v) == "{13,14,15,16}" );
  CHECK( doctest::StringMaker<std_e::span<int>>::convert(s) == "{13,14}" );
}
