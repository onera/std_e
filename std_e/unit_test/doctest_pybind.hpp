#pragma once

#include "std_e/unit_test/doctest.hpp"
#include "std_e/utils/embed_python.hpp"

// Note: This is a copy-paste of what is in doctest, the only modified part is that we throw if we don't find a python interpreter

#define CREATE_PYBIND_TEST_CASE(name,func) \
  static void func(); \
  TEST_CASE(name * doctest::description("PYBIND_TEST_CASE")) { \
    std_e::throw_if_no_python_interpreter(std::string("PYBIND_TEST_CASE(\"")+name+"\")"); \
    func(); \
  } \
  static void func()

#define PYBIND_TEST_CASE(name) \
  CREATE_PYBIND_TEST_CASE(name,DOCTEST_ANONYMOUS(DOCTEST_PYBIND_FUNC))
