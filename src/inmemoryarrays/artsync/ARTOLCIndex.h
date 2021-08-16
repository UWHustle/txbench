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

int32_t *s_g_keys_;
int32_t *ai_g_keys_;
int32_t *sf_g_keys_;
int32_t *cf_g_keys_;

void loadKey_tatp_s(TID tid, Key &key);
void loadKey_tatp_ai(TID tid, Key &key);
void loadKey_tatp_sf(TID tid, Key &key);
void loadKey_tatp_cf(TID tid, Key &key);

class ARTOLCIndex : public Index {
 public:

  ARTOLCIndex(int num_rows, std::string name) : Index(num_rows) {
    num_rows_ = num_rows;
    if (name == "s") {
      s_g_keys_ = new int32_t[num_rows_];
      index_ = new ART_OLC::Tree(loadKey_tatp_s);
    } else if (name == "ai") {
      ai_g_keys_ = new int32_t[num_rows_];
      index_ = new ART_OLC::Tree(loadKey_tatp_ai);
    } else if (name == "sf") {
      sf_g_keys_ = new int32_t[num_rows_];
      index_ = new ART_OLC::Tree(loadKey_tatp_sf);
    } else if (name == "cf") {
      cf_g_keys_ = new int32_t[num_rows_];
      index_ = new ART_OLC::Tree(loadKey_tatp_cf);
    }
  }

  ~ARTOLCIndex() {}

  bool insert(int key, void *value, int rowid) override;

  bool search(int key,
              void **value,
              void *heapbase,
              int rowsize_bytes) override;

  IndexIterator *lower_bound(int key) override;

  IndexIterator *upper_bound(int key) override;

  bool remove(int key) override;

  int count_range(int s_id, int sf_type, int start_time, int end_time) override;

  ART_OLC::Tree *index_;
  int num_rows_;
  int keys_index_ = 0;
  int32_t *keys;
};

#endif //TXBENCH_SRC_INMEMORYARRAYS_ARTOLC_LIB_H_
