#include "AlexIndex.h"

#include "../inmem_arrays_TATP_db.h"

bool AlexIterator::equal(IndexIterator *iter2) {
  return iter_ == ((AlexIterator *) iter2)->iter_;
}

bool AlexIterator::next() {
  iter_++;
  return !iter_.is_end();
}

void AlexIterator::getval(void **val) {
  *val = iter_.payload();
}

bool AlexIndex::insert(int key, void *value) {
  index_.insert(key, value);
  return true;
}

bool AlexIndex::search(int key, void **value) {
  void **out = index_.get_payload(key);
  if (out == nullptr) {
    return false;
  }
  *value = *out;
  return true;
}

bool AlexIndex::remove(int key) {
  if (index_.erase_one(key) > 0) { return true; }
  return false;
}

IndexIterator *AlexIndex::lower_bound(int key) {
  AlexIterator *iter = new AlexIterator(index_.lower_bound(key));
  return iter;
}

IndexIterator *AlexIndex::upper_bound(int key) {
  AlexIterator *iter = new AlexIterator(index_.upper_bound(key));
  return iter;
}

int AlexIndex::count_range(int s_id,
                           int sf_type,
                           int start_time,
                           int end_time) {
  int count = 0;

  IndexIterator
      *cfiter_l = lower_bound(to_cf_compound_key(s_id, sf_type, 0));
  IndexIterator *cfiter_u =
      upper_bound(to_cf_compound_key(s_id, sf_type, start_time + 1));

  while (!cfiter_l->equal(cfiter_u)) {
    void *val;
    cfiter_l->getval(&val);
    CFRow *cfrow = (CFRow *) val;
    if (cfrow->end_time > end_time) {
      count++;
    }
    cfiter_l->next();
  };

  return count;
}
