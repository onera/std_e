#pragma once


namespace std_e {


// TODO Rename, generalize, clean, rangify...
template<class Range_0, class Range_1, class Range_2> auto
apply_step(Range_0& rng, const Range_1& indices, const Range_2& step_values) -> void {
  STD_E_ASSERT(indices[0]==0);
  STD_E_ASSERT(indices.back()==(int)rng.size());
  STD_E_ASSERT(step_values.size()>=indices.size());
  auto idx = indices.begin()+1;
  auto step_val = step_values.begin();
  for (int i=0; i<(int)rng.size(); ++i) {
    while (*idx<=i) { ++idx; ++step_val; }
    rng[i] -= *step_val;
  }
}


} // std_e
