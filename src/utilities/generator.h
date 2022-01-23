#ifndef TXBENCH_GENERATOR_H
#define TXBENCH_GENERATOR_H

#include <random>
#include <utility>

class Generator {
public:
  Generator();

  int random_int(int a, int b);
  size_t random_size_t(size_t a, size_t b);
  bool random_bool();
  std::mt19937 &mt();

private:
  std::mt19937 mt_;
};

#endif // TXBENCH_GENERATOR_H
