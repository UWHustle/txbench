#include "generator.hpp"

std::string txbench::Generator::string(size_t n,
                                       const std::string &chars) {
  std::string s(n, '\0');
  std::generate(s.begin(), s.end(), [&] {
    return chars[uniform(size_t(0), chars.size() - 1)];
  });

  return s;
}
