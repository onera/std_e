#include "std_e/unit_test/doctest.hpp"

#include "std_e/buffer/owning_buffer.hpp"
#include "std_e/buffer/buffer_vector.hpp"


TEST_CASE("owning_buffer") {
  std_e::buffer_vector<int> v = {0,1,2};

  auto b = std_e::to_buffer(std::move(v)); // now the memory is owned by the buffer

  CHECK(b.is_owner());

  void* ptr = b.data();
  int* i_ptr = (int*)ptr;
  CHECK( i_ptr[0] == 0 );
  CHECK( i_ptr[1] == 1 );
  CHECK( i_ptr[2] == 2 );

  SUBCASE("release") {
    auto dealloc = b.release();
    CHECK_FALSE(b.is_owner());

    // the memory is still here, just not owned be the buffer
    CHECK( i_ptr[0] == 0 );
    CHECK( i_ptr[1] == 1 );
    CHECK( i_ptr[2] == 2 );

    // now that the buffer does not own the memory,
    // we have to manually deallocate it with the right deallocator
    dealloc(ptr);
  }
}

// TODO with custom allocator
