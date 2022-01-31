#ifndef TXBENCH_BUFFER_HPP
#define TXBENCH_BUFFER_HPP

#include <functional>
#include <utility>
#include <vector>

template <typename T> class Buffer {
public:
  explicit Buffer(std::function<void(const std::vector<T> &)> &&callback,
                  size_t capacity = 1000)
      : callback_(callback), capacity_(capacity) {
    buffer_.reserve(capacity_);
  }

  void insert(T &&item) {
    buffer_.push_back(item);
    if (buffer_.size() == capacity_) {
      flush();
    }
  }

  void flush() {
    callback_(buffer_);
    buffer_.clear();
  }

private:
  std::function<void(const std::vector<T> &)> callback_;
  std::vector<T> buffer_;
  size_t capacity_;
};

#endif // TXBENCH_BUFFER_HPP
