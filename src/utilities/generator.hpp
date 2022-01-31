#ifndef TXBENCH_GENERATOR_HPP
#define TXBENCH_GENERATOR_HPP

#include <random>
#include <utility>

class Generator {
public:
  Generator();

  template <typename T> T random_int(T a, T b) {
    std::uniform_int_distribution<T> dis(a, b);
    return dis(mt_);
  }

  bool random_bool();
  std::mt19937 &mt();

private:
  std::mt19937 mt_;
};

#endif // TXBENCH_GENERATOR_HPP
