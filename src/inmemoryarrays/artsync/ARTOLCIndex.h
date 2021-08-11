#ifndef TXBENCH_SRC_INMEMORYARRAYS_ARTOLC_LIB_H_
#define TXBENCH_SRC_INMEMORYARRAYS_ARTOLC_LIB_H_
#include "../Index.h"
#include "ARTSynchronized/OptimisticLockCoupling/Tree.h"

class ARTOLCIterator : public IndexIterator {
 public:
  ARTOLCIterator(){}

  bool next() override;

  bool equal(IndexIterator* iter2) override;

  void getval(void** val) override;

  void* iter_;

};

void loadKey2(TID tid, Key &key);

class ARTOLCIndex : public Index {
 public:

  ARTOLCIndex(): index_({loadKey}){}

  ~ARTOLCIndex() {}

  bool insert(int key, void* value, int rowid) override;

  bool search(int key, void **value, void* heapbase, int rowsize_bytes) override;

  IndexIterator* lower_bound(int key) override;

  IndexIterator* upper_bound(int key) override;

  bool remove(int key) override;

  int count_range(int s_id, int sf_type, int start_time, int end_time) override;

  ART_OLC::Tree index_;
};



#endif //TXBENCH_SRC_INMEMORYARRAYS_ARTOLC_LIB_H_
