#include "txbench/benchmarks/tatp.h"

#include <array>
#include <iostream>
#include <memory>

class StdoutTATPConnection : public TATPConnection {
public:
  void new_subscriber_row(int s_id, std::string sub_nbr,
                          std::array<bool, 10> bit, std::array<int, 10> hex,
                          std::array<int, 10> byte2, int msc_location,
                          int vlr_location) override {
    std::cout << "new_subscriber_row(" << s_id << "," << sub_nbr << ",";

    for (bool bit_i : bit) {
      std::cout << bit_i << ",";
    }

    for (int hex_i : hex) {
      std::cout << hex_i << ",";
    }

    for (int byte2_i : byte2) {
      std::cout << byte2_i << ",";
    }

    std::cout << msc_location << "," << vlr_location << ")" << std::endl;
  }

  void new_access_info_row(int s_id, int ai_type, int data1, int data2,
                           std::string data3, std::string data4) override {
    std::cout << "new_access_info_row(" << s_id << "," << ai_type << ","
              << data1 << "," << data2 << "," << data3 << "," << data4 << ")"
              << std::endl;
  }

  void new_special_facility_row(int s_id, int sf_type, bool is_active,
                                int error_cntrl, int data_a,
                                std::string data_b) override {
    std::cout << "new_special_facility_row(" << s_id << "," << sf_type << ","
              << is_active << "," << error_cntrl << "," << data_a << ","
              << data_b << ")" << std::endl;
  }

  void new_call_forwarding_row(int s_id, int sf_type, int start_time,
                               int end_time, std::string numberx) override {
    std::cout << "new_call_forwarding_row(" << s_id << "," << sf_type << ","
              << start_time << "," << end_time << "," << numberx << ")"
              << std::endl;
  }

  void get_subscriber_data(int s_id, std::string *sub_nbr,
                           std::array<bool, 10> &bit, std::array<int, 10> &hex,
                           std::array<int, 10> &byte2, int *msc_location,
                           int *vlr_location) override {
    std::cout << "get_subscriber_data(" << s_id << ")" << std::endl;
  }

  void get_new_destination(int s_id, int sf_type, int start_time, int end_time,
                           std::string *numberx) override {
    std::cout << "get_new_destination(" << s_id << "," << sf_type << ","
              << start_time << "," << end_time << ")" << std::endl;
  }

  void get_access_data(int s_id, int ai_type, int *data1, int *data2,
                       std::string *data3, std::string *data4) override {
    std::cout << "get_access_data(" << s_id << "," << ai_type << ")"
              << std::endl;
  }

  void update_subscriber_data(int s_id, bool bit_1, int sf_type,
                              int data_a) override {
    std::cout << "update_subscriber_data(" << s_id << "," << bit_1 << ","
              << sf_type << "," << data_a << ")" << std::endl;
  }

  void update_location(const std::string &sub_nbr, int vlr_location) override {
    std::cout << "update_location(" << sub_nbr << "," << vlr_location << ")"
              << std::endl;
  }

  void insert_call_forwarding(std::string sub_nbr, int sf_type, int start_time,
                              int end_time, std::string numberx) override {
    std::cout << "insert_call_forwarding(" << sub_nbr << "," << sf_type << ","
              << start_time << "," << end_time << "," << numberx << ")"
              << std::endl;
  }

  void delete_call_forwarding(const std::string &sub_nbr, int sf_type,
                              int start_time) override {
    std::cout << "delete_call_forwarding(" << sub_nbr << "," << sf_type << ","
              << start_time << ")" << std::endl;
  }
};

class StdoutTATPServer : public TATPServer {
public:
  std::unique_ptr<TATPConnection> connect() override {
    return std::make_unique<StdoutTATPConnection>();
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
