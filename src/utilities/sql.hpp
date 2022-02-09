#ifndef TXBENCH_SQL_HPP
#define TXBENCH_SQL_HPP

#include <sstream>
#include <string>
#include <vector>

namespace txbench::sql {

std::string insert(const std::string &table, size_t n_col, size_t n_row = 1) {
  std::ostringstream sql;
  sql << "INSERT INTO " << table << " VALUES ";
  for (size_t i = 0; i < n_row; ++i) {
    if (i != 0) {
      sql << ',';
    }

    sql << '(';
    for (size_t j = 0; j < n_col; ++j) {
      if (j != 0) {
        sql << ',';
      }
      sql << '?';
    }
    sql << ')';
  }
  return sql.str();
}

} // namespace txbench::sql

#endif // TXBENCH_SQL_HPP
