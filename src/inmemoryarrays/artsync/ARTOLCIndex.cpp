#include "ARTOLCIndex.h"

#include "../inmem_arrays_TATP_db.h"

void loadKey2(TID tid, Key &key) {
  // Store the key of the tuple into the key vector
  // Implementation is database specific
  key.setKeyLen(sizeof(tid));
  reinterpret_cast<uint64_t *>(&key[0])[0] = __builtin_bswap64(tid);
}


bool ARTOLCIterator::equal(IndexIterator *iter2) {
  return true; //iter_ == ((ARTOLCIndex *) iter2)->iter_;
}

bool ARTOLCIterator::next() {
  //iter_++;
  return false;// !iter_.is_end();
}

void ARTOLCIterator::getval(void **val) {
  //*val = iter_.payload();
}

bool ARTOLCIndex::insert(int key, void *value, int rowid) {
//  loadKey2(keys[i], key);
  index_.insert(key, rowid);
  return true;
}

bool ARTOLCIndex::search(int key, void **value, void* heapbase, int rowsize_bytes) {
  auto rowid = index_.lookup(key);
  if (rowid == 0) {
    return false;
  }
  *value = (heapbase + rowsize_bytes*rowid);
  return true;
}

bool ARTOLCIndex::remove(int key) {
//  if (index_.erase_one(key) > 0) { return true; }
  return false;
}

IndexIterator *ARTOLCIndex::lower_bound(int key) {
//  ARTOLCIndex *iter = new ARTOLCIndex(index_.lower_bound(key));
  return nullptr;
}

IndexIterator *ARTOLCIndex::upper_bound(int key) {
//  ARTOLCIndex *iter = new ARTOLCIndex(index_.upper_bound(key));
  return nullptr;
}

int ARTOLCIndex::count_range(int s_id,
                           int sf_type,
                           int start_time,
                           int end_time) {
  int count = 0;

//  IndexIterator
//      *cfiter_l = lower_bound(to_cf_compound_key(s_id, sf_type, 0));
//  IndexIterator *cfiter_u =\
//      upper_bound(to_cf_compound_key(s_id, sf_type, start_time + 1));
//
//  while (!cfiter_l->equal(cfiter_u)) {
//    void *val;
//    cfiter_l->getval(&val);
//    CFRow *cfrow = (CFRow *) val;
//    if (cfrow->end_time > end_time) {
//      count++;
//    }
//    cfiter_l->next();
//  };

  return count;
}
