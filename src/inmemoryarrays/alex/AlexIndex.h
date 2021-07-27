#ifndef TXBENCH_SRC_INMEMORYARRAYS_ALEX_ALEX_LIB_ALEXINDEX_H_
#define TXBENCH_SRC_INMEMORYARRAYS_ALEX_ALEX_LIB_ALEXINDEX_H_
#include "../Index.h"
#include "alex_lib/alex.h"

class AlexIterator : public IndexIterator {
 public:
  AlexIterator(alex::Alex<int,
                          void*,
                          alex::AlexCompare,
                          std::allocator<std::pair<int, void*>>,
                          false>::Iterator iter): iter_(iter) {}

  bool next() override;

  bool equal(IndexIterator* iter2) override;

  void getval(void** val) override;

  alex::Alex<int,
             void*,
             alex::AlexCompare,
             std::allocator<std::pair<int, void*>>, false>::Iterator iter_;

};

class AlexIndex : public Index {
 public:

  AlexIndex() {}

  ~AlexIndex() {}

  bool insert(int key, void* value) override;

  bool search(int key, void** value) override;

  IndexIterator* lower_bound(int key) override;

  IndexIterator* upper_bound(int key) override;

  bool remove(int key) override;

  int count_range(int s_id, int sf_type, int start_time, int end_time) override;

  alex::Alex<int,
             void*,
             alex::AlexCompare,
             std::allocator<std::pair<int, void*>>, false>
      index_;
};



#endif //TXBENCH_SRC_INMEMORYARRAYS_ALEX_ALEX_LIB_ALEXINDEX_H_
