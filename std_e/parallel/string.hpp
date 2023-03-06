#pragma once

#include <string>
#include "std_e/parallel/mpi/base.hpp"
#include "std_e/logging/console_color.hpp"


namespace std_e {


auto
cat_on_rank_0(const std::string& rank_str, MPI_Comm comm) -> std::string;


} // std_e
