Tutorial
========


Simple example
--------------

Example of use of the :cpp:class:`std_e::hvector` (for "heterogenous vector ")

.. code:: c++

  std_e::hvector<int,double> hv = { vector{1,2,3,4} , vector{3.14,2.7} };
  int sz = 0;
  auto f = [&sz](auto v){ sz += v.size(); };
  for_each_vector(hv,f);
  // sz = 4+2 ;
