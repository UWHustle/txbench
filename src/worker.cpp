#include "txbench/worker.hpp"

Worker::Worker() : commit_count_(0) {}

size_t Worker::get_commit_count() const { return commit_count_; }
