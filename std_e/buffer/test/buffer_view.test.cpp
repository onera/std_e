#include "std_e/unit_test/doctest.hpp"

#include "std_e/buffer/buffer_view.hpp"


TEST_CASE("buffer_view") {
  std::vector<int> v = {0,1,2};
  std_e::buffer_view b(v.data());

  CHECK_FALSE(b.is_owner());

  void* ptr = b.data();
  int* i_ptr = (int*)ptr;
  CHECK( i_ptr[0] == 0 );
  CHECK( i_ptr[1] == 1 );
  CHECK( i_ptr[2] == 2 );

  SUBCASE("sharing") {
    i_ptr[0] = 5;
    CHECK( i_ptr[0] == 5 );
    CHECK( v[0] == 5 );
  }

  SUBCASE("const") {
    SUBCASE("use const buffer") {
      const std_e::buffer_view b(v.data());
      const void* cptr = b.data();
      const int* i_ptr = static_cast<const int*>(ptr);
      CHECK( i_ptr[0] == 0 );
      CHECK( i_ptr[1] == 1 );
      CHECK( i_ptr[2] == 2 );
    }
    SUBCASE("buffers don't preserve constness themselves ") {
      // while std_e::buffer_const_view would make sense,
      // when erasing the type, we usally don't care about constness
      // so std_e::buffer_const_view does not exist,
      // but std_e::buffer_view does not const_cast implicitly
      
      // This would result in a compilation error:
      /*
      const std::vector<int> v;
      std_e::buffer_view b(v.data());
      */
    }
  }
}
