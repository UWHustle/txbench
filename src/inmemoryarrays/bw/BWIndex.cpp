#include "BWIndex.h"

#include "../inmem_arrays_TATP_db.h"

bool BWIterator::equal(IndexIterator *iter2) {
  return iter_ == ((BWIterator *) iter2)->iter_;
  return true;
}

bool BWIterator::next() {
//  iter_++;
//  return !iter_.is_end();
  return true;
}

void BWIterator::getval(void **val) {
//  *val = iter_.payload();
}

bool BWIndex::insert(int key, void *value, int rowid) {
  index_->AssignGCID(0);

  index_->Insert(key, value);
  return true;
}

bool BWIndex::search(int key, void **value, void* heapbase, int rowsize_bytes) {
  index_->AssignGCID(0);
  auto out = index_->GetValue(key);
  if (out.size() == 0) {
    return false;
  }
  *value = *(out.begin());
  return true;
}

bool BWIndex::remove(int key) {
  index_->AssignGCID(0);

  auto out = index_->GetValue(key);
  if (out.size() == 0) {
    return false;
  }
  if (index_->Delete(key, *out.begin()) > 0) { return true; }
  return false;
}

IndexIterator *BWIndex::lower_bound(int key) {
//  BWIndex *iter = new BWIndex(index_.lower_bound(key));
  return nullptr;
}

IndexIterator *BWIndex::upper_bound(int key) {
//  BWIndex *iter = new BWIndex(index_.upper_bound(key));
//  return iter;
return nullptr;
}

int BWIndex::count_range(int s_id,
                           int sf_type,
                           int start_time,
                           int end_time) {
  int count = 0;

  BwTreeType::ForwardIterator cfiter_l =
      index_->Begin(to_cf_compound_key(s_id, sf_type, 0));

  BwTreeType::ForwardIterator cfiter_u =
      index_->Begin(to_cf_compound_key(s_id, sf_type, start_time + 1));

  while (!(cfiter_l == cfiter_u)) {
    CFRow *cfrow  = (CFRow *)(*cfiter_l).second;
    if (cfrow->end_time > end_time) {
      count++;
    }
    if (cfrow->start_time > start_time) {
      return count;
    }
    cfiter_l++;
  };

  return count;
}
