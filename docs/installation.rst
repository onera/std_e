.. _installation:

Installation
============

Dependencies
------------

`std_e` is a header-only C++17 library. It depends only on the standard library.

The unit tests use `doctest` and the build process requires :

* Cmake >= 3.12
* GCC >= 8

The documentation build requires :

* Doxygen >= 1.8.19
* Breathe >= 4.15 (python package)
* Sphinx >= 3.00 (python package)

Build and install
-----------------

1. `git clone https://github.com/BerengerBerthoul/std_e`
2. `cd std_e`
3. `git submodule update --init` (needed for submodule dependency `project_utils`)
4. Use `cmake` to configure, build and install. The configuration step will automatically download `doctest`. If you are not familiar with `cmake` usage, an example is given in `scripts/install.sh`.
