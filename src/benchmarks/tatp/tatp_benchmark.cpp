#include "tatp_benchmark.h"

#include "tatp_util.h"
#include "tatp_worker.h"
#include "utilities/options.h"
#include "utilities/random_generator.h"
#include <algorithm>
#include <memory>
#include <vector>
#include <climits>

TATPBenchmark::TATPBenchmark(std::unique_ptr<TATPServer> server,
                             size_t num_rows, bool load, size_t num_workers,
                             size_t warmup_duration, size_t measure_duration)
    : Benchmark(load, num_workers, warmup_duration, measure_duration),
      server_(std::move(server)), num_rows_(num_rows) {}

TATPBenchmark TATPBenchmark::parse(int argc, char **argv,
                                   std::unique_ptr<TATPServer> server) {
  Options options(argv[0], "The TATP benchmark");

  options.add<size_t>("num_rows", "Number of rows in the Subscriber table",
                      true);
  options.add<size_t>("num_workers", "Number of worker threads", true);
  options.add<bool>("load", "Load the benchmark data (default: false)", false);
  options.add<size_t>("warmup_duration",
                      "Warmup duration in seconds (default: 10)", false);
  options.add<size_t>("measure_duration",
                      "Measure duration in seconds (default: 60)", false);

  ParseResult result = options.parse(argc, argv);

  size_t num_rows = result["num_rows"].as<size_t>();
  size_t num_workers = result["num_workers"].as<size_t>();

  bool load = true;
  if (result.count("load")) {
    load = true;
  }

  size_t warmup_duration = 2;
  if (result.count("warmup_duration")) {
    warmup_duration = result["warmup_duration"].as<size_t>();
  }

  size_t measure_duration = 2;
  if (result.count("measure_duration")) {
    measure_duration = result["measure_duration"].as<size_t>();
  }

  return TATPBenchmark(std::move(server), num_rows, load, num_workers,
                       warmup_duration, measure_duration);
}

void TATPBenchmark::load() {
  std::unique_ptr<TATPConnection> conn = server_->connect();

  RandomGenerator rg;

  std::vector<int> s_ids(num_rows_);
  std::iota(s_ids.begin(), s_ids.end(), 1);
  std::shuffle(s_ids.begin(), s_ids.end(), rg.mt());

  for (int s_id : s_ids) {
    std::string sub_nbr = leading_zero_pad(15, std::to_string(s_id));

    std::array<bool, 10> bit{};
    for (bool &bit_i : bit) {
      bit_i = rg.random_bool();
    }

    std::array<int, 10> hex{};
    for (int &hex_i : hex) {
      hex_i = rg.random_int(0, 15);
    }

    std::array<int, 10> byte2{};
    for (int &byte2_i : byte2) {
      byte2_i = rg.random_int(0, 255);
    }

    int msc_location = rg.random_int(INT_MIN, INT_MAX);
    int vlr_location = rg.random_int(INT_MIN, INT_MAX);

    conn->new_subscriber_row(s_id, std::move(sub_nbr), bit, hex, byte2,
                             msc_location, vlr_location);

    std::vector<int> ai_type_possible = {1, 2, 3, 4};
    std::vector<int> ai_types;
    std::sample(ai_type_possible.begin(), ai_type_possible.end(),
                std::back_inserter(ai_types), rg.random_int(1, 4), rg.mt());

    for (int ai_type : ai_types) {
      int data_1 = rg.random_int(0, 255);
      int data_2 = rg.random_int(0, 255);
      std::string data3 = uppercase_string(3, rg);
      std::string data4 = uppercase_string(5, rg);

      conn->new_access_info_row(s_id, ai_type, data_1, data_2, std::move(data3),
                                std::move(data4));
    }

    std::vector<int> sf_types_possible = {1, 2, 3, 4};
    std::vector<int> sf_types;
    std::sample(sf_types_possible.begin(), sf_types_possible.end(),
                std::back_inserter(sf_types), rg.random_int(1, 4), rg.mt());

    for (int sf_type : sf_types) {
      bool is_active = rg.random_bool();
      int error_cntrl = rg.random_int(0, 255);
      int data_a = rg.random_int(0, 255);
      std::string data_b = uppercase_string(5, rg);

      conn->new_special_facility_row(s_id, sf_type, is_active, error_cntrl,
                                     data_a, std::move(data_b));

      std::vector<int> start_times_possible = {0, 8, 16};
      std::vector<int> start_times;
      std::sample(start_times_possible.begin(), start_times_possible.end(),
                  std::back_inserter(start_times), rg.random_int(0, 3),
                  rg.mt());

      for (int start_time : start_times) {
        int end_time = start_time + rg.random_int(1, 8);
        std::string numberx = uppercase_string(15, rg);

        conn->new_call_forwarding_row(s_id, sf_type, start_time, end_time,
                                      std::move(numberx));
      }
    }
  }


  conn->done_loading();
}

std::unique_ptr<Worker> TATPBenchmark::make_worker() {
  return std::make_unique<TATPWorker>(num_rows_, server_->connect());
}
