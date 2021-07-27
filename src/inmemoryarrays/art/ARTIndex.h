#ifndef TXBENCH_SRC_INMEMORYARRAYS_ALEX_ALEX_LIB_ARTIndex_H_
#define TXBENCH_SRC_INMEMORYARRAYS_ALEX_ALEX_LIB_ARTIndex_H_
#include "../Index.h"
#include "art_lib/art.h"

class ARTIterator : public IndexIterator {
 public:
  ARTIterator()  {}

  bool next() override;

  bool equal(IndexIterator* iter2) override;

  void getval(void** val) override;


};

class ARTIndex : public Index {
 public:

  ARTIndex() { art_tree_init(&index_); }

  ~ARTIndex() {art_tree_destroy(&index_);}

  bool insert(int key, void* value) override;

  bool search(int key, void **value) override;

  IndexIterator* lower_bound(int key) override;

  IndexIterator* upper_bound(int key) override;

  bool remove(int key) override;

  int count_range(int s_id, int sf_type, int start_time, int end_time) override;

  art_tree index_;
};



#endif //TXBENCH_SRC_INMEMORYARRAYS_ALEX_ALEX_LIB_ARTIndex_H_
