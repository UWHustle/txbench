#include "benchmarks/tatp/tatp_benchmark.h"
#include "../inmem_arrays_TATP_db.h"
#include "inmem_arrays_TATP_db_PALM.h"
#include <iostream>
#include <memory>

#include <glog/logging.h>


int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);

  auto server = std::make_unique<InMemArraysTATPServer<PalmTATPDB>>();
  std::shared_ptr<PalmTATPDB> db = server->db_;

  TATPBenchmark benchmark = TATPBenchmark::parse(argc, argv, std::move(server));
  db->init(benchmark.num_rows());

  double tps = benchmark.run();

  db->print_stats();

  std::cout << "Throughput (tps): " << tps << std::endl;

  return 0;
}
