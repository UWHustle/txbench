#include "benchmarks/tatp/tatp_benchmark.h"
#include "../inmem_arrays_TATP_db.h"
#include "AlexIndex.h"
#include <iostream>
#include <memory>

#define lockenabled 1

int main(int argc, char **argv) {
  auto server = std::make_unique<InMemArraysTATPServer<AlexIndex>>();
  std::shared_ptr<InMemArraysTATPDB<AlexIndex>> db = server->db_;

  TATPBenchmark benchmark = TATPBenchmark::parse(argc, argv, std::move(server));

  db->init(benchmark.num_rows());
  double tps = benchmark.run();

  db->print_stats();

  std::cout << "ALEX Throughput (tps): " << tps << std::endl;

  return 0;
}
