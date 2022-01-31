#include "txbench/benchmarks/tatp.hpp"

#include <array>
#include <iostream>
#include <memory>
#include <vector>

class StdoutTATPLoaderConnection : public TATPLoaderConnection {
public:
  void load_subscriber_batch(
      const std::vector<TATPSubscriberRecord> &batch) override {
    std::cout << "load_subscriber_batch([" << std::endl;

    for (const TATPSubscriberRecord &record : batch) {
      std::cout << "  " << record.s_id << "," << record.sub_nbr << ",";

      for (bool bit_i : record.bit) {
        std::cout << bit_i << ",";
      }

      for (unsigned short hex_i : record.hex) {
        std::cout << hex_i << ",";
      }

      for (unsigned short byte2_i : record.byte2) {
        std::cout << byte2_i << ",";
      }

      std::cout << record.msc_location << "," << record.vlr_location
                << std::endl;
    }

    std::cout << "])" << std::endl;
  }

  void load_access_info_batch(
      const std::vector<TATPAccessInfoRecord> &batch) override {
    std::cout << "load_access_info_batch([" << std::endl;

    for (const TATPAccessInfoRecord &record : batch) {
      std::cout << "  " << record.s_id << "," << record.ai_type << ","
                << record.data1 << "," << record.data2 << "," << record.data3
                << "," << record.data4 << std::endl;
    }

    std::cout << "])" << std::endl;
  }

  void load_special_facility_batch(
      const std::vector<TATPSpecialFacilityRecord> &batch) override {
    std::cout << "load_special_facility_batch([" << std::endl;

    for (const TATPSpecialFacilityRecord &record : batch) {
      std::cout << "  " << record.s_id << "," << record.sf_type << ","
                << record.is_active << "," << record.error_cntrl << ","
                << record.data_a << "," << record.data_b << std::endl;
    }

    std::cout << "])" << std::endl;
  }

  void load_call_forwarding_batch(
      const std::vector<TATPCallForwardingRecord> &batch) override {
    std::cout << "load_call_forwarding_batch([" << std::endl;

    for (const TATPCallForwardingRecord &record : batch) {
      std::cout << "  " << record.s_id << "," << record.sf_type << ","
                << record.start_time << "," << record.end_time << ","
                << record.numberx << std::endl;
    }

    std::cout << "])" << std::endl;
  }
};

class StdoutTATPClientConnection : public TATPClientConnection {
public:
  ReturnCode get_subscriber_data(unsigned long long s_id, std::string *sub_nbr,
                                 std::array<bool, 10> &bit,
                                 std::array<unsigned short, 10> &hex,
                                 std::array<unsigned short, 10> &byte2,
                                 unsigned long *msc_location,
                                 unsigned long *vlr_location) override {
    std::cout << "get_subscriber_data(" << s_id << ")" << std::endl;
    return TXBENCH_SUCCESS;
  }

  ReturnCode get_new_destination(unsigned long long s_id,
                                 unsigned short sf_type,
                                 unsigned short start_time,
                                 unsigned short end_time,
                                 std::vector<std::string> *numberx) override {
    std::cout << "get_new_destination(" << s_id << "," << sf_type << ","
              << start_time << "," << end_time << ")" << std::endl;
    return TXBENCH_SUCCESS;
  }

  ReturnCode get_access_data(unsigned long long s_id, unsigned short ai_type,
                             unsigned short *data1, unsigned short *data2,
                             std::string *data3, std::string *data4) override {
    std::cout << "get_access_data(" << s_id << "," << ai_type << ")"
              << std::endl;
    return TXBENCH_SUCCESS;
  }

  ReturnCode update_subscriber_data(unsigned long long s_id, bool bit_1,
                                    unsigned short sf_type,
                                    unsigned short data_a) override {
    std::cout << "update_subscriber_data(" << s_id << "," << bit_1 << ","
              << sf_type << "," << data_a << ")" << std::endl;
    return TXBENCH_SUCCESS;
  }

  ReturnCode update_location(const std::string &sub_nbr,
                             unsigned long vlr_location) override {
    std::cout << "update_location(" << sub_nbr << "," << vlr_location << ")"
              << std::endl;
    return TXBENCH_SUCCESS;
  }

  ReturnCode insert_call_forwarding(std::string sub_nbr, unsigned short sf_type,
                                    unsigned short start_time,
                                    unsigned short end_time,
                                    std::string numberx) override {
    std::cout << "insert_call_forwarding(" << sub_nbr << "," << sf_type << ","
              << start_time << "," << end_time << "," << numberx << ")"
              << std::endl;
    return TXBENCH_SUCCESS;
  }

  ReturnCode delete_call_forwarding(const std::string &sub_nbr,
                                    unsigned short sf_type,
                                    unsigned short start_time) override {
    std::cout << "delete_call_forwarding(" << sub_nbr << "," << sf_type << ","
              << start_time << ")" << std::endl;
    return TXBENCH_SUCCESS;
  }
};

class StdoutTATPServer : public TATPServer {
public:
  std::unique_ptr<TATPLoaderConnection> connect_loader() override {
    return std::make_unique<StdoutTATPLoaderConnection>();
  }

  std::unique_ptr<TATPClientConnection> connect_client() override {
    return std::make_unique<StdoutTATPClientConnection>();
  }
};

int main(int argc, char **argv) {
  TATPOptions options = TATPOptions::parse(argc, argv);
  auto server = std::make_unique<StdoutTATPServer>();

  TATPBenchmark benchmark(std::move(server), options);

  double tps = benchmark.run();

  std::cout << "Throughput (tps): " << tps << std::endl;

  return 0;
}
