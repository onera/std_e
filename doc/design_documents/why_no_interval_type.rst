Why there is no interval type
=============================

`std_e` had an interval type. It was called `interval_sequence`, and before that `knot_sequence`. But we removed it. Why is that?

First, why would we want such a type in the first place? Well, to encode the fact that a sequence is indeed an interval, that is, it is an increasing sequence of integers. We can make that the class invariant, and hence ensure **by typing** that functions using intervals (using them as argument or producing them) do indeed are given/return intervals.

Why is it a bad idea:
- it makes interoperability more complicated, because
  - if you happen to have a sequence that is not typed as an `interval_sequence`, then you have to make a conversion (this can just be a view, so this is not particularly inefficient, but it is complicated
  - conversely, if your function produced an `interval_sequence` you often had to convert is back to a more standard type to use it
- pre- or post-conditions offer the same kind of guarantees as the class invariant
- you may want to reuse the memory of the `interval_sequence` for something else. This is doable through a modifying view, but this will externally break the class invariant: this is counter-productive

