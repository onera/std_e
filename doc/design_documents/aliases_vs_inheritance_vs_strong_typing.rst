.. _aliases_etc:

Aliases vs strong typing vs inheritance
=======================================

In C++, aliases are never deduced. So we can't use their name if we expect implicit function overload resolution. Instead, we need to use the original name of the alias, which defeats the point of giving semantically accurate names.

Strong typing (using a macro to create a type similar to another one) can be an option, but it feels like a heavyweight machinery to patch a language limitation.

Public inheritance is a way to get around the problem, even if not perfect (we actually don't want the implicit conversion from the derived to the base class). This is the choice made for :ref:`multi_index`.
