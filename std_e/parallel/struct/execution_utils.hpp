#pragma once


namespace std_e {


// DOC the ... args are there in case other tasks are needed before opening/closing
//    example: join(dist_array_0,fut_1,fut_2) | then(open_epoch)
//             here, fut_1 and fut_2 are not used by open_epoch, but must be executed before open_epoch


// open/close epoch {
constexpr auto open_epoch = []<class T>(const dist_array<T>& a, auto&&...) -> const dist_array<T>& {
  int assertion = 0;
  int err = MPI_Win_lock_all(assertion,a.win().underlying());
  STD_E_ASSERT(!err);
  return a;
};
constexpr auto close_epoch = []<class T>(const dist_array<T>& a, auto&&...) -> const dist_array<T>& {
  int err = MPI_Win_unlock_all(a.win().underlying());
  STD_E_ASSERT(!err);
  return a;
};
// open/close epoch }


constexpr auto extract_result_fn = [](const auto& x, auto&&...) {
  return std::move(x);
};

// TODO fix overconstrained execute_seq: does not want to return references
//constexpr auto local_array_fn = []<class T>(const dist_array<T>& a, auto&&...) -> const dist_array<T>& {
//  return a.local();
//}
constexpr auto local_array_fn = []<class T>(const dist_array<T>& a, auto&&...) {
  return 0;
};


} // std_e
