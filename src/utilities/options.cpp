#include "options.hpp"

Option::Option(std::string key, std::string help,
               std::shared_ptr<const cxxopts::Value> value, bool required)
    : key_(std::move(key)), description_(std::move(help)),
      value_(std::move(value)), required_(required) {}

Options::Options(std::string program, std::string help)
    : program_(std::move(program)), description_(std::move(help)) {}

cxxopts::ParseResult Options::parse(int argc, char **argv) {
  cxxopts::Options inner_options(program_, description_);

  for (const Option &option : options_) {
    inner_options.add_option("", "", option.key_, option.description_,
                             option.value_, "");
  }

  inner_options.add_option("", "", "help", "Print help", cxxopts::value<bool>(),
                           "");

  ParseResult result = inner_options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << inner_options.help() << std::endl;
    exit(0);
  }

  for (const Option &option : options_) {
    if (option.required_ && !result.count(option.key_)) {
      throw cxxopts::option_required_exception(option.key_);
    }
  }

  return result;
}
