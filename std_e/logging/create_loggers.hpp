#pragma once


#include <string>
#include <vector>
#include "std_e/logging/log.hpp"


namespace std_e {


auto create_loggers(const std::string& conf, const std::string& error_msg_context) -> std::vector<logger>;


} // std_e
