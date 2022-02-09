#include "txbench/benchmark.hpp"

#include <atomic>
#include <chrono>
#include <memory>
#include <thread>
#include <vector>

double txbench::Benchmark::run(size_t num_workers, size_t warmup_seconds,
                               size_t measure_seconds) {
  std::vector<std::unique_ptr<Worker>> workers;
  workers.reserve(num_workers);

  for (size_t i = 0; i < num_workers; ++i) {
    workers.push_back(make_worker());
  }

  std::vector<std::thread> threads;
  threads.reserve(num_workers);

  std::atomic_bool terminate = false;

  for (const std::unique_ptr<Worker> &worker : workers) {
    threads.emplace_back([&] { worker->work(terminate); });
  }

  std::this_thread::sleep_for(std::chrono::seconds(warmup_seconds));

  size_t warmup_commit_count = 0;
  for (const std::unique_ptr<Worker> &worker : workers) {
    warmup_commit_count += worker->commit_count();
  }

  std::this_thread::sleep_for(std::chrono::seconds(measure_seconds));

  size_t total_commit_count = 0;
  for (const std::unique_ptr<Worker> &worker : workers) {
    total_commit_count += worker->commit_count();
  }

  terminate = true;
  for (std::thread &thread : threads) {
    thread.join();
  }

  size_t measure_commit_count = total_commit_count - warmup_commit_count;
  double tps = (double)measure_commit_count / (double)measure_seconds;
  return tps;
}

txbench::ResultCode txbench::SQLConnection::begin() {
  if (!begin_stmt_) {
    begin_stmt_ = prepare("BEGIN");
  }
  return begin_stmt_->execute();
}

txbench::ResultCode txbench::SQLConnection::commit() {
  if (!commit_stmt_) {
    commit_stmt_ = prepare("COMMIT");
  }
  return commit_stmt_->execute();
}
