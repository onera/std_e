#pragma once


#include <string>
#include <vector>
#include "std_e/logging/log.hpp"


namespace std_e {


auto create_printer(const std::string& printer_desc, const std::string& error_msg_context) -> std::unique_ptr<printer>;
auto create_loggers(const std::string& conf, const std::string& error_msg_context) -> std::vector<logger>;

auto add_printer(const std::string& logger_name, const std::string& printer_desc) -> void;

} // std_e
