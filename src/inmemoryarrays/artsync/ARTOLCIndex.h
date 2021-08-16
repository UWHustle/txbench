#ifndef TXBENCH_SRC_INMEMORYARRAYS_ARTOLC_LIB_H_
#define TXBENCH_SRC_INMEMORYARRAYS_ARTOLC_LIB_H_
#include "../Index.h"
#include "ARTSynchronized/OptimisticLockCoupling/Tree.h"

class ARTOLCIterator : public IndexIterator {
 public:
  ARTOLCIterator() {}

  bool next() override;

  bool equal(IndexIterator *iter2) override;

  void getval(void **val) override;

  void *iter_;

};

void loadKey_tatp(TID tid, Key &key, int32_t* arr);

class ARTOLCIndex : public Index {
 public:

  ARTOLCIndex(int num_rows)
      : num_rows_(num_rows), index_({loadKey}), Index(num_rows) {
    keys = new int32_t[num_rows_];
  }

  ~ARTOLCIndex() {
    delete[] keys;
  }

  bool insert(int key, void *value, int rowid) override;

  bool search(int key,
              void **value,
              void *heapbase,
              int rowsize_bytes) override;

  IndexIterator *lower_bound(int key) override;

  IndexIterator *upper_bound(int key) override;

  bool remove(int key) override;

  int count_range(int s_id, int sf_type, int start_time, int end_time) override;

  ART_OLC::Tree index_;
  int num_rows_;
  int keys_index_ = 0;
  int32_t *keys;
};

#endif //TXBENCH_SRC_INMEMORYARRAYS_ARTOLC_LIB_H_
