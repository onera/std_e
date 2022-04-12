#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"
#include "std_e/meta/compile_time_size.hpp"
#include "std_e/future/span_ref.hpp"

#include <vector>
#include <deque>

TEST_CASE("compile_time_size") {
  CHECK( std_e::compile_time_size< std::array<int,4> > == 4 );

  CHECK( std_e::compile_time_size< std_e::span<int,4> > == 4 );
  CHECK( std_e::compile_time_size< std_e::span<int> > == std_e::dynamic_size );

  CHECK( std_e::compile_time_size< std_e::span_ref<int,4> > == 4 );
  CHECK( std_e::compile_time_size< std_e::span_ref<int> > == std_e::dynamic_size );

  CHECK( std_e::compile_time_size< std::vector<int> > == std_e::dynamic_size );
  CHECK( std_e::compile_time_size< std::deque<int> > == std_e::dynamic_size );
}
#endif // C++>17
