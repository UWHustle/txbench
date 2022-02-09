#ifndef TXBENCH_BENCHMARK_HPP
#define TXBENCH_BENCHMARK_HPP

#include <atomic>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace txbench {

enum ResultCode {
  TXBENCH_OK,
  TXBENCH_MISSING,
  TXBENCH_CONSTRAINT,
  TXBENCH_UNKNOWN
};

class Worker {
public:
  Worker() : commit_count_(0) {}

  virtual ~Worker() = default;

  size_t commit_count() { return commit_count_; }

  virtual void work(std::atomic_bool &terminate) = 0;

protected:
  std::atomic_size_t commit_count_;
};

class Benchmark {
public:
  virtual ~Benchmark() = default;

  double run(size_t num_workers, size_t warmup_seconds, size_t measure_seconds);

protected:
  virtual std::unique_ptr<Worker> make_worker() = 0;
};

class SQLCursor {
public:
  SQLCursor() : halted_(false) {}

  virtual ~SQLCursor() = default;

  /**
   * Gets a boolean value.
   * @param i The index of the value to get. Undefined behavior if out of range.
   * @param v Sets this to the boolean value.
   */
  virtual void get(size_t i, bool &v) = 0;

  /**
   * Gets a uint8 value.
   * @param i The index of the value to get. Undefined behavior if out of range.
   * @param v Sets this to the uint8 value.
   */
  virtual void get(size_t i, uint8_t &v) = 0;

  /**
   * Gets a uint32 value.
   * @param i The index of the value to get. Undefined behavior if out of range.
   * @param v Sets this to the uint32 value.
   */
  virtual void get(size_t i, uint32_t &v) = 0;

  /**
   * Gets a uint64 value.
   * @param i The index of the value to get. Undefined behavior if out of range.
   * @param v Sets this to the uint64 value.
   */
  virtual void get(size_t i, uint64_t &v) = 0;

  /**
   * Gets a string value.
   * @param i The index of the value to get. Undefined behavior if out of range.
   * @param v Sets this to the string value.
   */
  virtual void get(size_t i, std::string &v) = 0;

  /**
   * Gets all the values.
   * @tparam T The types of the values.
   * @param v Sets the contents of this to the values.
   */
  template <typename... T> void get_all(T &...vs) {
    get_all(std::index_sequence_for<T...>(), vs...);
  }

  /**
   * Requests that the cursor be halted.
   */
  void halt() { halted_ = true; }

  /**
   * Whether the cursor is halted.
   * @return true if the cursor is halted, false otherwise.
   */
  [[nodiscard]] bool halted() const { return halted_; }

private:
  template <size_t... i, typename... T>
  void get_all(std::index_sequence<i...>, T &...vs) {
    (get(i, vs), ...);
  }

  bool halted_;
};

class SQLStatement {
public:
  virtual ~SQLStatement() = default;

  /**
   * Sets a bool value.
   * @param i The index of the value to set. Undefined behavior if out of range.
   * @param v The bool value.
   */
  virtual void set(size_t i, bool v) = 0;

  /**
   * Sets a uint8 value.
   * @param i The index of the value to set. Undefined behavior if out of range.
   * @param v The uint8 value.
   */
  virtual void set(size_t i, uint8_t v) = 0;

  /**
   * Sets a uint32 value.
   * @param i The index of the value to set. Undefined behavior if out of range.
   * @param v The uint32 value.
   */
  virtual void set(size_t i, uint32_t v) = 0;

  /**
   * Sets a uint64 value.
   * @param i The index of the value to set. Undefined behavior if out of range.
   * @param v The uint64 value.
   */
  virtual void set(size_t i, uint64_t v) = 0;

  /**
   * Sets a string value.
   * @param i The index of the value to set. Undefined behavior if out of range.
   * @param v The string value.
   */
  virtual void set(size_t i, const std::string &v) = 0;

  /**
   * Executes the prepared statement with the bound arguments.
   * @param callback Calls for each row in the result.
   * @param changes If not null, sets to the number of rows modified.
   * @return TXBENCH_OK if successful, or an error code otherwise.
   */
  virtual ResultCode execute(const std::function<void(SQLCursor &)> &callback,
                             size_t *changes) = 0;

  /**
   * Executes the prepared statement.
   * @param callback Calls for each row in the result.
   * @return TXBENCH_OK if successful, or an error code otherwise.
   */
  ResultCode execute(const std::function<void(SQLCursor &)> &callback) {
    return execute(callback, nullptr);
  }

  /**
   * Executes the prepared statement.
   * @param changes If not null, sets to the number of rows modified.
   * @return TXBENCH_OK if successful, or an error code otherwise.
   */
  ResultCode execute(size_t *changes = nullptr) {
    return execute([](SQLCursor &) {}, changes);
  }

  /**
   * Sets all the values.
   * @tparam T The types of the values.
   * @param vs The values.
   */
  template <typename... T> void set_all(T... vs) {
    set_all(std::index_sequence_for<T...>(), vs...);
  }

  /**
   * Deleted to prevent implicit conversions.
   */
  template <typename T> void set(size_t i, T v) = delete;

private:
  template <size_t... i, typename... T>
  void set_all(std::index_sequence<i...>, T... vs) {
    (set(i + 1, vs), ...);
  }
};

class SQLConnection {
public:
  virtual ~SQLConnection() = default;

  /**
   * Prepares a SQL statement.
   * @param sql The SQL statement.
   * @return A pointer to the prepared statement.
   */
  virtual std::unique_ptr<SQLStatement> prepare(const std::string &sql) = 0;

  /**
   * Executes a SQL statement.
   * @param sql The SQL statement.
   * @param changes If not null, sets to the number of rows modified.
   * @return TXBENCH_OK if successful, or an error code otherwise.
   */
  ResultCode execute(const std::string &sql, size_t *changes = nullptr) {
    return prepare(sql)->execute(changes);
  }

  /**
   * Executes a SQL statement.
   * @param sql The SQL statement.
   * @param callback Calls for each row in the result.
   * @param changes If not null, sets to the number of rows modified.
   * @return TXBENCH_OK if successful, or an error code otherwise.
   */
  ResultCode execute(const std::string &sql,
                     const std::function<void(SQLCursor &)> &callback,
                     size_t *changes = nullptr) {
    return prepare(sql)->execute(callback, changes);
  }

  ResultCode begin();

  ResultCode commit();

private:
  std::unique_ptr<SQLStatement> begin_stmt_;
  std::unique_ptr<SQLStatement> commit_stmt_;
};

struct SQLConfig {
  bool enable_foreign_keys = true;
  bool enable_fixed_string = true;

  std::string bool_type = "BOOLEAN";
  std::string uint8_type = "UINT8";
  std::string uint32_type = "UINT32";
  std::string uint64_type = "UINT64";
  std::string variable_string_type = "VARCHAR";
  std::string fixed_string_prefix = "CHAR";

  [[nodiscard]] std::string fixed_string_type(size_t n) const {
    if (enable_fixed_string) {
      return fixed_string_prefix + "(" + std::to_string(n) + ")";
    }
    return variable_string_type;
  }
};

class SQLDatabase {
public:
  explicit SQLDatabase(SQLConfig config) : config_(std::move(config)) {}

  virtual ~SQLDatabase() = default;

  virtual std::unique_ptr<SQLConnection> connect() = 0;

  const SQLConfig &config() { return config_; }

private:
  SQLConfig config_;
};

} // namespace txbench

#endif // TXBENCH_BENCHMARK_HPP
