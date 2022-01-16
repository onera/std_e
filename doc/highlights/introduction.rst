.. _highlights_introduction:

Introduction
------------

Below is a succinct presentation of the main facilities of **std_e**. For a more detailed presentation, see the :ref:`user_manual`.

In the examples below, when there is no ambiguity, we will assume that we are using the :cpp:`std` and :cpp:`std_e` namespaces:

.. code:: c++

  using namespace std;
  using namespace std_e;

.. note::

  Most of the code snippets presented here are actually directly extracted from **std_e** unit tests. A :cpp:`TEST_CASE` allows to define a unit test. Each :cpp:`SUBCASE` is independent from previous ones at the same level: if a variable is changed in one section, the change is not kept in the following ones (for more details, see :ref:`unit_tests`). The :cpp:`CHECK` command allows to easily see what the expected result is supposed to be.
