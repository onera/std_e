#include "std_e/__unused/test/test_helper.hpp"

#include "std_e/__unused/iter_kind_1.hpp"
#include "std_e/logging/time_logger.hpp"

auto
copy_connect(std::vector<int_t>& cs, std::vector<int_t>& cs_target, int n) -> void {
  connectivity_range_1 connec_view(cs,n);
  connectivity_range_1 connec_view_target(cs_target,n);

  {
    auto _ = std_e::stdout_time_logger("copy by connec (size stored)"); // 0.40s
    std::copy(connec_view.begin(),connec_view.end(),connec_view_target.begin());
  }
}
