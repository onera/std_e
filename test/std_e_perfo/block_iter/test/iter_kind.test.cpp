#include "std_e/unit_test/doctest.hpp"

#include "test/std_e_perfo/block_iter/iter_kind_0.hpp"
#include "test/std_e_perfo/block_iter/iter_kind_1.hpp"
#include "std_e/logging/time_logger.hpp"
#include "std_e/base/system/profiling.hpp"

using std::vector;
using namespace std_e;


TEST_CASE("block iteration") {
  constexpr int N = 4;
  int n = 4;
  int connec_sz = 100'000'000;
  //int connec_sz = 1;

  vector<int_t> cs(connec_sz*N);
  vector<int_t> cs_target(connec_sz*N);
  std::iota(begin(cs),end(cs),0);

  SUBCASE("straight copy") {
    clobber_memory();
    {
      auto _ = std_e::stdout_time_logger("straight copy"); // N=3: 0.15s, N=4: 0.20s
      std::copy(cs.begin(),cs.end(),cs_target.begin());
    }
    do_not_optimize(cs_target.data());
  }

  SUBCASE("iter kind 0") {
    clobber_memory();
    connectivity_range_0<N> connec_view(cs);
    connectivity_range_0<N> connec_view_target(cs_target);

    {
      auto _ = std_e::stdout_time_logger("copy by connec kind 0 (size templated)"); // N=3: 0.40s, N=4: 0.33s
      std::copy(connec_view.begin(),connec_view.end(),connec_view_target.begin());
    }
    do_not_optimize(cs_target.data());
  }

  SUBCASE("iter kind 1") {
    clobber_memory();
    do_not_optimize(&n); // if not present, timing as above: it means that the compiler (at least GCC 11) propagates n
    connectivity_range_1 connec_view(cs,n);
    connectivity_range_1 connec_view_target(cs_target,n);

    {
      auto _ = std_e::stdout_time_logger("copy by connec (size stored)");
      std::copy(connec_view.begin(),connec_view.end(),connec_view_target.begin()); // N=3: 0.40s, N=4 0.43s
    }
    do_not_optimize(cs_target.data());
  }

  CHECK( cs_target == cs );
}
