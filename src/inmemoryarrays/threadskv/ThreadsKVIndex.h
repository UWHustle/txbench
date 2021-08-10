#ifndef TXBENCH_SRC_INMEMORYARRAYS_THREADSKV10_LIB_BTREE2INDEX_H_
#define TXBENCH_SRC_INMEMORYARRAYS_THREADSKV10_LIB_BTREE2INDEX_H_
#include "../Index.h"
extern "C" {
#include "threadskv10.h.h"
}
#include <string>

class ThreadskvIndexIterator : public IndexIterator {
 public:
  ThreadskvIndex() {}

  bool next() override;

  bool equal(IndexIterator *iter2) override;

  void getval(void **val) override;

//  BTREE2::BTREE2<int,
//             void*,
//             BTREE2::BTREE2Compare,
//             std::allocator<std::pair<int, void*>>, false>::Iterator iter_;

};

static int uidfile = 333;

class ThreadskvIndex : public Index {
 public:
  ThreadskvIndex() {
    std::string s = std::to_string(uidfile) + "fileindexname";
    uidfile++;
    index_ = bt_open((char*)s.c_str(), BT_rw, 16, 8192);
  }

  ~ThreadskvIndex() {
    bt_close(index_);
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

  BtDb *index_;
};

#endif //TXBENCH_SRC_INMEMORYARRAYS_THREADSKV10_LIB_BTREE2INDEX_H_