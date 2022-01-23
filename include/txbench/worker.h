#ifndef TXBENCH_WORKER_H
#define TXBENCH_WORKER_H

#include <atomic>
#include <utility>

class Worker {
public:
  Worker();
  virtual ~Worker() = default;

  size_t get_commit_count() const;

  virtual void run(std::atomic_bool &terminate) = 0;

protected:
  std::atomic_size_t commit_count_;
};

#endif // TXBENCH_WORKER_H
