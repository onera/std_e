#include "std_e/logging/create_loggers.hpp"
#include "std_e/logging/configure_loggers_exception.hpp"

#include "std_e/logging/build_printer_from_strings.hpp"


namespace std_e {


auto
create_printer(const std::string& printer_desc, const std::string& error_msg_context) {
  auto n_open_paren  = std::count(begin(printer_desc),end(printer_desc),'(');
  auto n_close_paren = std::count(begin(printer_desc),end(printer_desc),')');
  STD_E_ASSERT(n_close_paren == n_open_paren); // precondition of the function

  auto error_msg_begin = error_msg_context + ": printer \"" + printer_desc + "\": must be of the form \"printer_name\" or \"printer_name(arg0,arg1,...)\"";
  if (n_open_paren > 1) {
    throw configure_loggers_exception(error_msg_begin + " but too many parentheses");
  }

  auto tokens = split(printer_desc,'(');
  auto printer_name = trim(tokens[0]);
  std::vector<std::string> printer_args = {};
  if (n_open_paren == 1) {
    auto printer_args_cat = trim(tokens[1]);
    if (printer_args_cat.back() != ')') {
      auto ss = split(printer_args_cat,')');
      throw configure_loggers_exception(error_msg_begin + " but extra characters \"" + ss.back() + "\" where found at the end");
    }
    printer_args_cat.pop_back();
    printer_args = trim(split(printer_args_cat,','));
  }

  return build_printer(printer_name,printer_args,error_msg_context);
}

auto
create_logger(const std::string& line, const std::string& error_msg_context) -> logger {
  auto n_colon = std::count(begin(line),end(line),':');
  if (n_colon == 0) {
    throw configure_loggers_exception(error_msg_context + ": missing colon");
  }
  if (n_colon > 1) {
    throw configure_loggers_exception(error_msg_context + ": too many colons");
  }

  if (line[0] == ':') {
    throw configure_loggers_exception(error_msg_context + ": empty logger name");
  }
  auto logger_and_printers = split(line,':');
  auto logger_name = trim(logger_and_printers[0]);

  typename logger::printers_type printers;
  if (logger_and_printers.size() == 2) { // there is something after the colon
    auto printer_descriptions_cat = trim(logger_and_printers[1]);

    if (printer_descriptions_cat != "") { // what is after the colon is not only spaces
      if (!balanced_parens(printer_descriptions_cat,'(',')')) {
        throw configure_loggers_exception(error_msg_context + ": parentheses are not balanced");
      }
      auto printer_descriptions = split_with_parens(printer_descriptions_cat,',','(',')');

      for (const auto& printer_description : printer_descriptions) {
        printers.emplace_back( create_printer(printer_description,error_msg_context) );
      }
    }
  }

  return logger(logger_name, std::move(printers));
}


auto
strip_comment(const std::string& s) -> std::string {
  auto tokens = split(s,'#');
  if (tokens.size() > 0) return tokens[0];
  else return "";
}

auto
create_loggers(const std::string& conf, const std::string& error_msg_context) -> std::vector<logger> {
  std::vector<logger> loggers = {};

  auto lines = split(conf,'\n');
  for (int i=0; i<(int)lines.size(); ++i) {
    auto line = trim(strip_comment(lines[i]));
    if (line != "") {
      auto error_msg_context_with_line = error_msg_context + " at line " + std::to_string(i+1);
      loggers.emplace_back( create_logger(line, error_msg_context_with_line) );
    }
  }

  return loggers;
}


} // std_e
