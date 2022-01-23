#ifndef TXBENCH_TATP_H
#define TXBENCH_TATP_H

#include "txbench/benchmark.h"

#include <array>
#include <memory>
#include <string>
#include <utility>

struct TATPOptions {
  static TATPOptions parse(int argc, char **argv);

  bool load;
  size_t num_rows;
  size_t num_workers;
  size_t warmup_duration;
  size_t measure_duration;
};

class TATPConnection {
public:
  virtual ~TATPConnection() = default;

  // Loading functions.

  virtual void new_subscriber_row(int s_id, std::string sub_nbr,
                                  std::array<bool, 10> bit,
                                  std::array<int, 10> hex,
                                  std::array<int, 10> byte2, int msc_location,
                                  int vlr_location) = 0;

  virtual void new_access_info_row(int s_id, int ai_type, int data1, int data2,
                                   std::string data3, std::string data4) = 0;

  virtual void new_special_facility_row(int s_id, int sf_type, bool is_active,
                                        int error_cntrl, int data_a,
                                        std::string data_b) = 0;

  virtual void new_call_forwarding_row(int s_id, int sf_type, int start_time,
                                       int end_time, std::string numberx) = 0;

  // Benchmark functions.

  virtual void get_subscriber_data(int s_id, std::string *sub_nbr,
                                   std::array<bool, 10> &bit,
                                   std::array<int, 10> &hex,
                                   std::array<int, 10> &byte2,
                                   int *msc_location, int *vlr_location) = 0;

  virtual void get_new_destination(int s_id, int sf_type, int start_time,
                                   int end_time, std::string *numberx) = 0;

  virtual void get_access_data(int s_id, int ai_type, int *data1, int *data2,
                               std::string *data3, std::string *data4) = 0;

  virtual void update_subscriber_data(int s_id, bool bit_1, int sf_type,
                                      int data_a) = 0;

  virtual void update_location(const std::string &sub_nbr,
                               int vlr_location) = 0;

  virtual void insert_call_forwarding(std::string sub_nbr, int sf_type,
                                      int start_time, int end_time,
                                      std::string numberx) = 0;

  virtual void delete_call_forwarding(const std::string &sub_nbr, int sf_type,
                                      int start_time) = 0;
};

class TATPServer {
public:
  virtual ~TATPServer() = default;
  virtual std::unique_ptr<TATPConnection> connect() = 0;
};

class TATPBenchmark : public Benchmark {
public:
  TATPBenchmark(std::unique_ptr<TATPServer> server, bool load, size_t num_rows,
                size_t num_workers, size_t warmup_duration,
                size_t measure_duration);

  TATPBenchmark(std::unique_ptr<TATPServer> server, TATPOptions options);

protected:
  void load() override;
  std::unique_ptr<Worker> make_worker() override;

private:
  std::unique_ptr<TATPServer> server_;
  size_t num_rows_;
};

#endif // TXBENCH_TATP_H
