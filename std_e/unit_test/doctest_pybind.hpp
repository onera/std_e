#pragma once

#include "std_e/unit_test/doctest.hpp"
#include "std_e/utils/embed_python.hpp"

#define CREATE_PYBIND_TEST_CASE(name,func) \
  static void func(); \
  TEST_CASE(name * doctest::description("PYBIND_TEST_CASE")) { \
    std_e::throw_if_no_python_interpreter(std::string("PYBIND_TEST_CASE(\"")+name+"\")"); \
    func(); \
  } \
  static void func()

#define PYBIND_TEST_CASE(name) \
  CREATE_PYBIND_TEST_CASE(name,DOCTEST_ANONYMOUS(DOCTEST_PYBIND_FUNC))
