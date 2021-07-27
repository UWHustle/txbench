#ifndef TXBENCH_SRC_EXAMPLES_INMEMORYARRAYS_PALM_PALM_TATP_HELPERS_H_
#define TXBENCH_SRC_EXAMPLES_INMEMORYARRAYS_PALM_PALM_TATP_HELPERS_H_

#include <string>
#include "../inmem_arrays_TATP_db.h"


namespace std {
std::string to_string(SRow sr);

std::string to_string(AIRow sr);

std::string to_string(SFRow sr);

std::string to_string(CFRow sr);
}

#endif //TXBENCH_SRC_EXAMPLES_INMEMORYARRAYS_PALM_PALM_TATP_HELPERS_H_
