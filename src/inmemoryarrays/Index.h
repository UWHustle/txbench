#ifndef TXBENCH_SRC_INMEMORYARRAYS_INDEX_H_
#define TXBENCH_SRC_INMEMORYARRAYS_INDEX_H_

class IndexIterator {
 public:
  IndexIterator() {};

  virtual bool next() = 0;

  virtual void getval(void** val) = 0;

  virtual bool equal(IndexIterator* iter2) = 0;
};

class Index {
 public:
  Index(int num_rows) {};

  ~Index() {};

  virtual bool insert(int key, void* value, int rowid) = 0;

  virtual bool search(int key, void **value, void* heapbase, int rowsize_bytes) = 0;

  virtual IndexIterator* lower_bound(int key) = 0;

  virtual IndexIterator* upper_bound(int key) = 0;

  virtual bool remove(int key) = 0;

  virtual int count_range(int s_id, int sf_type, int start_time, int end_time) = 0;
};



#endif //TXBENCH_SRC_INMEMORYARRAYS_INDEX_H_
