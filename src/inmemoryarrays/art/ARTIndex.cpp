#include <inmemoryarrays/inmem_arrays_TATP_db.h>
#include "ARTIndex.h"

bool ARTIterator::equal(IndexIterator *iter2) {
  return true;
}

bool ARTIterator::next() {
  return false;
}

void ARTIterator::getval(void **val) {
}

bool ARTIndex::insert(int key, void *value) {
  art_insert(&index_, (unsigned char *) &key, sizeof(key), (void *) value);
  return true;
}

bool ARTIndex::search(int key, void **value) {
  void *out = (void *) art_search(&index_, (unsigned char *) &key, sizeof(key));
  if (out == nullptr) { return false; }
  *value = out;
  return true;
}

bool ARTIndex::remove(int key) {
  if (art_delete(&index_, (unsigned char *) &key, sizeof(key))
      != nullptr) { return true; }
  return false;
}

IndexIterator *ARTIndex::lower_bound(int key) {
//  ARTIterator *iter = new ARTIterator(index_.lower_bound(key));
  return nullptr;
}

IndexIterator *ARTIndex::upper_bound(int key) {
//  ARTIterator *iter = new ARTIterator(index_.upper_bound(key));
  return nullptr;
}

typedef struct {
  int count;
  int start_time;
  int end_time;
} tx2_helper;

static int prefix_cb(void *data,
                     const unsigned char *key,
                     uint32_t k_len,
                     void *val) {
  tx2_helper *d = (tx2_helper *) data;
  CFRow* cfrow = (CFRow*) val;

  if (cfrow->start_time > d->start_time ||
      d->end_time >= cfrow->end_time) {
    return -1;
  }

  d->count++;
  return 0;
}

// TODO(chronis) debug this, the prefix key is not correct
int ARTIndex::count_range(int s_id, int sf_type, int start_time, int end_time) {
  tx2_helper helper;
  helper.count = 0;
  helper.start_time = start_time;
  helper.end_time = end_time;
  int prefix_key = to_cf_compound_key(s_id, sf_type, 0);
  int lenpk = sizeof(prefix_key) - 1;

  art_iter_prefix(&index_,
                  (const unsigned char *) &prefix_key,
                  lenpk,
                  prefix_cb,
                  (void *) &helper);

  return helper.count;
}