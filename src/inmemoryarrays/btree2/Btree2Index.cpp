#include "Btree2Index.h"

#include "../inmem_arrays_TATP_db.h"

bool Btree2Iterator::equal(IndexIterator *iter2) {
//  return iter_ == ((Btree2Iterator *) iter2)->iter_;
}

bool Btree2Iterator::next() {
//  iter_++;
//  return !iter_.is_end();
}

void Btree2Iterator::getval(void **val) {
//  *val = iter_.payload();
}

bool Btree2Index::insert(int key, void *value) {
//  index_.insert(key, value);
//  return true;
}

bool Btree2Index::search(int key, void **value) {
//  void **out = index_.get_payload(key);
//  if (out == nullptr) {
//    return false;
//  }
//  *value = *out;
//  return true;
}

bool Btree2Index::remove(int key) {
//  if (index_.erase_one(key) > 0) { return true; }
//  return false;
}

IndexIterator *Btree2Index::lower_bound(int key) {
//  Btree2Iterator *iter = new Btree2Iterator(index_.lower_bound(key));
//  return iter;
}

IndexIterator *Btree2Index::upper_bound(int key) {
//  Btree2Iterator *iter = new Btree2Iterator(index_.upper_bound(key));
//  return iter;
}

int Btree2Index::count_range(int s_id,
                           int sf_type,
                           int start_time,
                           int end_time) {
  int count = 0;

//  IndexIterator
//      *cfiter_l = lower_bound(to_cf_compound_key(s_id, sf_type, 0));
//  IndexIterator *cfiter_u =
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