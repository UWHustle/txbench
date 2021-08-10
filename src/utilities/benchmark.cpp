#include "benchmark.h"

#include <chrono>
#include <memory>
#include <thread>
#include <pthread.h>
#include <vector>
#include <iostream>

Benchmark::Benchmark(bool load, size_t num_workers, size_t warmup_duration,
                     size_t measure_duration)
    : load_(load), num_workers_(num_workers), warmup_duration_(warmup_duration),
      measure_duration_(measure_duration) {}

double Benchmark::run() {
  if (load_) {
    load();
    std::cout << "Loading done" << std::endl;
  }

  std::vector<std::unique_ptr<Worker> > workers;
  workers.reserve(num_workers_);

  for (size_t i = 0; i < num_workers_; ++i) {
    std::unique_ptr<Worker> worker = make_worker();
    workers.push_back(std::move(worker));
  }

  std::vector<std::thread> threads;
  threads.reserve(num_workers_);

  std::atomic_bool terminate = false;
  std::atomic_uint32_t completed_txs = 0;
  uint32_t warmup_commit_count = 0;
  uint32_t total_commit_count = 0;

  for (const std::unique_ptr<Worker> &worker : workers) {
    threads.emplace_back([&] { worker->run(terminate, completed_txs); });
  }

  std::this_thread::sleep_for(std::chrono::seconds(warmup_duration_));

  warmup_commit_count = completed_txs;

  std::this_thread::sleep_for(std::chrono::seconds(measure_duration_));

  total_commit_count = completed_txs;

  terminate = true;
  for (std::thread &thread : threads) {
    thread.join();
  }

  uint32_t measure_commit_count = total_commit_count - warmup_commit_count;
  double tps = (double) measure_commit_count / (double) measure_duration_;

  return tps;
}
