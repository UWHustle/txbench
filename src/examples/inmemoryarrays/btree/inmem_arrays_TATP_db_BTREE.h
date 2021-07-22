#ifndef TXBENCH_SRC_EXAMPLES_INMEMORYARRAYS_INMEMARRAYSTATPDB_BTREE_H_
#define TXBENCH_SRC_EXAMPLES_INMEMORYARRAYS_INMEMARRAYSTATPDB_BTREE_H_

#include <vector>
#include <memory>
#include <string>
#include <array>
#include <shared_mutex>

extern "C" {
#include "threadskv10.h"
}

class BtreeTATPDB {
 public:

  BtreeTATPDB() {}

  ~BtreeTATPDB() {
    delete[] s_heap_;
    delete[] ai_heap_;
    delete[] sf_heap_;
    delete[] cf_heap_;
    //bt_mgrclose(btmgr);
  }

  void init(int num_rows) {
    s_heap_ = new SRow[num_rows];
    ai_heap_ = new AIRow[num_rows * 4];
    sf_heap_ = new SFRow[num_rows * 12];
    cf_heap_ = new CFRow[num_rows * 4];

btmgrs1 = bt_mgr("bufmgrs1", 16, 4, 500);
btmgrs2 = bt_mgr("bufmgrs2", 16, 4, 500);
btmgrai1 = bt_mgr("bufmgrai1", 16, 4, 500);
btmgrai2 = bt_mgr("bufmgrai2", 16, 4, 500);
btmgrsf1 = bt_mgr("bufmgrsf1", 16, 4, 500);
btmgrsf2 = bt_mgr("bufmgrsf2", 16, 4, 500);
btmgrcf1 = bt_mgr("bufmgrcf1", 16, 4, 500);
btmgrcf2 = bt_mgr("bufmgrcf2", 16, 4, 500);

bts  = bt_open(btmgrs1, btmgrs2);
btai = bt_open(btmgrai1, btmgrai2);
btsf = bt_open(btmgrsf1, btmgrsf2);
btcf = bt_open(btmgrcf1, btmgrcf2);

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

/*  alex::Btree<int,
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
*/

BtMgr* btmgrs1;
BtMgr* btmgrs2;
BtMgr* btmgrai1;
BtMgr* btmgrai2;
BtMgr* btmgrsf1;
BtMgr* btmgrsf2;
BtMgr* btmgrcf1;
BtMgr* btmgrcf2;

BtDb* bts;
BtDb* btai;
BtDb* btsf;
BtDb* btcf;

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
