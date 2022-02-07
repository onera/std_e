#include "std_e/unit_test/doctest.hpp"

#include "test/std_e_perfo/rotate/rotate_fixed_size.hpp"
#include "std_e/logging/time_logger.hpp"
#include "std_e/base/system/profiling.hpp"
#include "std_e/algorithm/rotate/rotate.hpp"

using std::vector;
using namespace std_e;

/*
rotate_min_first_handwritten_ptr; -> 3.2s
rotate_min_first_handwritten; -> 3.2s or 3.8s same ASM GCC 11 -> Note that the "optim loop" is better
rotate_min_first_array; -> 6.5s ! However ASM is almost exact same (only register names are different!)
rotate_min_first_copy_n; -> 9s
rotate_min_first_rotate_copy; -> 11s
rotate_min_first_rotate_copy_range; -> 11s
rotate_min_first_rotate; -> 6s
rotate_min_first_rotate_range; -> 6s
*/

const int repeat = 2;

TEST_CASE("rotate_fixed_size - 3") {
  constexpr int ct_size = 3;
  const int sz = 100'000'000*ct_size;
  vector<int> v(sz);
  clobber_memory();

  std::array<int,ct_size> a = {2,0,1};
  span<int,ct_size> s(a.data());

  SUBCASE("handwritten ptr") {
    {
      rotate_min_first_handwritten_ptr(a.data());
      CHECK( vector{a[0],a[1],a[2]} == vector{0,1,2} );
    }

    auto _ = std_e::stdout_time_logger("handwritten ptr");
    for (int k=0; k<repeat; ++k) {
      clobber_memory();
      int* ptr = v.data();
      for (int i=0; i<sz/ct_size; ++i) {
        rotate_min_first_handwritten_ptr(ptr);
        ++ptr;
      }
      do_not_optimize(v.data());
    }
  }
  SUBCASE("handwritten ptr int 64") {
    {
      rotate_min_first_handwritten_ptr(a.data());
      CHECK( vector{a[0],a[1],a[2]} == vector{0,1,2} );
    }

    auto _ = std_e::stdout_time_logger("handwritten ptr int 64");
    for (int k=0; k<repeat; ++k) {
      clobber_memory();
      int* ptr = v.data();
      for (int i=0; i<sz/ct_size; ++i) {
        rotate_min_first_handwritten_ptr_int64(ptr);
        ++ptr;
      }
      do_not_optimize(v.data());
    }
  }
  SUBCASE("handwritten") {
    {
      rotate_min_first_handwritten(s);
      CHECK( s == vector{0,1,2} );
    }

    SUBCASE("optim loop") {
      auto _ = std_e::stdout_time_logger("handwritten - optim loop");
      for (int k=0; k<repeat; ++k) {
        clobber_memory();
        int* ptr = v.data();
      for (int i=0; i<sz/ct_size; ++i) {
          auto elt = span<int,ct_size>(ptr);
          rotate_min_first_handwritten(elt);
          ++ptr;
        }
        do_not_optimize(v.data());
      }
    }
    SUBCASE("non-optim loop") {
      auto _ = std_e::stdout_time_logger("handwritten - non-optim loop");
      for (int k=0; k<repeat; ++k) {
        clobber_memory();
        for (int i=0; i<sz; i+=ct_size) {
          auto elt = span<int,ct_size>(v.data()+i);
          rotate_min_first_handwritten(elt);
        }
        do_not_optimize(v.data());
      }
    }
  }
  SUBCASE("handwritten int64") {
    {
      rotate_min_first_handwritten(s);
      CHECK( s == vector{0,1,2} );
    }

    auto _ = std_e::stdout_time_logger("handwritten int64");
    for (int k=0; k<repeat; ++k) {
      clobber_memory();
      for (int i=0; i<sz; i+=ct_size) {
        auto elt = span<int,ct_size>(v.data()+i);
        rotate_min_first_handwritten_int64(elt);
      }
      do_not_optimize(v.data());
    }
  }
  SUBCASE("handwritten range") {
    {
      rotate_min_first_handwritten(s);
      CHECK( s == vector{0,1,2} );
    }

    auto _ = std_e::stdout_time_logger("handwritten range");
    for (int k=0; k<repeat; ++k) {
      clobber_memory();
      for (int i=0; i<sz; i+=ct_size) {
        auto elt = span<int,ct_size>(v.data()+i);
        rotate_min_first_handwritten_range(elt);
      }
      do_not_optimize(v.data());
    }
  }
  SUBCASE("array") {
    {
      rotate_min_first_array(s);
      CHECK( s == vector{0,1,2} );
    }

    auto _ = std_e::stdout_time_logger("array");
    for (int k=0; k<repeat; ++k) {
      clobber_memory();
      for (int i=0; i<sz; i+=ct_size) {
        auto elt = span<int,ct_size>(v.data()+i);
        rotate_min_first_array(elt);
      }
      do_not_optimize(v.data());
    }
  }
  SUBCASE("copy_n") {
    {
      rotate_min_first_copy_n(s);
      CHECK( s == vector{0,1,2} );
    }

    auto _ = std_e::stdout_time_logger("copy_n");
    for (int k=0; k<repeat; ++k) {
      clobber_memory();
      for (int i=0; i<sz; i+=ct_size) {
        auto elt = span<int,ct_size>(v.data()+i);
        rotate_min_first_copy_n(elt);
      }
      do_not_optimize(v.data());
    }
  }
  SUBCASE("rotate_copy") {
    {
      rotate_min_first_rotate_copy(s);
      CHECK( s == vector{0,1,2} );
    }

    auto _ = std_e::stdout_time_logger("rotate_copy");
    for (int k=0; k<repeat; ++k) {
      clobber_memory();
      for (int i=0; i<sz; i+=ct_size) {
        auto elt = span<int,ct_size>(v.data()+i);
        rotate_min_first_rotate_copy(elt);
      }
      do_not_optimize(v.data());
    }
  }
  SUBCASE("rotate_copy range") {
    {
      rotate_min_first_rotate_copy_range(s);
      CHECK( s == vector{0,1,2} );
    }

    auto _ = std_e::stdout_time_logger("rotate_copy range");
    for (int k=0; k<repeat; ++k) {
      clobber_memory();
      for (int i=0; i<sz; i+=ct_size) {
        auto elt = span<int,ct_size>(v.data()+i);
        rotate_min_first_rotate_copy_range(elt);
      }
      do_not_optimize(v.data());
    }
  }
  SUBCASE("rotate") {
    {
      rotate_min_first_rotate(s);
      CHECK( s == vector{0,1,2} );
    }

    auto _ = std_e::stdout_time_logger("rotate");
    for (int k=0; k<repeat; ++k) {
      clobber_memory();
      for (int i=0; i<sz; i+=ct_size) {
        auto elt = span<int,ct_size>(v.data()+i);
        rotate_min_first_rotate(elt);
      }
      do_not_optimize(v.data());
    }
  }
  SUBCASE("rotate range") {
    {
      rotate_min_first_rotate_range(s);
      CHECK( s == vector{0,1,2} );
    }

    auto _ = std_e::stdout_time_logger("rotate range");
    for (int k=0; k<repeat; ++k) {
      clobber_memory();
      for (int i=0; i<sz; i+=ct_size) {
        auto elt = span<int,ct_size>(v.data()+i);
        rotate_min_first_rotate_range(elt);
      }
      do_not_optimize(v.data());
    }
  }
  SUBCASE("std_e::rotate_min_first") {
    {
      rotate_min_first_rotate_range(s);
      CHECK( s == vector{0,1,2} );
    }

    auto _ = std_e::stdout_time_logger("std_e::rotate_min_first");
    for (int k=0; k<repeat; ++k) {
      clobber_memory();
      for (int i=0; i<sz; i+=ct_size) {
        auto elt = span<int,ct_size>(v.data()+i);
        std_e::rotate_min_first(elt);
      }
      do_not_optimize(v.data());
    }
  }

  do_not_optimize(v.data());
}

TEST_CASE("rotate_fixed_size - 4") {
  constexpr int ct_size = 4;
  const int sz = 100'000'000*ct_size;
  vector<int> v(sz);
  clobber_memory();

  std::array<int,4> a = {2,0,1,3};
  span<int,4> s(a.data());

  SUBCASE("rotate range") {
    {
      rotate_min_first_rotate_range(s);
      CHECK( s == vector{0,1,3,2} );
    }

    auto _ = std_e::stdout_time_logger("rotate range");
    for (int k=0; k<repeat; ++k) {
      clobber_memory();
      for (int i=0; i<sz; i+=ct_size) {
        auto elt = span<int,ct_size>(v.data()+i);
        rotate_min_first_rotate_range(elt);
      }
      do_not_optimize(v.data());
    }
  }
  SUBCASE("std_e::rotate_min_first") {
    {
      rotate_min_first_rotate_range(s);
      CHECK( s == vector{0,1,3,2} );
    }

    auto _ = std_e::stdout_time_logger("std_e::rotate_min_first");
    for (int k=0; k<repeat; ++k) {
      clobber_memory();
      for (int i=0; i<sz; i+=ct_size) {
        auto elt = span<int,ct_size>(v.data()+i);
        std_e::rotate_min_first(elt);
      }
      do_not_optimize(v.data());
    }
  }

  do_not_optimize(v.data());
}
