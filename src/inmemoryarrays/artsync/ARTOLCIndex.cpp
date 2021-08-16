#include "ARTOLCIndex.h"

#include "../inmem_arrays_TATP_db.h"


int32_t *s_g_keys_;
int32_t *ai_g_keys_;
int32_t *sf_g_keys_;
int32_t *cf_g_keys_;

void loadKey_tatp(TID tid, Key &key, int32_t* arr) {
  // Store the key of the tuple into the key vector
  // Implementation is database specific
  key.setKeyLen(sizeof(int32_t));
  key.set((char*)arr[tid], sizeof(int32_t));
}

void loadKey_tatp_s(TID tid, Key &key) {
  key.setKeyLen(sizeof(int32_t));
  key.set((char*) s_g_keys_[tid], sizeof(int32_t));
}

void loadKey_tatp_ai(TID tid, Key &key) {
  key.setKeyLen(sizeof(int32_t));
  key.set((char*) s_g_keys_[tid], sizeof(int32_t));
}

void loadKey_tatp_sf(TID tid, Key &key) {
  key.setKeyLen(sizeof(int32_t));
  key.set((char*) s_g_keys_[tid], sizeof(int32_t));
}

void loadKey_tatp_cf(TID tid, Key &key){
  key.setKeyLen(sizeof(int32_t));
  key.set((char*) s_g_keys_[tid], sizeof(int32_t));
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

bool ARTOLCIndex::insert(int key_int, void *value, int rowid) {
  auto t = index_->getThreadInfo();
  keys[keys_index_++] = key_int;
  Key key;
  key.setKeyLen(sizeof(int32_t));
  key.set((char*)key_int, sizeof(int32_t));

  index_->insert(key, rowid, t);
  return true;
}

bool ARTOLCIndex::search(int key_int, void **value, void* heapbase, int rowsize_bytes) {
  auto t = index_->getThreadInfo();
  Key key;
  key.setKeyLen(sizeof(int32_t));
  key.set((char*)key_int, sizeof(int32_t));
  auto rowid = index_->lookup(key, t);
  if (rowid == 0) {
    return false;
  }
  *value = (heapbase + rowsize_bytes*rowid);
  return true;
}

bool ARTOLCIndex::remove(int key) {
//  if (index_->erase_one(key) > 0) { return true; }
  return false;
}

IndexIterator *ARTOLCIndex::lower_bound(int key) {
//  ARTOLCIndex *iter = new ARTOLCIndex(index_->lower_bound(key));
  return nullptr;
}

IndexIterator *ARTOLCIndex::upper_bound(int key) {
//  ARTOLCIndex *iter = new ARTOLCIndex(index_->upper_bound(key));
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
