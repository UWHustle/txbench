#ifndef TXBENCH_BENCHMARK_H
#define TXBENCH_BENCHMARK_H

#include "worker.h"

#include <memory>
#include <utility>

class Benchmark {
public:
  Benchmark(bool load, size_t num_workers, size_t warmup_duration,
            size_t measure_duration);

  double run();

protected:
  virtual void load() = 0;
  virtual std::unique_ptr<Worker> make_worker() = 0;

private:
  bool load_;
  size_t num_workers_;
  size_t warmup_duration_;
  size_t measure_duration_;
};

#endif // TXBENCH_BENCHMARK_H
