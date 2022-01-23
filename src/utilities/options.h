#ifndef TXBENCH_OPTIONS_H
#define TXBENCH_OPTIONS_H

#include "cxxopts.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

typedef cxxopts::ParseResult ParseResult;

struct Option {
  Option(std::string key, std::string help,
         std::shared_ptr<const cxxopts::Value> value, bool required);

  std::string key_;
  std::string description_;
  std::shared_ptr<const cxxopts::Value> value_;
  bool required_;
};

class Options {
public:
  Options(std::string program, std::string help);

  template <typename T>
  void add(std::string key, std::string help, bool required) {
    options_.emplace_back(std::move(key), std::move(help), cxxopts::value<T>(),
                          required);
  }

  ParseResult parse(int argc, char **argv);

private:
  std::string program_;
  std::string description_;
  std::vector<Option> options_;
};

#endif // TXBENCH_OPTIONS_H
