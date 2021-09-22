.. _installation:

Installation
============

Dependencies
------------

**std_e** only depends on the standard library. There is an optional dependency to MPI.

The build process requires:

* Cmake >= 3.12
* GCC >= 8

The documentation build requires:

* Doxygen >= 1.8.19
* Breathe >= 4.15 (python package)
* Sphinx >= 3.00 (python package)

Build and install
-----------------

1. :code:`git clone https://github.com/BerengerBerthoul/std_e`
2. :code:`cd std_e`
3. :code:`git submodule update --init` (needed for submodule dependency **project_utils**)
4. Use **cmake** to configure, build and install. The configuration step will automatically download **doctest** (unit test library).

The main **cmake** configuration variables are the following (values are the defaults):

.. code:: bash

  std_e_ENABLE_CPP20=OFF # required for some functionalities, in particular graphs
  std_e_ENABLE_MPI=OFF # thin wrappers around MPI functions
  std_e_ENABLE_TEST=ON
  std_e_ENABLE_COVERAGE=OFF
  std_e_BUILD_DOCUMENTATION=OFF
