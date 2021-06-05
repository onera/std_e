Member initialization list without std::initializer_list
========================================================

One of the problem with using :cpp:`std::initializer_list` for initialization is the fact that one cannot overload based on the size of the list. To overcome this limitation, `use build-in arrays instead <https://stackoverflow.com/a/38679907/1583122>`_.
