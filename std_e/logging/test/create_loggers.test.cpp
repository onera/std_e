#include "std_e/unit_test/doctest.hpp"
#include "std_e/logging/create_loggers.hpp"
#include "std_e/logging/printer_common.hpp"


TEST_CASE("create_loggers") {
  std::string conf =
    "my_logger      : stdout_printer\n"
    "my_other_logger: stdout_printer  \n"
    //"my_third_logger:  stdout_printer\n"
    "my_third_logger:  stdout_printer ,  file_printer( 'file_name'  )  \n"
    "no_print_logger:\n";
  auto loggers = std_e::create_loggers(conf, "<context>");

  REQUIRE(loggers.size() == 4);
  CHECK(loggers[0].name == "my_logger");
  CHECK(loggers[1].name == "my_other_logger");
  CHECK(loggers[2].name == "my_third_logger");
  CHECK(loggers[3].name == "no_print_logger");

  CHECK(loggers[0].printers.size() == 1 );
  CHECK(loggers[1].printers.size() == 1 );
  CHECK(loggers[2].printers.size() == 2 );
  CHECK(loggers[3].printers.size() == 0 );

  CHECK_NOTHROW( dynamic_cast<std_e::stdout_printer&>(*loggers[0].printers[0]) );
  CHECK_NOTHROW( dynamic_cast<std_e::stdout_printer&>(*loggers[1].printers[0]) );
  CHECK_NOTHROW( dynamic_cast<std_e::stdout_printer&>(*loggers[2].printers[0]) );
  CHECK_NOTHROW( dynamic_cast<std_e::file_printer  &>(*loggers[2].printers[1]) );

  std::string conf_0 = "my_logger,stdout_printer\n";
  std::string conf_1 = ":stdout_printer\n";
  std::string conf_2 = "my_logger:stdout_printer:str\n";
  std::string conf_3 = "my_logger:stdout_printer(\n";
  std::string conf_4 = "my_logger:stdout_printer('x',y(z))\n";
  std::string conf_5 = "my_logger:stdout_printer()str\n";
  std::string conf_6 = "my_logger:stdout_printer('x')\n";
  std::string conf_7 = "my_logger:file_printer\n";
  std::string conf_8 = "my_logger:file_printer('x',y)\n";

  CHECK_THROWS_WITH( std_e::create_loggers(conf_0,"<context>"), "<context> at line 1: missing colon" );
  CHECK_THROWS_WITH( std_e::create_loggers(conf_1,"<context>"), "<context> at line 1: empty logger name" );
  CHECK_THROWS_WITH( std_e::create_loggers(conf_2,"<context>"), "<context> at line 1: too many colons" );
  CHECK_THROWS_WITH( std_e::create_loggers(conf_3,"<context>"), "<context> at line 1: parentheses are not balanced" );

  CHECK_THROWS_WITH( std_e::create_loggers(conf_4,"<context>"),
      "<context> at line 1: printer \"stdout_printer('x',y(z))\":"
      " must be of the form \"printer_name\" or \"printer_name(arg0,arg1,...)\" but too many parentheses" );

  CHECK_THROWS_WITH( std_e::create_loggers(conf_5,"<context>"),
      "<context> at line 1: printer \"stdout_printer()str\":"
      " must be of the form \"printer_name\" or \"printer_name(arg0,arg1,...)\" but extra characters \"str\" where found at the end" );

  CHECK_THROWS_WITH( std_e::create_loggers(conf_6,"<context>"), "<context> at line 1: stdout_printer needs 0 argument, but 1 given" );
  CHECK_THROWS_WITH( std_e::create_loggers(conf_7,"<context>"), "<context> at line 1: file_printer needs 1 argument, but 0 given" );
  CHECK_THROWS_WITH( std_e::create_loggers(conf_8,"<context>"), "<context> at line 1: file_printer needs 1 argument, but 2 given" );
}
