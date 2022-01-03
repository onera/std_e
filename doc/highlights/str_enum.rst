.. _enum:

Enums convertible to strings
----------------------------

C++ :cpp:`enum` and :cpp:`enum class` are missing two convenient features: the number of entries in the enum and conversions to :cpp:`std::string`. These features are provided by :cpp:`STD_E_ENUM` and :cpp:`STD_E_ENUM_CLASS` respectively. Example with :cpp:`STD_E_ENUM_CLASS`:

.. literalinclude:: /../std_e/utils/test/enum.test.cpp
  :language: C++
  :start-after: [Sphinx Doc] STD_E_ENUM_CLASS {
  :end-before: [Sphinx Doc] STD_E_ENUM_CLASS }
