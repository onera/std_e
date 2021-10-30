#pragma once


#include <thread>
#include <deque>
#include <functional>

// REF code from Sean Parent Better concurrency (https://www.youtube.com/watch?v=zULU6Hhp42w)
// Note that this task_system here is generally not efficient (as mentionned in the talk)


namespace std_e {


using lock_t = std::unique_lock<std::mutex>;


class notification_queue {
  private:
    std::deque<std::function<void()>> q;
    std::mutex mut;
    condition_variable ready;
    bool done = false;
  public:
    auto
    set_done() -> void {
      {
        lock_t lock(mut);
        done = true;
      }
      ready.notify_all();
    }

    auto
    pop(std::function<void()>& x) -> bool {
      lock_t lock(mut);
      while (q.empty() && !done) ready.wait();
      if (q.empty()) return false;
      x = std::move(q.front());
      q.pop_front();
      return true;
    }

    template<class F> auto
    push(F&& f) -> void {
      {
        lock_t lock(mut);
        q.emplace_back(FWD(f));
      }
      ready.notify_one();
    }
};


class task_system {
  private:
    std::vector<std::thread> threads;
    notification_queue q;

  public:
    task_system(int n_thread) {
      for (int i=0; i<n_thread; ++i) {
        threads.emplace_back([&]{ run(); }
      }
    }

    ~task_system() {
      q.set_done();
       for (auto& t : threads) {
         t.join();
       }
    }

    auto
    run() -> void {
      while (true) {
        std::function<void()> f;
        auto could_pop = q.pop(f);
        STD_E_ASSERT(could_pop);
        f();
      }
    }

    template<class F> auto
    push_task(F&& f) -> void {
      q.push(FWD(f));
    }
};


} // std_e
