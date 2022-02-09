#include "txbench/benchmarks/tatp.hpp"

#include "utilities/generator.hpp"
#include "utilities/sql.hpp"

#include <iostream>
#include <sstream>

#define TATP_ASSERT(x)                                                         \
  do {                                                                         \
    if (!(x)) {                                                                \
      std::cerr << "TATP assertion `" << #x << "` failed" << std::endl;        \
      std::terminate();                                                        \
    }                                                                          \
  } while (false)

static std::string leading_zero_pad(uint64_t x) {
  std::string s = std::to_string(x);
  return std::string(15 - s.length(), '0') + s;
}

class TATPWorker : public txbench::Worker {
public:
  TATPWorker(std::unique_ptr<txbench::TATPClientConnection> conn,
             uint64_t num_rows)
      : conn_(std::move(conn)), num_rows_(num_rows),
        a_(num_rows <= 1000000    ? 65535
           : num_rows <= 10000000 ? 1048575
                                  : 2097151) {}

  void work(std::atomic_bool &terminate) override {
    std::discrete_distribution<int> transaction_distribution(
        {35, 10, 35, 2, 14, 2, 2});

    while (!terminate.load()) {
      int type = transaction_distribution(gen_.prng());
      txbench::ResultCode rc = transaction(type);

      if (rc == txbench::TXBENCH_OK) {
        ++commit_count_;
      }
    }
  }

  txbench::ResultCode transaction(int type) {
    switch (type) {
    case 0: { // GET_SUBSCRIBER_DATA
      // Input:
      uint64_t s_id = generate_s_id();

      // Output:
      std::string sub_nbr;
      std::array<bool, 10> bit{};
      std::array<uint8_t, 10> hex{}, byte2{};
      uint32_t msc_location, vlr_location;

      return conn_->get_subscriber_data(s_id, sub_nbr, bit, hex, byte2,
                                        msc_location, vlr_location);
    }

    case 1: { // GET_NEW_DESTINATION
      // Input:
      uint64_t s_id = generate_s_id();
      uint8_t sf_type = gen_.uniform(1, 4);
      uint8_t start_time = gen_.uniform(0, 2) * 8;
      uint8_t end_time = gen_.uniform(1, 24);

      // Output:
      std::vector<std::string> numberx;

      return conn_->get_new_destination(s_id, sf_type, start_time, end_time,
                                        numberx);
    }

    case 2: { // GET_ACCESS_DATA
      // Input:
      uint64_t s_id = generate_s_id();
      uint8_t ai_type = gen_.uniform(1, 4);

      // Output:
      uint8_t data1, data2;
      std::string data3, data4;

      return conn_->get_access_data(s_id, ai_type, data1, data2, data3, data4);
    }

    case 3: { // UPDATE_SUBSCRIBER_DATA
      // Input:
      uint64_t s_id = generate_s_id();
      bool bit_1 = gen_.uniform<bool>();
      uint8_t sf_type = gen_.uniform(1, 4);
      uint8_t data_a = gen_.uniform(0, 255);

      return conn_->update_subscriber_data(s_id, bit_1, sf_type, data_a);
    }

    case 4: { // UPDATE_LOCATION
      // Input:
      std::string sub_nbr = leading_zero_pad(generate_s_id());
      uint32_t vlr_location = gen_.uniform(1ull, 4294967295ull);

      return conn_->update_location(sub_nbr, vlr_location);
    }

    case 5: { // INSERT_CALL_FORWARDING
      // Input:
      std::string sub_nbr = leading_zero_pad(generate_s_id());
      uint8_t sf_type = gen_.uniform(1, 4);
      uint8_t start_time = gen_.uniform(0, 2) * 8;
      uint8_t end_time = gen_.uniform(1, 24);
      std::string numberx = leading_zero_pad(gen_.uniform(1ull, num_rows_));

      return conn_->insert_call_forwarding(sub_nbr, sf_type, start_time,
                                           end_time, numberx);
    }

    case 6: { // DELETE_CALL_FORWARDING
      // Input:
      std::string sub_nbr = leading_zero_pad(generate_s_id());
      uint8_t sf_type = gen_.uniform(1, 4);
      uint8_t start_time = gen_.uniform(0, 2) * 8;

      return conn_->delete_call_forwarding(sub_nbr, sf_type, start_time);
    }

    default:
      assert(false);
      return txbench::TXBENCH_UNKNOWN;
    }
  }

private:
  uint64_t generate_s_id() {
    return (gen_.uniform(uint64_t(0), a_) |
            gen_.uniform(uint64_t(1), num_rows_)) %
               num_rows_ +
           uint64_t(1);
  }

  std::unique_ptr<txbench::TATPClientConnection> conn_;
  uint64_t num_rows_;
  uint64_t a_;
  txbench::Generator gen_;
};

void txbench::TATPBenchmark::load() {
  static std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  std::unique_ptr<TATPLoaderConnection> conn = db_->connect_loader();

  Generator gen;

  std::vector<TATPSubscriberRecord> subscriber_records;
  std::vector<TATPAccessInfoRecord> access_info_records;
  std::vector<TATPSpecialFacilityRecord> special_facility_records;
  std::vector<TATPCallForwardingRecord> call_forwarding_records;

  std::vector<uint64_t> s_ids(num_rows_);
  std::iota(s_ids.begin(), s_ids.end(), 1);
  std::shuffle(s_ids.begin(), s_ids.end(), gen.prng());

  for (uint64_t s_id : s_ids) {
    std::string sub_nbr = leading_zero_pad(s_id);

    std::array<bool, 10> bit{};
    for (bool &bit_i : bit) {
      bit_i = gen.uniform<bool>();
    }

    std::array<uint8_t, 10> hex{};
    for (uint8_t &hex_i : hex) {
      hex_i = gen.uniform(0, 15);
    }

    std::array<uint8_t, 10> byte2{};
    for (uint8_t &byte2_i : byte2) {
      byte2_i = gen.uniform(0, 255);
    }

    uint32_t msc_location = gen.uniform(1ull, 4294967295ull);
    uint32_t vlr_location = gen.uniform(1ull, 4294967295ull);

    subscriber_records.push_back({s_id, std::move(sub_nbr), bit, hex, byte2,
                                  msc_location, vlr_location});

    for (uint8_t ai_type :
         gen.sample<uint8_t>(gen.uniform(1, 4), {1, 2, 3, 4})) {
      uint8_t data1 = gen.uniform(0, 255);
      uint8_t data2 = gen.uniform(0, 255);
      std::string data3 = gen.string(3, chars);
      std::string data4 = gen.string(5, chars);
      access_info_records.push_back(
          {s_id, ai_type, data1, data2, data3, data4});
    }

    for (uint8_t sf_type :
         gen.sample<uint8_t>(gen.uniform(1, 4), {1, 2, 3, 4})) {
      bool is_active = gen.uniform(0, 99) < 85;
      uint8_t error_cntrl = gen.uniform(0, 255);
      uint8_t data_a = gen.uniform(0, 255);
      std::string data_b = gen.string(5, chars);
      special_facility_records.push_back(
          {s_id, sf_type, is_active, error_cntrl, data_a, data_b});

      for (uint8_t start_time :
           gen.sample<uint8_t>(gen.uniform(0, 3), {0, 8, 16})) {
        uint8_t end_time = start_time + gen.uniform<uint8_t>(1, 8);
        std::string numberx = gen.string(15, chars);
        call_forwarding_records.push_back(
            {s_id, sf_type, start_time, end_time, numberx});
      }
    }

    if (subscriber_records.size() == 1000 || s_id == s_ids.back()) {
      conn->load_subscriber_batch(subscriber_records);
      conn->load_access_info_batch(access_info_records);
      conn->load_special_facility_batch(special_facility_records);
      conn->load_call_forwarding_batch(call_forwarding_records);

      subscriber_records.clear();
      access_info_records.clear();
      special_facility_records.clear();
      call_forwarding_records.clear();
    }
  }
}

std::unique_ptr<txbench::Worker> txbench::TATPBenchmark::make_worker() {
  return std::make_unique<TATPWorker>(db_->connect_client(), num_rows_);
}

txbench::TATPSQLLoaderConnection::TATPSQLLoaderConnection(
    std::unique_ptr<SQLConnection> conn)
    : conn_(std::move(conn)) {}

void txbench::TATPSQLLoaderConnection::load_subscriber_batch(
    const std::vector<TATPSubscriberRecord> &batch) {
  ResultCode rc;

  std::unique_ptr<SQLStatement> stmt =
      conn_->prepare(sql::insert("subscriber", 34));

  rc = conn_->begin();
  TATP_ASSERT(rc == TXBENCH_OK);

  for (const TATPSubscriberRecord &record : batch) {
    stmt->set(1, record.s_id);
    stmt->set(2, record.sub_nbr);
    for (size_t i = 0; i < 10; ++i) {
      stmt->set(i + 3, record.bit[i]);
    }
    for (size_t i = 0; i < 10; ++i) {
      stmt->set(i + 13, record.hex[i]);
    }
    for (size_t i = 0; i < 10; ++i) {
      stmt->set(i + 23, record.byte2[i]);
    }
    stmt->set(33, record.msc_location);
    stmt->set(34, record.vlr_location);
    rc = stmt->execute();
    TATP_ASSERT(rc == TXBENCH_OK);
  }

  rc = conn_->commit();
  TATP_ASSERT(rc == TXBENCH_OK);
}

void txbench::TATPSQLLoaderConnection::load_access_info_batch(
    const std::vector<TATPAccessInfoRecord> &batch) {
  ResultCode rc;

  std::unique_ptr<SQLStatement> stmt =
      conn_->prepare(sql::insert("access_info", 6));

  rc = conn_->begin();
  TATP_ASSERT(rc == TXBENCH_OK);

  for (const TATPAccessInfoRecord &record : batch) {
    stmt->set_all(record.s_id, record.ai_type, record.data1, record.data2,
                  record.data3, record.data4);
    rc = stmt->execute();
    TATP_ASSERT(rc == TXBENCH_OK);
  }

  rc = conn_->commit();
  TATP_ASSERT(rc == TXBENCH_OK);
}

void txbench::TATPSQLLoaderConnection::load_special_facility_batch(
    const std::vector<TATPSpecialFacilityRecord> &batch) {
  ResultCode rc;

  std::unique_ptr<SQLStatement> stmt =
      conn_->prepare(sql::insert("special_facility", 6));

  rc = conn_->begin();
  TATP_ASSERT(rc == TXBENCH_OK);

  for (const TATPSpecialFacilityRecord &record : batch) {
    stmt->set_all(record.s_id, record.sf_type, record.is_active,
                  record.error_cntrl, record.data_a, record.data_b);
    rc = stmt->execute();
    TATP_ASSERT(rc == TXBENCH_OK);
  }

  rc = conn_->commit();
  TATP_ASSERT(rc == TXBENCH_OK);
}

void txbench::TATPSQLLoaderConnection::load_call_forwarding_batch(
    const std::vector<TATPCallForwardingRecord> &batch) {
  ResultCode rc;

  std::unique_ptr<SQLStatement> stmt =
      conn_->prepare(sql::insert("call_forwarding", 5));

  rc = conn_->begin();
  TATP_ASSERT(rc == TXBENCH_OK);

  for (const TATPCallForwardingRecord &record : batch) {
    stmt->set_all(record.s_id, record.sf_type, record.start_time,
                  record.end_time, record.numberx);
    rc = stmt->execute();
    TATP_ASSERT(rc == TXBENCH_OK);
  }

  rc = conn_->commit();
  TATP_ASSERT(rc == TXBENCH_OK);
}

txbench::TATPSQLClientConnection::TATPSQLClientConnection(
    std::unique_ptr<SQLConnection> conn)
    : conn_(std::move(conn)) {
  stmts_[0] = conn_->prepare("SELECT *\n"
                             "FROM subscriber\n"
                             "WHERE s_id = ?");

  stmts_[1] = conn_->prepare("SELECT cf.numberx\n"
                             "FROM special_facility AS sf,\n"
                             "     call_forwarding AS cf\n"
                             "WHERE sf.s_id = ?\n"
                             "  AND sf.sf_type = ?\n"
                             "  AND sf.is_active = 1\n"
                             "  AND cf.s_id = sf.s_id\n"
                             "  AND cf.sf_type = sf.sf_type\n"
                             "  AND cf.start_time <= ?\n"
                             "  AND ? < cf.end_time");

  stmts_[2] = conn_->prepare("SELECT data1, data2, data3, data4\n"
                             "FROM access_info\n"
                             "WHERE s_id = ?\n"
                             "  AND ai_type = ?");

  stmts_[3] = conn_->prepare("UPDATE subscriber\n"
                             "SET bit_1 = ?\n"
                             "WHERE s_id = ?");

  stmts_[4] = conn_->prepare("UPDATE special_facility\n"
                             "SET data_a = ?\n"
                             "WHERE s_id = ?\n"
                             "  AND sf_type = ?");

  stmts_[5] = conn_->prepare("UPDATE subscriber\n"
                             "SET vlr_location = ?\n"
                             "WHERE sub_nbr = ?");

  stmts_[6] = conn_->prepare("SELECT s_id\n"
                             "FROM subscriber\n"
                             "WHERE sub_nbr = ?");

  stmts_[7] = conn_->prepare("SELECT sf_type\n"
                             "FROM special_facility\n"
                             "WHERE s_id = ?");

  stmts_[8] = conn_->prepare("INSERT INTO call_forwarding\n"
                             "VALUES (?, ?, ?, ?, ?)");

  stmts_[9] = conn_->prepare("DELETE\n"
                             "FROM call_forwarding\n"
                             "WHERE s_id = ?\n"
                             "  AND sf_type = ?\n"
                             "  AND start_time = ?");
}

txbench::ResultCode txbench::TATPSQLClientConnection::get_subscriber_data(
    uint64_t s_id, std::string &sub_nbr, std::array<bool, 10> &bit,
    std::array<uint8_t, 10> &hex, std::array<uint8_t, 10> &byte2,
    uint32_t &msc_location, uint32_t &vlr_location) {
  txbench::ResultCode rc;
  size_t count = 0;

  rc = conn_->begin();
  TATP_ASSERT(rc == txbench::TXBENCH_OK);

  // Select from subscriber.
  stmts_[0]->set_all(s_id);
  rc = stmts_[0]->execute([&](SQLCursor &cursor) {
    cursor.get(1, sub_nbr);
    for (size_t i = 0; i < 10; ++i) {
      cursor.get(i + 2, bit[i]);
    }
    for (size_t i = 0; i < 10; ++i) {
      cursor.get(i + 12, hex[i]);
    }
    for (size_t i = 0; i < 10; ++i) {
      cursor.get(i + 22, byte2[i]);
    }
    cursor.get(32, msc_location);
    cursor.get(33, vlr_location);
    ++count;
  });
  TATP_ASSERT(rc == txbench::TXBENCH_OK);
  TATP_ASSERT(count == 1);

  rc = conn_->commit();
  TATP_ASSERT(rc == txbench::TXBENCH_OK);

  return txbench::TXBENCH_OK;
}

txbench::ResultCode txbench::TATPSQLClientConnection::get_new_destination(
    uint64_t s_id, uint8_t sf_type, uint8_t start_time, uint8_t end_time,
    std::vector<std::string> &numberx) {
  txbench::ResultCode rc;
  size_t count = 0;

  rc = conn_->begin();
  TATP_ASSERT(rc == txbench::TXBENCH_OK);

  // Select from special_facility, call_forwarding.
  stmts_[1]->set_all(s_id, sf_type, start_time, end_time);
  rc = stmts_[1]->execute([&](SQLCursor &cursor) {
    numberx.emplace_back();
    cursor.get_all(numberx.back());
    ++count;
  });
  TATP_ASSERT(rc == txbench::TXBENCH_OK);
  TATP_ASSERT(count <= 3);

  rc = conn_->commit();
  TATP_ASSERT(rc == txbench::TXBENCH_OK);

  return count == 0 ? txbench::TXBENCH_MISSING : txbench::TXBENCH_OK;
}

txbench::ResultCode txbench::TATPSQLClientConnection::get_access_data(
    uint64_t s_id, uint8_t ai_type, uint8_t &data1, uint8_t &data2,
    std::string &data3, std::string &data4) {
  txbench::ResultCode rc;
  size_t count = 0;

  // Begin.
  rc = conn_->begin();
  TATP_ASSERT(rc == txbench::TXBENCH_OK);

  // Select from access_info.
  stmts_[2]->set_all(s_id, ai_type);
  rc = stmts_[2]->execute([&](SQLCursor &cursor) {
    cursor.get_all(data1, data2, data3, data4);
    ++count;
  });
  TATP_ASSERT(rc == txbench::TXBENCH_OK);
  TATP_ASSERT(count <= 1);

  rc = conn_->commit();
  TATP_ASSERT(rc == txbench::TXBENCH_OK);

  return count == 0 ? txbench::TXBENCH_MISSING : txbench::TXBENCH_OK;
}

txbench::ResultCode txbench::TATPSQLClientConnection::update_subscriber_data(
    uint64_t s_id, bool bit_1, uint8_t sf_type, uint8_t data_a) {
  txbench::ResultCode rc;
  size_t changes;

  rc = conn_->begin();
  TATP_ASSERT(rc == txbench::TXBENCH_OK);

  // Update subscriber.
  stmts_[3]->set_all(bit_1, s_id);
  rc = stmts_[3]->execute(&changes);
  TATP_ASSERT(rc == txbench::TXBENCH_OK);
  TATP_ASSERT(changes == 1);

  // Update special_facility.
  stmts_[4]->set_all(data_a, s_id, sf_type);
  rc = stmts_[4]->execute(&changes);
  TATP_ASSERT(rc == txbench::TXBENCH_OK);
  TATP_ASSERT(changes == 0 || changes == 1);

  rc = conn_->commit();
  TATP_ASSERT(rc == txbench::TXBENCH_OK);

  return changes == 0 ? txbench::TXBENCH_MISSING : txbench::TXBENCH_OK;
}

txbench::ResultCode
txbench::TATPSQLClientConnection::update_location(const std::string &sub_nbr,
                                                  uint32_t vlr_location) {
  txbench::ResultCode rc;
  size_t changes;

  rc = conn_->begin();
  TATP_ASSERT(rc == txbench::TXBENCH_OK);

  // Update subscriber.
  stmts_[5]->set_all(vlr_location, sub_nbr);
  rc = stmts_[5]->execute(&changes);
  TATP_ASSERT(rc == txbench::TXBENCH_OK);
  TATP_ASSERT(changes == 1);

  rc = conn_->commit();
  TATP_ASSERT(rc == txbench::TXBENCH_OK);

  return txbench::TXBENCH_OK;
}

txbench::ResultCode txbench::TATPSQLClientConnection::insert_call_forwarding(
    std::string sub_nbr, uint8_t sf_type, uint8_t start_time, uint8_t end_time,
    std::string numberx) {
  txbench::ResultCode rc;
  size_t count;
  size_t changes;

  rc = conn_->begin();
  TATP_ASSERT(rc == txbench::TXBENCH_OK);

  // Select from subscriber.
  uint64_t s_id;
  count = 0;
  stmts_[6]->set_all(sub_nbr);
  rc = stmts_[6]->execute([&](SQLCursor &cursor) {
    cursor.get_all(s_id);
    ++count;
  });
  TATP_ASSERT(rc == txbench::TXBENCH_OK);
  TATP_ASSERT(count == 1);

  // Select from special_facility.
  count = 0;
  stmts_[7]->set_all(s_id);
  rc = stmts_[7]->execute([&](SQLCursor &) { ++count; });
  TATP_ASSERT(rc == txbench::TXBENCH_OK);
  TATP_ASSERT(1 <= count && count <= 4);

  // Insert into call_forwarding.
  stmts_[8]->set_all(s_id, sf_type, start_time, end_time, numberx);
  rc = stmts_[8]->execute(&changes);
  TATP_ASSERT(rc == txbench::TXBENCH_CONSTRAINT && changes == 0 ||
              rc == txbench::TXBENCH_OK && changes == 1);

  rc = conn_->commit();
  TATP_ASSERT(rc == txbench::TXBENCH_OK);

  return rc;
}

txbench::ResultCode txbench::TATPSQLClientConnection::delete_call_forwarding(
    const std::string &sub_nbr, uint8_t sf_type, uint8_t start_time) {
  txbench::ResultCode rc;
  size_t count = 0;
  size_t changes;

  rc = conn_->begin();
  TATP_ASSERT(rc == txbench::TXBENCH_OK);

  // Select from subscriber.
  uint64_t s_id;
  count = 0;
  stmts_[6]->set_all(sub_nbr);
  rc = stmts_[6]->execute([&](SQLCursor &cursor) {
    cursor.get_all(s_id);
    ++count;
  });
  TATP_ASSERT(rc == txbench::TXBENCH_OK);
  TATP_ASSERT(count == 1);

  // Delete from call_forwarding.
  stmts_[9]->set_all(s_id, sf_type, start_time);
  rc = stmts_[9]->execute(&changes);
  assert(rc == txbench::TXBENCH_OK);
  assert(changes == 0 || changes == 1);

  rc = conn_->commit();
  TATP_ASSERT(rc == txbench::TXBENCH_OK);

  return changes == 0 ? txbench::TXBENCH_MISSING : txbench::TXBENCH_OK;
}

void txbench::TATPSQLDatabase::create_tables() {
  std::unique_ptr<SQLConnection> conn = db_->connect();
  conn->execute("DROP TABLE IF EXISTS call_forwarding");
  conn->execute("DROP TABLE IF EXISTS special_facility");
  conn->execute("DROP TABLE IF EXISTS access_info");
  conn->execute("DROP TABLE IF EXISTS subscriber");

  std::ostringstream create_subscriber;
  create_subscriber << "CREATE TABLE subscriber ("
                    << "s_id " << db_->config().uint64_type << ", "
                    << "sub_nbr " << db_->config().fixed_string_type(15)
                    << ", ";
  for (int i = 1; i <= 10; ++i) {
    create_subscriber << "bit_" << i << " " << db_->config().bool_type << ", ";
  }
  for (int i = 1; i <= 10; ++i) {
    create_subscriber << "hex_" << i << " " << db_->config().uint8_type << ", ";
  }
  for (int i = 1; i <= 10; ++i) {
    create_subscriber << "byte2_" << i << " " << db_->config().uint8_type
                      << ", ";
  }
  create_subscriber << "msc_location " << db_->config().uint32_type << ", "
                    << "vlr_location " << db_->config().uint32_type << ", "
                    << "PRIMARY KEY (s_id))";
  conn->execute(create_subscriber.str());

  std::ostringstream create_access_info;
  create_access_info << "CREATE TABLE access_info ("
                     << "s_id " << db_->config().uint64_type << ", "
                     << "ai_type " << db_->config().uint8_type << ", "
                     << "data1 " << db_->config().uint8_type << ", "
                     << "data2 " << db_->config().uint8_type << ", "
                     << "data3 " << db_->config().fixed_string_type(3) << ", "
                     << "data4 " << db_->config().fixed_string_type(5) << ", "
                     << "PRIMARY KEY (s_id, ai_type)";
  if (db_->config().enable_foreign_keys) {
    create_access_info << ", FOREIGN KEY (s_id) REFERENCES subscriber (s_id)";
  }
  create_access_info << ")";
  conn->execute(create_access_info.str());

  std::ostringstream create_special_facility;
  create_special_facility << "CREATE TABLE special_facility ("
                          << "s_id " << db_->config().uint64_type << ", "
                          << "sf_type " << db_->config().uint8_type << ", "
                          << "is_active " << db_->config().bool_type << ", "
                          << "error_cntrl " << db_->config().uint8_type << ", "
                          << "data_a " << db_->config().uint8_type << ", "
                          << "data_b " << db_->config().fixed_string_type(5)
                          << ", "
                          << "PRIMARY KEY (s_id, sf_type)";
  if (db_->config().enable_foreign_keys) {
    create_special_facility
        << ", FOREIGN KEY (s_id) REFERENCES subscriber (s_id)";
  }
  create_special_facility << ")";
  conn->execute(create_special_facility.str());

  std::ostringstream create_call_forwarding;
  create_call_forwarding << "CREATE TABLE call_forwarding ("
                         << "s_id " << db_->config().uint64_type << ", "
                         << "sf_type " << db_->config().uint8_type << ", "
                         << "start_time " << db_->config().uint8_type << ", "
                         << "end_time " << db_->config().uint8_type << ", "
                         << "numberx " << db_->config().fixed_string_type(15)
                         << ", "
                         << "PRIMARY KEY (s_id, sf_type, start_time)";
  if (db_->config().enable_foreign_keys) {
    create_call_forwarding << ", FOREIGN KEY (s_id, sf_type) "
                           << "REFERENCES special_facility (s_id, sf_type)";
  }
  create_call_forwarding << ")";
  conn->execute(create_call_forwarding.str());
}

std::unique_ptr<txbench::TATPLoaderConnection>
txbench::TATPSQLDatabase::connect_loader() {
  return std::make_unique<TATPSQLLoaderConnection>(db_->connect());
}

std::unique_ptr<txbench::TATPClientConnection>
txbench::TATPSQLDatabase::connect_client() {
  return std::make_unique<TATPSQLClientConnection>(db_->connect());
}
