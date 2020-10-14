.. contents:: :local:

.. _log:

Logging facilities
==================

Introduction
------------

When offering logging facilities, multiple points come into consideration:
1. Where to log and with which format
2. When: log messages when asked or depending on the context
3. Defaults: do not specify everything everytime, use reasonable defaults

The idea is to have a pool of `logger`, and `log` functions. The log functions can be called either with the name of a logger or a logging level, and every logger in the pool matching the criterion is asked to log. The pool has already default loggers that will be triggered by `log` calls with default logging levels. The user is free to add and remove loggers, and parametrize when his logger is triggered and how it should log.

Point 1 is handled by creating and parameterizing `logger` objects, point 2 by matching loggers with the `log` functions, and point 3 by default loggers.

Details
-------

log functions
^^^^^^^^^^^^^

.. code:: c++
  /// log by matching logger's name
  auto log(const std::string& log_name, const std::string& msg) -> void;

  /// log by matching lvl
  auto log(logging_level log_lvl, const std::string& msg) -> void;

  /// same, with default values
  auto log(const std::string& msg) -> void;

When a log function is called, all matching loggers will be asked to log the `msg` string.

logger
^^^^^^

.. code:: c++
    struct logger {
      std::string name;
      logging_level lvl;
      std::unique_ptr<log_printer> printer;
    };

The `log_printer` is an abstract class with a `log(std::string)` virtual method. The user may want to define his own `log_printer` class.

add a logger to the pool
""""""""""""""""""""""""

.. code:: c++
  auto add_logger(logger l) -> logger&;

default loggers and MPI
^^^^^^^^^^^^^^^^^^^^^^^

By default, two default loggers are registered to the pool under the name "terminal" and "file", printing messages to the standard output and file "log.txt"

When building with MPI, "terminal" messages will be prefixed by the mpi rank, and "file" actually prints to one file by rank, of name "mpi_log_<rank>.txt". An additional `log_0` function is provided if a message must be printed only once.

