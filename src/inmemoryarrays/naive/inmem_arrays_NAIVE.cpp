#include "benchmarks/tatp/tatp_benchmark.h"
#include "../inmem_arrays_TATP_db.h"
#include "NaiveIndex.h"
#include <iostream>
#include <memory>

int main(int argc, char **argv) {
  auto server = std::make_unique<InMemArraysTATPServer<NaiveIndex>>();
  std::shared_ptr<InMemArraysTATPDB<NaiveIndex>> db = server->db_;

  TATPBenchmark benchmark = TATPBenchmark::parse(argc, argv, std::move(server));

  db->init(benchmark.num_rows());
  double tps = benchmark.run();

  db->print_stats();

  std::cout << "Throughput (tps): " << tps << std::endl;

  return 0;
}