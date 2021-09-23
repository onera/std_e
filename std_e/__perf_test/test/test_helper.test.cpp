#include "std_e/__perf_test/test/test_helper.hpp"

#include "std_e/__perf_test/iter_kind_1.hpp"
#include "std_e/logging/time_logger.hpp"

namespace std_e {

auto
copy_connect(std::vector<int_t>& cs, std::vector<int_t>& cs_target, int n) -> void {
  connectivity_range_1 connec_view(cs,n);
  connectivity_range_1 connec_view_target(cs_target,n);

  {
    auto _ = std_e::stdout_time_logger("copy by connec (size stored)"); // 0.40s
    std::copy(connec_view.begin(),connec_view.end(),connec_view_target.begin());
  }
}

} // std_e
