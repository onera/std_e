#include "std_e/unit_test/doctest.hpp"

#include "std_e/__unused/iter_kind_0.hpp"
#include "std_e/__unused/iter_kind_1.hpp"
#include "std_e/__unused/test/test_helper.hpp"
#include "std_e/logging/time_logger.hpp"

using std::vector;
using namespace std_e;


TEST_CASE("iter_kind 0") {
  constexpr int N = 3;
  int connec_sz = 100'000'000;
  //int connec_sz = 1;

  vector<int_t> cs(connec_sz*N);
  vector<int_t> cs_target(connec_sz*N);
  std::iota(begin(cs),end(cs),0);

  SUBCASE("straight copy") {
    auto _ = std_e::stdout_time_logger("straight copy"); // N=3: 0.15s, N=4: 0.20s
    std::copy(cs.begin(),cs.end(),cs_target.begin());
  }

  SUBCASE("iter kind 0") {
    connectivity_range_0<N> connec_view(cs);
    connectivity_range_0<N> connec_view_target(cs_target);

    {
      auto _ = std_e::stdout_time_logger("copy by connec kind 0 (size templated)"); // N=3: 0.40s, N=4: 0.33s
      std::copy(connec_view.begin(),connec_view.end(),connec_view_target.begin());
    }
  }

  SUBCASE("iter kind 1") {
    copy_connect(cs,cs_target,N); // N=3: 0.40s, N=4 0.43s (and same timing as kind 0 if the compiler sees "copy_connect" implementation (can propagate N)
    //connectivity_range_1 connec_view(cs,N);
    //connectivity_range_1 connec_view_target(cs_target,N);

    //{
    //  auto _ = std_e::stdout_time_logger("copy by connec (size stored)"); // 0.40s
    //  std::copy(connec_view.begin(),connec_view.end(),connec_view_target.begin());
    //}
  }

  CHECK( cs_target == cs );
}
