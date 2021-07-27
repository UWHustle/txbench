#include "benchmarks/tatp/tatp_benchmark.h"
#include "../inmem_arrays_TATP_db.h"
#include "Btree2Index.h"
#include <iostream>
#include <memory>


int main(int argc, char **argv) {
  auto server = std::make_unique<InMemArraysTATPServer<Btree2Index>>();
  std::shared_ptr<InMemArraysTATPDB<Btree2Index>> db = server->db_;

  TATPBenchmark benchmark = TATPBenchmark::parse(argc, argv, std::move(server));

  db->init(benchmark.num_rows());
  double tps = benchmark.run();

  db->print_stats();

  std::cout << "Btree2 Throughput (tps): " << tps << std::endl;

  return 0;
}
