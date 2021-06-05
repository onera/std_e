.. _todo:

Todo
====

Code
----
RENAME make_sub_array -> slice ?
strided_view of strided_view (which sometimes returns a multi_array!)
helper_bind
flatten_tuple
utils/integer_range => move + facto
utils/pretty_print => TEST


Doc
---
literalinclude: link to the original source file


Build and CI
------------
More CMake checks on whether we can build the documentation (sphinxcontrib-applehelp, breathe==4.17)

Use CodeCoverage, Sanitizer, CppCheck, Clang-tidy

CppCheck: http://cppcheck.sourceforge.net/manual.html#html-report
