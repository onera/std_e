#pragma once

#include <vector>
#include "std_e/__perf_test/int_t.hpp"

namespace std_e {

auto
copy_connect(std::vector<int_t>& cs, std::vector<int_t>& cs_target, int n) -> void;

} // std_e
