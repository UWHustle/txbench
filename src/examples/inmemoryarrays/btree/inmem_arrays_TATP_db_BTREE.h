#ifndef TXBENCH_SRC_EXAMPLES_INMEMORYARRAYS_INMEMARRAYSTATPDB_BTREE_H_
#define TXBENCH_SRC_EXAMPLES_INMEMORYARRAYS_INMEMARRAYSTATPDB_BTREE_H_

#include <vector>
#include <memory>
#include <string>
#include <array>
#include <shared_mutex>

#include "alex_lib/alex.h"

class BtreeTATPDB {
 public:

  BtreeTATPDB() {}

  ~BtreeTATPDB() {
    delete[] s_heap_;
    delete[] ai_heap_;
    delete[] sf_heap_;
    delete[] cf_heap_;
//    delete[] s_row_mtxs_;
  }

  void init(int num_rows) {
    s_heap_ = new SRow[num_rows];
    ai_heap_ = new AIRow[num_rows * 4];
    sf_heap_ = new SFRow[num_rows * 12];
    cf_heap_ = new CFRow[num_rows * 4];
//    s_row_mtxs_ = new std::shared_mutex[num_rows];
  }


  void print_stats();

  void new_subscriber_row(int s_id, std::string sub_nbr,
                          std::array<bool, 10> bit, std::array<int, 10> hex,
                          std::array<int, 10> byte2, int msc_location,
                          int vlr_location);

  void new_access_info_row(int s_id, int ai_type, int data1, int data2,
                           std::string data3, std::string data4);

  void new_special_facility_row(int s_id, int sf_type, bool is_active,
                                int error_cntrl, int data_a,
                                std::string data_b);

  void new_call_forwarding_row(int s_id, int sf_type, int start_time,
                               int end_time, std::string numberx);


  //////////////////////

  void get_subscriber_data(int s_id, std::string *sub_nbr,
                           std::array<bool, 10> &bit, std::array<int, 10> &hex,
                           std::array<int, 10> &byte2, int *msc_location,
                           int *vlr_location);

  void get_new_destination(int s_id, int sf_type, int start_time, int end_time,
                           std::vector<std::string> *numberx);

  void get_access_data(int s_id, int ai_type, int *data1, int *data2,
                       std::string *data3, std::string *data4);
  void update_subscriber_data(int s_id, bool bit_1, int sf_type,
                              int data_a);

  void update_location(const std::string &sub_nbr, int vlr_location);

  void insert_call_forwarding(std::string sub_nbr, int sf_type, int start_time,
                              int end_time, std::string numberx);

  void delete_call_forwarding(const std::string &sub_nbr, int sf_type,
                              int start_time);
 private:
  std::vector<SRow> s_;
  std::vector<AIRow> ai_;
  std::vector<SFRow> sf_;
  std::vector<CFRow> cf_;

  alex::Btree<int,
             int,
             alex::BtreeCompare,
             std::allocator<std::pair<int, int>>, false>
      s_alex_;
  alex::Btree<int,
             int,
             alex::BtreeCompare,
             std::allocator<std::pair<int, int>>, false>
      ai_alex_;
  alex::Btree<int,
             int,
             alex::BtreeCompare,
             std::allocator<std::pair<int, int>>, false>
      sf_alex_;
  alex::Btree<int,
             int,
             alex::BtreeCompare,
             std::allocator<std::pair<int, int>>, false>
      cf_alex_;

  std::shared_mutex s_mutex_;
  std::shared_mutex ai_mutex_;
  std::shared_mutex sf_mutex_;
  std::shared_mutex cf_mutex_;

  SRow *s_heap_ = nullptr;
  AIRow *ai_heap_ = nullptr;
  SFRow *sf_heap_ = nullptr;
  CFRow *cf_heap_ = nullptr;

  int s_heap_index_ = 0;
  int ai_heap_index_ = 0;
  int sf_heap_index_ = 0;
  int cf_heap_index_ = 0;

//  std::shared_mutex* s_row_mtxs_;
  int mtx_id = 0;

  std::atomic_uint32_t tx1_succ = 0, tx2_succ = 0, tx3_succ = 0, tx4_succ = 0,
      tx5_succ = 0, tx6_succ = 0, tx7_succ = 0;
  std::atomic_uint32_t tx1_total = 0, tx2_total = 0, tx3_total = 0,
      tx4_total = 0, tx5_total = 0, tx6_total = 0,
      tx7_total = 0;
};

#endif //TXBENCH_SRC_EXAMPLES_INMEMORYARRAYS_INMEMARRAYSTATPDB_BTREE_H_
