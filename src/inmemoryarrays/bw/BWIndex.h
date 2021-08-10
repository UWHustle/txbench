#ifndef TXBENCH_SRC_INMEMORYARRAYS_BW_BWINDEX_H_
#define TXBENCH_SRC_INMEMORYARRAYS_BW_BWINDEX_H_

#include "../Index.h"
#include "BwTree/src/bwtree.h"
#include <atomic>

using namespace wangziqi2013::bwtree;
//
class KeyComparator1 {
 public:
  inline bool operator()(const int k1, const int k2) const {
    return k1 < k2;
  }

  KeyComparator1(int dummy) {
    (void) dummy;

    return;
  }

  KeyComparator1() = delete;
  //KeyComparator(const KeyComparator &p_key_cmp_obj) = delete;
};

class KeyEqualityChecker1 {
 public:
  inline bool operator()(const int k1, const int k2) const {
    return k1 == k2;
  }

  KeyEqualityChecker1(int dummy) {
    (void) dummy;

    return;
  }

  KeyEqualityChecker1() = delete;
  //KeyEqualityChecker(const KeyEqualityChecker &p_key_eq_obj) = delete;
};

using BwTreeType = BwTree<int,
                          void*,
                          KeyComparator1,
                          KeyEqualityChecker1>;



class BWIterator : public IndexIterator {
 public:
  BWIterator() {}

  bool next() override;

  bool equal(IndexIterator *iter2) override;

  void getval(void **val) override;

  BwTreeType::ForwardIterator* iter_;
};



class BWIndex : public Index {
 public:

  BWIndex() {
    index_ = new BwTreeType{true,
                                KeyComparator1{1},
                                KeyEqualityChecker1{1}};
    index_->UpdateThreadLocal(10);
    index_->AssignGCID(0);

    print_flag = false;
  }

  ~BWIndex() {  }

  bool insert(int key, void *value, int rowid)
  override;

  bool search(int key, void **value, void *heapbase, int rowsize_bytes)
  override;

  IndexIterator *lower_bound(int key)
  override;

  IndexIterator *upper_bound(int key)
  override;

  bool remove(int key)
  override;

  int count_range(int s_id, int sf_type, int start_time, int end_time)
  override;

  BwTreeType* index_;
};

#endif //TXBENCH_SRC_INMEMORYARRAYS_BW_BWINDEX_H_
