#include "std_e/unit_test/thread.hpp"


#include <thread>
#include <chrono>


namespace std_e {


auto
sleep_for_seconds(double d) -> void {
  std::this_thread::sleep_for(std::chrono::duration<double>(d));
}


} // std_e
