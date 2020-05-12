#pragma once

#include "std_e/utils/enum.hpp"

// DEPRECATED (use STD_E_ENUM_CLASS)
#define STR_ENUM(enum_name, ... ) STD_E_ENUM__IMPL(enum class,enum_name,__VA_ARGS__)
