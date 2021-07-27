#ifndef TXBENCH_SRC_EXAMPLES_INMEMORYARRAYS_INMEMARRAYSTATPDBPALM_H_
#define TXBENCH_SRC_EXAMPLES_INMEMORYARRAYS_INMEMARRAYSTATPDBPALM_H_

#include <vector>
#include <memory>
#include <string>
#include <array>

#include "palm_lib/palmtree.h"

class PalmTATPDB {
 public:

  PalmTATPDB() :
      s_palm_l_(std::numeric_limits<int>::min(), 1),
      ai_palm_l_(std::numeric_limits<int>::min(), 1),
      sf_palm_l_(std::numeric_limits<int>::min(), 1),
      cf_palm_l_(std::numeric_limits<int>::min(), 1) {
    s_palm_ = &s_palm_l_;
    ai_palm_ = &ai_palm_l_;
    sf_palm_ = &sf_palm_l_;
    cf_palm_ = &cf_palm_l_;
  }

  void print_stats();

  ~PalmTATPDB() {
    delete s_heap_;
    delete ai_heap_;
    delete sf_heap_;
    delete cf_heap_;
  }

  void done_loading() {
    s_palm_->wait_finish();
    s_palm_->reset_metric();
    ai_palm_->wait_finish();
    ai_palm_->reset_metric();
    sf_palm_->wait_finish();
    sf_palm_->reset_metric();
    cf_palm_->wait_finish();
    cf_palm_->reset_metric();

  }

  void init(int num_rows) {
    s_heap_ = new SRow[num_rows];
    ai_heap_ = new AIRow[num_rows * 4];
    sf_heap_ = new SFRow[num_rows * 12];
    cf_heap_ = new CFRow[num_rows * 4];
  }

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
  palmtree::PalmTree<int, int> s_palm_l_;
  palmtree::PalmTree<int, int> ai_palm_l_;
  palmtree::PalmTree<int, int> sf_palm_l_;
  palmtree::PalmTree<int, int> cf_palm_l_;

  palmtree::PalmTree<int, int> *s_palm_;
  palmtree::PalmTree<int, int> *ai_palm_;
  palmtree::PalmTree<int, int> *sf_palm_;
  palmtree::PalmTree<int, int> *cf_palm_;

  SRow *s_heap_ = nullptr;
  AIRow *ai_heap_ = nullptr;
  SFRow *sf_heap_ = nullptr;
  CFRow *cf_heap_ = nullptr;
  int s_heap_index_ = 0;
  int ai_heap_index_ = 0;
  int sf_heap_index_ = 0;
  int cf_heap_index_ = 0;

  std::atomic_uint32_t tx1_succ = 0, tx2_succ = 0, tx3_succ = 0, tx4_succ = 0,
      tx5_succ = 0, tx6_succ = 0, tx7_succ = 0;
  std::atomic_uint32_t tx1_total = 0, tx2_total = 0, tx3_total = 0,
      tx4_total = 0, tx5_total = 0, tx6_total = 0,
      tx7_total = 0;

};

#endif //TXBENCH_SRC_EXAMPLES_INMEMORYARRAYS_INMEMARRAYSTATPDBPALM_H_
