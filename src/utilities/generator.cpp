#include "generator.hpp"

Generator::Generator() : mt_(std::random_device()()) {}

bool Generator::random_bool() { return random_int(0, 1); }

std::mt19937 &Generator::mt() { return mt_; }
