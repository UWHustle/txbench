#include "txbench/benchmarks/tatp.hpp"

#include "txbench/benchmark.hpp"
#include "txbench/worker.hpp"

#include "utilities/buffer.hpp"
#include "utilities/generator.hpp"
#include "utilities/options.hpp"

#include <algorithm>
#include <array>
#include <functional>
#include <random>
#include <string>
#include <utility>

std::string leading_zero_pad(size_t length, const std::string &s) {
  assert(length >= s.length());
  return std::string(length - s.length(), '0') + s;
}

std::string uppercase_string(size_t length, Generator &rg) {
  static std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  static std::uniform_int_distribution<size_t> dis(0, chars.size() - 1);

  std::string s(length, 0);
  std::generate(s.begin(), s.end(), [&] {
    return chars[rg.random_int(size_t(0), chars.size() - 1)];
  });

  return s;
}

TATPOptions TATPOptions::parse(int argc, char **argv) {
  Options options(argv[0], "The TATP benchmark");

  options.add<unsigned long long>(
      "num_rows", "Number of rows in the Subscriber table", true);
  options.add<size_t>("num_workers", "Number of worker threads", true);
  options.add<bool>("load", "Load the benchmark data (default: false)", false);
  options.add<size_t>("warmup_duration",
                      "Warmup duration in seconds (default: 10)", false);
  options.add<size_t>("measure_duration",
                      "Measure duration in seconds (default: 60)", false);

  ParseResult result = options.parse(argc, argv);

  auto num_rows = result["num_rows"].as<unsigned long long>();
  auto num_workers = result["num_workers"].as<size_t>();

  bool load = false;
  if (result.count("load")) {
    load = true;
  }

  size_t warmup_duration = 10;
  if (result.count("warmup_duration")) {
    warmup_duration = result["warmup_duration"].as<size_t>();
  }

  size_t measure_duration = 60;
  if (result.count("measure_duration")) {
    measure_duration = result["measure_duration"].as<size_t>();
  }

  return {load, num_rows, num_workers, warmup_duration, measure_duration};
}

class TATPWorker : public Worker {
public:
  TATPWorker(unsigned long long num_rows,
             std::unique_ptr<TATPClientConnection> connection)
      : num_rows_(num_rows), a_val_(num_rows <= 1000000    ? 65535
                                    : num_rows <= 10000000 ? 1048575
                                                           : 2097151),
        connection_(std::move(connection)) {}

  void run(std::atomic_bool &terminate) override {
    Generator rg;

    static unsigned short start_times_possible[] = {0, 8, 16};

    while (!terminate) {
      unsigned long long s_id =
          (rg.random_int(0ull, a_val_) | rg.random_int(1ull, num_rows_)) %
              num_rows_ +
          1ull;

      int transaction_type = rg.random_int(0, 99);

      ReturnCode rc;

      if (transaction_type < 35) {
        // GET_SUBSCRIBER_DATA
        // Probability: 35%
        std::string sub_nbr;
        std::array<bool, 10> bit{};
        std::array<unsigned short, 10> hex{};
        std::array<unsigned short, 10> byte2{};
        unsigned long msc_location, vlr_location;
        rc = connection_->get_subscriber_data(s_id, &sub_nbr, bit, hex, byte2,
                                              &msc_location, &vlr_location);

      } else if (transaction_type < 45) {
        // GET_NEW_DESTINATION
        // Probability: 10%
        unsigned short sf_type = rg.random_int(1, 4);
        unsigned short start_time = start_times_possible[rg.random_int(0, 2)];
        unsigned short end_time = rg.random_int(1, 24);
        std::vector<std::string> numberx;
        rc = connection_->get_new_destination(s_id, sf_type, start_time,
                                              end_time, &numberx);

      } else if (transaction_type < 80) {
        // GET_ACCESS_DATA
        // Probability: 35%
        unsigned short ai_type = rg.random_int(1, 4);
        unsigned short data1, data2;
        std::string data3, data4;
        rc = connection_->get_access_data(s_id, ai_type, &data1, &data2, &data3,
                                          &data4);

      } else if (transaction_type < 82) {
        // UPDATE_SUBSCRIBER_DATA
        // Probability: 2%
        bool bit_1 = rg.random_bool();
        unsigned short sf_type = rg.random_int(1, 4);
        unsigned short data_a = rg.random_int(0, 255);
        rc = connection_->update_subscriber_data(s_id, bit_1, sf_type, data_a);

      } else if (transaction_type < 96) {
        // UPDATE_LOCATION
        // Probability: 14%
        std::string sub_nbr = leading_zero_pad(15, std::to_string(s_id));
        unsigned long vlr_location = rg.random_int(1ull, 4294967295ull);
        rc = connection_->update_location(sub_nbr, vlr_location);

      } else if (transaction_type < 98) {
        // INSERT_CALL_FORWARDING
        // Probability: 2%
        std::string sub_nbr = leading_zero_pad(15, std::to_string(s_id));
        unsigned short sf_type = rg.random_int(1, 4);
        unsigned short start_time = start_times_possible[rg.random_int(0, 2)];
        unsigned short end_time = rg.random_int(1, 24);
        std::string numberx = leading_zero_pad(
            15, std::to_string(rg.random_int(1ull, num_rows_)));
        rc = connection_->insert_call_forwarding(sub_nbr, sf_type, start_time,
                                                 end_time, numberx);

      } else {
        // DELETE_CALL_FORWARDING
        // Probability: 2%
        std::string sub_nbr = leading_zero_pad(15, std::to_string(s_id));
        unsigned short sf_type = rg.random_int(1, 4);
        unsigned short start_time = start_times_possible[rg.random_int(0, 2)];
        rc = connection_->delete_call_forwarding(sub_nbr, sf_type, start_time);
      }

      if (rc == TXBENCH_SUCCESS) {
        ++commit_count_;
      } else if (rc == TXBENCH_FAILURE) {
        throw std::runtime_error("transaction failed for unknown reason");
      }
    }
  }

private:
  unsigned long long num_rows_;
  unsigned long long a_val_;
  std::unique_ptr<TATPClientConnection> connection_;
};

TATPBenchmark::TATPBenchmark(std::unique_ptr<TATPServer> server, bool load,
                             size_t num_rows, size_t num_workers,
                             size_t warmup_duration, size_t measure_duration)
    : Benchmark(load, num_workers, warmup_duration, measure_duration),
      server_(std::move(server)), num_rows_(num_rows) {}

TATPBenchmark::TATPBenchmark(std::unique_ptr<TATPServer> server,
                             TATPOptions options)
    : TATPBenchmark(std::move(server), options.load, options.num_rows,
                    options.num_workers, options.warmup_duration,
                    options.measure_duration) {}

void TATPBenchmark::load() {
  std::unique_ptr<TATPLoaderConnection> conn = server_->connect_loader();

  Generator rg;

  Buffer<TATPSubscriberRecord> subscriber_buffer(
      [&](const auto &batch) { conn->load_subscriber_batch(batch); });

  Buffer<TATPAccessInfoRecord> access_info_buffer(
      [&](const auto &batch) { conn->load_access_info_batch(batch); });

  Buffer<TATPSpecialFacilityRecord> special_facility_buffer(
      [&](const auto &batch) { conn->load_special_facility_batch(batch); });

  Buffer<TATPCallForwardingRecord> call_forwarding_buffer(
      [&](const auto &batch) { conn->load_call_forwarding_batch(batch); });

  std::vector<unsigned long long> s_ids(num_rows_);
  std::iota(s_ids.begin(), s_ids.end(), 1);
  std::shuffle(s_ids.begin(), s_ids.end(), rg.mt());

  for (unsigned long long s_id : s_ids) {
    std::string sub_nbr = leading_zero_pad(15, std::to_string(s_id));

    std::array<bool, 10> bit{};
    for (bool &bit_i : bit) {
      bit_i = rg.random_bool();
    }

    std::array<unsigned short, 10> hex{};
    for (unsigned short &hex_i : hex) {
      hex_i = rg.random_int(0, 15);
    }

    std::array<unsigned short, 10> byte2{};
    for (unsigned short &byte2_i : byte2) {
      byte2_i = rg.random_int(0, 255);
    }

    unsigned long msc_location = rg.random_int(1ull, 4294967295ull);
    unsigned long vlr_location = rg.random_int(1ull, 4294967295ull);

    subscriber_buffer.insert({s_id, std::move(sub_nbr), bit, hex, byte2,
                              msc_location, vlr_location});

    std::vector<unsigned short> ai_type_possible = {1, 2, 3, 4};
    std::vector<unsigned short> ai_types;
    std::sample(ai_type_possible.begin(), ai_type_possible.end(),
                std::back_inserter(ai_types), rg.random_int(1, 4), rg.mt());

    for (unsigned short ai_type : ai_types) {
      unsigned short data_1 = rg.random_int(0, 255);
      unsigned short data_2 = rg.random_int(0, 255);
      std::string data3 = uppercase_string(3, rg);
      std::string data4 = uppercase_string(5, rg);

      access_info_buffer.insert(
          {s_id, ai_type, data_1, data_2, std::move(data3), std::move(data4)});
    }

    std::vector<unsigned short> sf_types_possible = {1, 2, 3, 4};
    std::vector<unsigned short> sf_types;
    std::sample(sf_types_possible.begin(), sf_types_possible.end(),
                std::back_inserter(sf_types), rg.random_int(1, 4), rg.mt());

    for (unsigned short sf_type : sf_types) {
      bool is_active = rg.random_int(0, 99) < 85;
      unsigned short error_cntrl = rg.random_int(0, 255);
      unsigned short data_a = rg.random_int(0, 255);
      std::string data_b = uppercase_string(5, rg);

      special_facility_buffer.insert(
          {s_id, sf_type, is_active, error_cntrl, data_a, std::move(data_b)});

      std::vector<unsigned short> start_times_possible = {0, 8, 16};
      std::vector<unsigned short> start_times;
      std::sample(start_times_possible.begin(), start_times_possible.end(),
                  std::back_inserter(start_times), rg.random_int(0, 3),
                  rg.mt());

      for (unsigned short start_time : start_times) {
        unsigned short end_time = start_time + rg.random_int(1, 8);
        std::string numberx = uppercase_string(15, rg);

        call_forwarding_buffer.insert(
            {s_id, sf_type, start_time, end_time, std::move(numberx)});
      }
    }
  }

  subscriber_buffer.flush();
  access_info_buffer.flush();
  special_facility_buffer.flush();
  call_forwarding_buffer.flush();
}

std::unique_ptr<Worker> TATPBenchmark::make_worker() {
  return std::make_unique<TATPWorker>(num_rows_, server_->connect_client());
}
