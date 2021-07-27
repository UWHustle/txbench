#include "benchmarks/tatp/tatp_benchmark.h"
#include "../inmem_arrays_TATP_db.h"
#include "ARTIndex.h"
#include <iostream>
#include <memory>

int main(int argc, char **argv) {
  auto server = std::make_unique<InMemArraysTATPServer<ARTIndex>>();
  std::shared_ptr<InMemArraysTATPDB<ARTIndex>> db = server->db_;

  TATPBenchmark benchmark = TATPBenchmark::parse(argc, argv, std::move(server));

  db->init(benchmark.num_rows());
  double tps = benchmark.run();

  db->print_stats();

  std::cout << "ART Throughput (tps): " << tps << std::endl;

  return 0;
}