#include "sqlite3.h"
#include "txbench/benchmarks/tatp.hpp"

#include <iostream>

class SQLite3Cursor : public txbench::SQLCursor {
public:
  explicit SQLite3Cursor(sqlite3_stmt *stmt) : stmt_(stmt) {}

  void get(size_t i, bool &v) override {
    v = sqlite3_column_int(stmt_, (int)i);
  }

  void get(size_t i, uint8_t &v) override {
    v = sqlite3_column_int(stmt_, (int)i);
  }

  void get(size_t i, uint32_t &v) override {
    v = sqlite3_column_int64(stmt_, (int)i);
  }

  void get(size_t i, uint64_t &v) override {
    v = sqlite3_column_int64(stmt_, (int)i);
  }

  void get(size_t i, std::string &v) override {
    v = (char *)sqlite3_column_text(stmt_, (int)i);
  }

private:
  sqlite3_stmt *stmt_;
};

class SQLite3Statement : public txbench::SQLStatement {
public:
  explicit SQLite3Statement(sqlite3_stmt *stmt) : stmt_(stmt) {}

  ~SQLite3Statement() override { sqlite3_finalize(stmt_); }

  void set(size_t i, bool v) override {
    assert_ok(sqlite3_bind_int(stmt_, (int)i, v));
  }

  void set(size_t i, uint8_t v) override {
    assert_ok(sqlite3_bind_int(stmt_, (int)i, v));
  }

  void set(size_t i, uint32_t v) override {
    assert_ok(sqlite3_bind_int64(stmt_, (int)i, v));
  }

  void set(size_t i, uint64_t v) override {
    assert(v <= INT64_MAX);
    assert_ok(sqlite3_bind_int64(stmt_, (int)i, (sqlite3_int64)v));
  }

  void set(size_t i, const std::string &v) override {
    assert_ok(
        sqlite3_bind_text(stmt_, (int)i, v.c_str(), -1, SQLITE_TRANSIENT));
  }

  txbench::ResultCode
  execute(const std::function<void(txbench::SQLCursor &)> &callback,
          size_t *changes) override {
    int rc = sqlite3_step(stmt_);

    while (rc == SQLITE_ROW) {
      SQLite3Cursor cursor(stmt_);
      callback(cursor);
      rc = sqlite3_step(stmt_);
    }

    if (changes) {
      *changes = sqlite3_changes(sqlite3_db_handle(stmt_));
    }

    rc = sqlite3_reset(stmt_);

    switch (rc) {
    case SQLITE_OK:
      return txbench::TXBENCH_OK;
    case SQLITE_CONSTRAINT:
      return txbench::TXBENCH_CONSTRAINT;
    default:
      return txbench::TXBENCH_UNKNOWN;
    }
  }

private:
  void assert_ok(int rc) {
    if (rc != SQLITE_OK) {
      throw std::runtime_error(sqlite3_errmsg(sqlite3_db_handle(stmt_)));
    }
  }

  sqlite3_stmt *stmt_;
};

class SQLite3Connection : public txbench::SQLConnection {
public:
  explicit SQLite3Connection(sqlite3 *db) : db_(db) {}

  ~SQLite3Connection() override { sqlite3_close(db_); }

  std::unique_ptr<txbench::SQLStatement>
  prepare(const std::string &sql) override {
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
      throw std::runtime_error(sqlite3_errmsg(db_));
    }
    return std::make_unique<SQLite3Statement>(stmt);
  }

private:
  sqlite3 *db_;
};

class SQLite3Database : public txbench::SQLDatabase {
public:
  explicit SQLite3Database(std::string filename)
      : txbench::SQLDatabase(
            txbench::SQLConfig{.enable_fixed_string = false,
                               .bool_type = "INTEGER",
                               .uint8_type = "INTEGER",
                               .uint32_type = "INTEGER",
                               .uint64_type = "INTEGER",
                               .variable_string_type = "TEXT"}),
        filename_(std::move(filename)){};

  std::unique_ptr<txbench::SQLConnection> connect() override {
    sqlite3 *db;

    if (sqlite3_open(filename_.c_str(), &db) != SQLITE_OK) {
      throw std::runtime_error(sqlite3_errmsg(db));
    }
    return std::make_unique<SQLite3Connection>(db);
  }

private:
  std::string filename_;
};

int main() {
  auto db = std::make_unique<txbench::TATPSQLDatabase>(
      std::make_unique<SQLite3Database>("tatp.sqlite"));

  db->create_tables();

  txbench::TATPBenchmark tatp(std::move(db), 2);
  tatp.load();
  std::cout << tatp.run(1, 5, 10) << std::endl;

  return 0;
}
