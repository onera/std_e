std_e
=====

`std_e` (for "std extended") is a C++17 library. It contains general-purpose data structures and algorithms (multi-dimensional array, constexpr std algorithms, utilities...). Hopefully, one day, these functionnalities will be provided by the C++ standard library.


## Dependencies ##
`std_e` is a header-only C++17 library. It depends only on the standard library. The unit tests use `doctest` and the build process requires GCC>=8, Git>=2.14 and Cmake>=3.12.

## Build and install ##
1. `git clone https://github.com/BerengerBerthoul/std_e`
2. `cd std_e`
3. `git submodule update --init` (needed for submodule dependency `project_utils`)
4. Use `cmake` to configure, build and install. The configuration step will automatically download `doctest`. If you are not familiar with `cmake` usage, an example is given in `scripts/install.sh`.

## Development ##
The `std_e` repository is compatible with the development process described in `external/project_utils/doc/main.md`. It uses git submodules to ease the joint development with other repositories compatible with this organization. TL;DR: configure the git repository with `cd external/project_utils/scripts && configure_top_level_repo.sh".
