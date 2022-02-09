#ifndef TXBENCH_TATP_H
#define TXBENCH_TATP_H

#include "txbench/benchmark.hpp"

#include <array>
#include <string>

namespace txbench {

struct TATPSubscriberRecord {
  uint64_t s_id;
  std::string sub_nbr;
  std::array<bool, 10> bit;
  std::array<uint8_t, 10> hex;
  std::array<uint8_t, 10> byte2;
  uint32_t msc_location;
  uint32_t vlr_location;
};

struct TATPAccessInfoRecord {
  uint64_t s_id;
  uint8_t ai_type;
  uint8_t data1;
  uint8_t data2;
  std::string data3;
  std::string data4;
};

struct TATPSpecialFacilityRecord {
  uint64_t s_id;
  uint8_t sf_type;
  bool is_active;
  uint8_t error_cntrl;
  uint8_t data_a;
  std::string data_b;
};

struct TATPCallForwardingRecord {
  uint64_t s_id;
  uint8_t sf_type;
  uint8_t start_time;
  uint8_t end_time;
  std::string numberx;
};

class TATPLoaderConnection {
public:
  virtual ~TATPLoaderConnection() = default;

  virtual void
  load_subscriber_batch(const std::vector<TATPSubscriberRecord> &batch) = 0;

  virtual void
  load_access_info_batch(const std::vector<TATPAccessInfoRecord> &batch) = 0;

  virtual void load_special_facility_batch(
      const std::vector<TATPSpecialFacilityRecord> &batch) = 0;

  virtual void load_call_forwarding_batch(
      const std::vector<TATPCallForwardingRecord> &batch) = 0;
};

class TATPClientConnection {
public:
  virtual ~TATPClientConnection() = default;

  virtual ResultCode get_subscriber_data(uint64_t s_id, std::string &sub_nbr,
                                         std::array<bool, 10> &bit,
                                         std::array<uint8_t, 10> &hex,
                                         std::array<uint8_t, 10> &byte2,
                                         uint32_t &msc_location,
                                         uint32_t &vlr_location) = 0;

  virtual ResultCode get_new_destination(uint64_t s_id, uint8_t sf_type,
                                         uint8_t start_time, uint8_t end_time,
                                         std::vector<std::string> &numberx) = 0;

  virtual ResultCode get_access_data(uint64_t s_id, uint8_t ai_type,
                                     uint8_t &data1, uint8_t &data2,
                                     std::string &data3,
                                     std::string &data4) = 0;

  virtual ResultCode update_subscriber_data(uint64_t s_id, bool bit_1,
                                            uint8_t sf_type,
                                            uint8_t data_a) = 0;

  virtual ResultCode update_location(const std::string &sub_nbr,
                                     uint32_t vlr_location) = 0;

  virtual ResultCode insert_call_forwarding(std::string sub_nbr,
                                            uint8_t sf_type, uint8_t start_time,
                                            uint8_t end_time,
                                            std::string numberx) = 0;

  virtual ResultCode delete_call_forwarding(const std::string &sub_nbr,
                                            uint8_t sf_type,
                                            uint8_t start_time) = 0;
};

class TATPDatabase {
public:
  virtual ~TATPDatabase() = default;

  virtual void create_tables() = 0;

  virtual std::unique_ptr<TATPLoaderConnection> connect_loader() = 0;

  virtual std::unique_ptr<TATPClientConnection> connect_client() = 0;
};

class TATPBenchmark : public Benchmark {
public:
  explicit TATPBenchmark(std::unique_ptr<TATPDatabase> db, uint64_t num_rows)
      : db_(std::move(db)), num_rows_(num_rows) {}

  void load();

private:
  std::unique_ptr<Worker> make_worker() override;

  std::unique_ptr<TATPDatabase> db_;
  uint64_t num_rows_;
};

class TATPSQLLoaderConnection : public TATPLoaderConnection {
public:
  explicit TATPSQLLoaderConnection(std::unique_ptr<SQLConnection> conn);

  void load_subscriber_batch(
      const std::vector<TATPSubscriberRecord> &batch) override;

  void load_access_info_batch(
      const std::vector<TATPAccessInfoRecord> &batch) override;

  void load_special_facility_batch(
      const std::vector<TATPSpecialFacilityRecord> &batch) override;

  void load_call_forwarding_batch(
      const std::vector<TATPCallForwardingRecord> &batch) override;

private:
  std::unique_ptr<SQLConnection> conn_;
};

class TATPSQLClientConnection : public TATPClientConnection {
public:
  explicit TATPSQLClientConnection(std::unique_ptr<SQLConnection> conn);

  ResultCode get_subscriber_data(uint64_t s_id, std::string &sub_nbr,
                                 std::array<bool, 10> &bit,
                                 std::array<uint8_t, 10> &hex,
                                 std::array<uint8_t, 10> &byte2,
                                 uint32_t &msc_location,
                                 uint32_t &vlr_location) override;

  ResultCode get_new_destination(uint64_t s_id, uint8_t sf_type,
                                 uint8_t start_time, uint8_t end_time,
                                 std::vector<std::string> &numberx) override;

  ResultCode get_access_data(uint64_t s_id, uint8_t ai_type, uint8_t &data1,
                             uint8_t &data2, std::string &data3,
                             std::string &data4) override;

  ResultCode update_subscriber_data(uint64_t s_id, bool bit_1, uint8_t sf_type,
                                    uint8_t data_a) override;

  ResultCode update_location(const std::string &sub_nbr,
                             uint32_t vlr_location) override;

  ResultCode insert_call_forwarding(std::string sub_nbr, uint8_t sf_type,
                                    uint8_t start_time, uint8_t end_time,
                                    std::string numberx) override;

  ResultCode delete_call_forwarding(const std::string &sub_nbr, uint8_t sf_type,
                                    uint8_t start_time) override;

private:
  std::unique_ptr<SQLConnection> conn_;
  std::array<std::unique_ptr<SQLStatement>, 10> stmts_;
};

class TATPSQLDatabase : public TATPDatabase {
public:
  explicit TATPSQLDatabase(std::unique_ptr<SQLDatabase> db)
      : db_(std::move(db)) {}

  void create_tables() override;

  std::unique_ptr<TATPLoaderConnection> connect_loader() override;

  std::unique_ptr<TATPClientConnection> connect_client() override;

private:
  std::unique_ptr<SQLDatabase> db_;
};

} // namespace txbench

#endif // TXBENCH_TATP_H
