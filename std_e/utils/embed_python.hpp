#pragma once

#include <Python.h>
#include <exception>

namespace std_e {
  inline auto
  throw_if_no_python_interpreter(const std::string& src_location_hint) -> void {
    if (!Py_IsInitialized()) {
      throw std::runtime_error(src_location_hint + ": the Python interpreter is needed, yet it is not running. If you are not running through Python, you might want to instanciate a pybind11::scoped_interpreter somewhere");
    }
  }
}
