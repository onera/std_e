#pragma once

#include "std_e/logging/printer.hpp"
#include <memory>
#include <vector>


namespace std_e {


//auto
//build_printer_common(const std::string& printer_name, const std::string& printer_args) -> std::unique_ptr<printer>;

auto
build_printer(const std::string& printer_name, const std::vector<std::string>& printer_args, const std::string& error_msg_context) -> std::unique_ptr<printer>;


} // std_e
