#include "std_e/unit_test/doctest.hpp"

#include "std_e/buffer/polymorphic_buffer.hpp"
#include "std_e/buffer/owning_buffer.hpp"
#include "std_e/buffer/buffer_vector.hpp"
#include "std_e/buffer/buffer_view.hpp"


TEST_CASE("polymorphic_buffer") {
  // std_e::polymorphic_buffer is a Buffer that can be constructed from any Buffer type
  // Its Buffer behavior will delegate to the correct version of the Buffer type it was constructed from
  SUBCASE("from non-owning") {
    std::vector<int> v = {0,1,2};
    std_e::polymorphic_buffer b(std_e::buffer_view(v.data()));

    CHECK_FALSE(b.is_owner());

    void* ptr = b.data();
    int* i_ptr = (int*)ptr;
    CHECK( i_ptr[0] == 0 );
    CHECK( i_ptr[1] == 1 );
    CHECK( i_ptr[2] == 2 );
  }

  SUBCASE("from owning") {
    std_e::buffer_vector<int> v = {0,1,2};
    std_e::polymorphic_buffer b(std_e::to_buffer(std::move(v)));

    CHECK(b.is_owner());

    void* ptr = b.data();
    int* i_ptr = (int*)ptr;
    CHECK( i_ptr[0] == 0 );
    CHECK( i_ptr[1] == 1 );
    CHECK( i_ptr[2] == 2 );

    auto dealloc = b.release();
    CHECK_FALSE(b.is_owner());

    // now that the buffer does not own the memory,
    // we have to manually deallocate it with the right deallocator
    dealloc(ptr);
  }
}

// TODO with custom allocator
