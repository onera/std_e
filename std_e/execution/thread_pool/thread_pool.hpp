#pragma once


#include <thread>
#include <deque>
#include <functional>
#include <mutex>
#include <condition_variable>


// REF code from Sean Parent "Better concurrency" (https://www.youtube.com/watch?v=zULU6Hhp42w)
// TODO EFFICIENCY replace std::function by a template param
//     for task_graph, type-erasure has already been taken care of by "any_task"
//     so there is no need to type-erase once more!


namespace std_e {


using lock_t = std::unique_lock<std::mutex>;


class notification_queue {
  private:
    std::deque<std::function<void()>> q;
    std::mutex mut;
    std::condition_variable ready;
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
      while (q.empty() && !done) ready.wait(lock);
      if (q.empty()) return false;
      x = std::move(q.front());
      q.pop_front();
      return true;
    }

    auto
    try_pop(std::function<void()>& x) -> bool {
      lock_t lock(mut,std::try_to_lock);
      if (!lock || q.empty()) return false;
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
    template<class F> auto
    try_push(F&& f) -> bool {
      {
        lock_t lock(mut,std::try_to_lock);
        if (!lock) return false;
        q.emplace_back(FWD(f));
      }
      ready.notify_one();
      return true;
    }
};


class thread_pool {
  private:
    unsigned n;
    std::vector<std::thread> threads;
    std::vector<notification_queue> qs;
    std::atomic<unsigned> idx;
  public:
    thread_pool(unsigned n_thread)
      : n(n_thread)
      , qs(n_thread)
    {
      for (unsigned i=0; i<n_thread; ++i) {
        threads.emplace_back([&,i]{ run(i); });
      }
    }

    ~thread_pool() {
      for (auto& q : qs) q.set_done();
      for (auto& t : threads) t.join();
    }

    auto
    run(unsigned i) -> void {
      while (true) {
        std::function<void()> f;

        for (unsigned k=0; k<n; ++k) {
          if (qs[(i+k)%n].try_pop(f)) break;
        }
        if (!f && !qs[i].pop(f)) break;
        f();
      }
    }

    template<class F> auto
    push_task(F&& f) -> void {
      constexpr unsigned K = 1; // number of rounds trying to find a lock without blocking
      unsigned i = idx.fetch_add(1,std::memory_order_relaxed);

      for (unsigned k=0; k<n*K; ++k) {
        if (qs[(i+k)%n].try_push(FWD(f))) return;
      }
      qs[i%n].push(FWD(f));
    }
};


} // std_e
