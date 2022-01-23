#include "generator.h"

Generator::Generator() : mt_(std::random_device()()) {}

int Generator::random_int(int a, int b) {
  std::uniform_int_distribution<int> dis(a, b);
  return dis(mt_);
}

size_t Generator::random_size_t(size_t a, size_t b) {
  std::uniform_int_distribution<size_t> dis(a, b);
  return dis(mt_);
}

bool Generator::random_bool() { return random_int(0, 1); }

std::mt19937 &Generator::mt() { return mt_; }
