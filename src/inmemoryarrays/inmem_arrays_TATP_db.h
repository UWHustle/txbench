#ifndef TXBENCH_SRC_EXAMPLES_INMEMORYARRAYS_INMEMARRAYSTATPDB_H_
#define TXBENCH_SRC_EXAMPLES_INMEMORYARRAYS_INMEMARRAYSTATPDB_H_

#include "benchmarks/tatp/tatp_benchmark.h"
#include "Index.h"

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <array>
#include<shared_mutex>

struct SRow {
// public:
  int s_id;
  std::string sub_nbr;
  std::array<bool, 10> bit;
  std::array<int, 10> hex;
  std::array<int, 10> byte2;
  int msc_location;
  int vlr_location;
  int mtx_array_id;

  SRow(int s_id,
       const std::string &sub_nbr,
       const std::array<bool, 10> &bit,
       const std::array<int, 10> &hex,
       const std::array<int, 10> &byte_2,
       int msc_location,
       int vlr_location);

  SRow(int s_id,
       const std::string &sub_nbr,
       const std::array<bool, 10> &bit,
       const std::array<int, 10> &hex,
       const std::array<int, 10> &byte_2,
       int msc_location,
       int vlr_location,
       int mtx_row_id);

  SRow() {}

//  friend std::ostream& operator<<(std::ostream &os, const SRow& n)
//  {
//    os << "SROW - " << n.s_id;
//    return os;
//  }

};

struct AIRow {
  int s_id;
  int ai_type;
  int data1;
  int data2;
  std::string data3;
  std::string data4;
  AIRow(int s_id,
        int ai_type,
        int data_1,
        int data_2,
        const std::string &data_3,
        const std::string &data_4);

  AIRow() {}

//  friend std::ostream& operator<<(std::ostream &os, const AIRow& n)
//  {
//    os << "AIRow - " << n.s_id << ", " << n.ai_type;
//    return os;
//  }
};

struct SFRow {
  int s_id;
  int sf_type;
  bool is_active;
  int error_cntrl;
  int data_a;
  std::string data_b;
  SFRow(int s_id,
        int sf_type,
        bool is_active,
        int error_cntrl,
        int data_a,
        const std::string &data_b);

  SFRow() {}


//  friend std::ostream& operator<<(std::ostream &os, const SFRow& n)
//  {
//    os << "SFRow - " << n.s_id << ", " << n.sf_type;
//    return os;
//  }
};

struct CFRow {
  int s_id;
  int sf_type;
  int start_time;
  int end_time;
  std::string numberx;
  CFRow(int s_id,
        int sf_type,
        int start_time,
        int end_time,
        const std::string &numberx);

  CFRow() {}

//  friend std::ostream& operator<<(std::ostream &os, const CFRow& n)
//  {
//    os << "CFRow - " << n.s_id << ", " << n.sf_type << ", " << n.start_time;
//    return os;
//  }

};

struct Tx1Res {
  int s_id;
  std::string sub_nbr;
  std::array<bool, 10> bit;
  std::array<int, 10> hex;
  std::array<int, 10> byte2;
  int msc_location;
  int vlr_location;
};

struct Tx2Res {
  std::vector<std::string> numberx;
};

struct Tx3Res {
  std::vector<int> data1;
  std::vector<int> data2;
  std::vector<std::string> data3;
  std::vector<std::string> data4;
};

int to_ai_compound_key(int s_id, int ai_type);

int ai_compound_key_to_s_id(int ai_key);

int ai_compound_key_to_ai_type(int ai_key);

int to_sf_compound_key(int s_id, int sf_type);

int sf_compound_key_to_s_id(int sf_key);

int sf_compound_key_to_sf_type(int sf_key);

int to_cf_compound_key(int s_id, int sf_type, int start_time);

int cf_compound_key_to_s_id(int cf_key);

int cf_compound_key_to_sf_type(int cf_key);

int cf_compound_key_to_start_time(int cf_key);

template<typename IndexType>
class InMemArraysTATPDB {
 public:

  InMemArraysTATPDB() {}

  ~InMemArraysTATPDB() {
    delete[] s_heap_;
    delete[] ai_heap_;
    delete[] sf_heap_;
    delete[] cf_heap_;
  }

  void init(int num_rows) {
    num_rows_ = num_rows + 100;  // buffer;
    s_heap_ = new SRow[num_rows_];
    ai_heap_ = new AIRow[num_rows_ * 4];
    sf_heap_ = new SFRow[num_rows_ * 12];
    cf_heap_ = new CFRow[num_rows_ * 12];
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

  IndexType s_index_;
  IndexType ai_index_;
  IndexType sf_index_;
  IndexType cf_index_;

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

  int mtx_id = 0;

  int num_rows_;

  std::atomic_uint32_t tx1_succ = 0, tx2_succ = 0, tx3_succ = 0, tx4_succ = 0,
      tx5_succ = 0, tx6_succ = 0, tx7_succ = 0;
  std::atomic_uint32_t tx1_total = 0, tx2_total = 0, tx3_total = 0,
      tx4_total = 0, tx5_total = 0, tx6_total = 0,
      tx7_total = 0;

};

template<typename IndexType>
class InMemArraysTATPConnection : public TATPConnection {
 public:
  InMemArraysTATPConnection(std::shared_ptr<InMemArraysTATPDB<IndexType>> db)
      : db_(db) {}

  void new_subscriber_row(int s_id, std::string sub_nbr,
                          std::array<bool, 10> bit, std::array<int, 10> hex,
                          std::array<int, 10> byte2, int msc_location,
                          int vlr_location) override {
    return db_->new_subscriber_row(s_id, std::move(sub_nbr), bit, hex, byte2,
                                   msc_location, vlr_location);
  }

  void new_access_info_row(int s_id, int ai_type, int data1, int data2,
                           std::string data3, std::string data4) override {
    return db_->new_access_info_row(s_id, ai_type, data1, data2,
                                    std::move(data3), std::move(data4));
  }

  void new_special_facility_row(int s_id, int sf_type, bool is_active,
                                int error_cntrl, int data_a,
                                std::string data_b) override {
    return db_->new_special_facility_row(s_id, sf_type, is_active, error_cntrl,
                                         data_a, std::move(data_b));
  }

  void new_call_forwarding_row(int s_id, int sf_type, int start_time,
                               int end_time, std::string numberx) override {
    return db_->new_call_forwarding_row(s_id, sf_type, start_time, end_time,
                                        std::move(numberx));
  }

  void get_subscriber_data(int s_id, std::string *sub_nbr,
                           std::array<bool, 10> &bit, std::array<int, 10> &hex,
                           std::array<int, 10> &byte2, int *msc_location,
                           int *vlr_location) override {
    return db_->get_subscriber_data(s_id, sub_nbr, bit, hex, byte2,
                                    msc_location, vlr_location);
  }

  void get_new_destination(int s_id, int sf_type, int start_time, int end_time,
                           std::vector<std::string> *numberx) override {
    return db_->get_new_destination(s_id, sf_type, start_time, end_time,
                                    numberx);
  }

  void get_access_data(int s_id, int ai_type, int *data1, int *data2,
                       std::string *data3, std::string *data4) override {
    return db_->get_access_data(s_id, ai_type, data1, data2, data3, data4);
  }

  void update_subscriber_data(int s_id, bool bit_1, int sf_type,
                              int data_a) override {
    return db_->update_subscriber_data(s_id, bit_1, sf_type, data_a);
  }

  void update_location(const std::string &sub_nbr, int vlr_location) override {
    return db_->update_location(sub_nbr, vlr_location);
  }

  void insert_call_forwarding(std::string sub_nbr, int sf_type, int start_time,
                              int end_time, std::string numberx) override {
    return db_->insert_call_forwarding(sub_nbr, sf_type, start_time,
                                       end_time, numberx);
  }

  void delete_call_forwarding(const std::string &sub_nbr, int sf_type,
                              int start_time) override {
    return db_->delete_call_forwarding(sub_nbr, sf_type, start_time);
  }

 private:
  std::shared_ptr<InMemArraysTATPDB<IndexType>> db_;
};

template<typename IndexType>
class InMemArraysTATPServer : public TATPServer {
 public:
  InMemArraysTATPServer() :
      db_(std::make_shared<InMemArraysTATPDB<IndexType>>()) {}

  std::unique_ptr<TATPConnection> connect() override {
    return std::make_unique<InMemArraysTATPConnection<IndexType>>(db_);
  }

  void print_db_stats() {
    db_->print_stats();
  }

  std::shared_ptr<InMemArraysTATPDB<IndexType>> db_;
};

template<typename IndexType>
void InMemArraysTATPDB<IndexType>::print_stats() {
  std::cout << "Table sizes" << std::endl;
  std::cout << "\tS: " << s_heap_index_ << std::endl;
  std::cout << "\tAI: " << ai_heap_index_ << std::endl;
  std::cout << "\tSF: " << sf_heap_index_ << std::endl;
  std::cout << "\tCF: " << cf_heap_index_ << std::endl;
  std::cout << "Success rates" << std::endl;
  std::cout << "\tTx1: " << ((double) tx1_succ / tx1_total) * 100
            << "% (expected 100%, executed "
            << tx1_total << ", successful " << tx1_succ << ")" << std::endl;
  std::cout << "\tTx2: " << ((double) tx2_succ / tx2_total) * 100
            << "% (expected 23.9%, executed "
            << tx2_total << ", successful " << tx2_succ << ")" << std::endl;
  std::cout << "\tTx3: " << ((double) tx3_succ / tx3_total) * 100
            << "% (expected 62.5%, executed "
            << tx3_total << ", successful " << tx3_succ << ")" << std::endl;
  std::cout << "\tTx4: " << ((double) tx4_succ / tx4_total) * 100
            << "% (expected 62.5%, executed "
            << tx4_total << ", successful " << tx4_succ << ")" << std::endl;
  std::cout << "\tTx5: " << ((double) tx5_succ / tx5_total) * 100
            << "% (expected 100%, executed "
            << tx5_total << ", successful " << tx5_succ << ")" << std::endl;
  std::cout << "\tTx6: " << ((double) tx6_succ / tx6_total) * 100
            << "% (expected 31.25%, executed "
            << tx6_total << ", successful " << tx6_succ << ")" << std::endl;
  std::cout << "\tTx7: " << ((double) tx7_succ / tx7_total) * 100
            << "% (expected 31.25%, executed "
            << tx7_total << ", successful " << tx7_succ << ")" << std::endl;
}

template<typename IndexType>
void InMemArraysTATPDB<IndexType>::new_subscriber_row(int s_id,
                                                      std::string sub_nbr,
                                                      std::array<bool, 10> bit,
                                                      std::array<int, 10> hex,
                                                      std::array<int, 10> byte2,
                                                      int msc_location,
                                                      int vlr_location) {
  s_heap_[s_heap_index_] = {s_id, sub_nbr, bit, hex, byte2,
                            msc_location, vlr_location,
                            mtx_id++};

  s_index_.insert(s_id, s_heap_ + s_heap_index_, s_heap_index_);
  s_heap_index_++;
}

template<typename IndexType>
void InMemArraysTATPDB<IndexType>::new_access_info_row(int s_id,
                                                       int ai_type,
                                                       int data1,
                                                       int data2,
                                                       std::string data3,
                                                       std::string data4) {
  ai_heap_[ai_heap_index_] = {s_id, ai_type, data1, data2,
                              std::move(data3), std::move(data4)};
  ai_index_.insert(to_ai_compound_key(s_id, ai_type),
                   ai_heap_ + ai_heap_index_, ai_heap_index_);
  ai_heap_index_++;

}

template<typename IndexType>
void InMemArraysTATPDB<IndexType>::new_special_facility_row(int s_id,
                                                            int sf_type,
                                                            bool is_active,
                                                            int error_cntrl,
                                                            int data_a,
                                                            std::string data_b) {
  sf_heap_[sf_heap_index_] = {s_id, sf_type, is_active, error_cntrl,
                              data_a, std::move(data_b)};

  sf_index_.insert(to_sf_compound_key(s_id, sf_type),
                   sf_heap_ + sf_heap_index_, sf_heap_index_);
  sf_heap_index_++;
}

template<typename IndexType>
void InMemArraysTATPDB<IndexType>::new_call_forwarding_row(int s_id,
                                                           int sf_type,
                                                           int start_time,
                                                           int end_time,
                                                           std::string numberx) {
  cf_heap_[cf_heap_index_] =
      {s_id, sf_type, start_time, end_time, std::move(numberx)};

  cf_index_.insert(to_cf_compound_key(s_id, sf_type, start_time),
                   cf_heap_ + cf_heap_index_, cf_heap_index_);
  cf_heap_index_++;
}

template<typename IndexType>
void InMemArraysTATPDB<IndexType>::get_subscriber_data(int s_id,
                                                       std::string *sub_nbr,
                                                       std::array<bool,
                                                                  10> &bit,
                                                       std::array<int, 10> &hex,
                                                       std::array<int,
                                                                  10> &byte2,
                                                       int *msc_location,
                                                       int *vlr_location) {
  tx1_total++;

  std::cout << "TX1" << std::endl;

  //////
  std::shared_lock lock(s_mutex_);
  //////

  void *out;
  if (s_index_.search(s_id, &out, (void*) s_heap_, sizeof(SRow)) == false) {
    std::cout << "TX1 END" << std::endl;
    return; }

  SRow *srow = (SRow *) out;

  sub_nbr->assign(srow->sub_nbr);
  bit = srow->bit;
  hex = srow->hex;
  byte2 = srow->byte2;
  *msc_location = srow->msc_location;
  *vlr_location = srow->vlr_location;
  tx1_succ++;

  std::cout << "TX1 END" << std::endl;

  return;

}

// TODO(chronis) debug this
template<typename IndexType>
void InMemArraysTATPDB<IndexType>::get_new_destination(int s_id,
                                                       int sf_type,
                                                       int start_time,
                                                       int end_time,
                                                       std::vector<std::string> *numberxs) {
// SELECT cf.numberx FROM Special_Facility AS sf, Call_Forwarding AS cf
// WHERE (sf.s_id = <s_id rnd> AND sf.sf_type = <sf_type rnd>
//        AND sf.is_active = 1)
//  AND (cf.s_id = sf.s_id AND cf.sf_type = sf.sf_type)
//  AND (cf.start_time \<= <start_time rnd>
//       AND <end_time rnd> \< cf.end_time);

  tx2_total++;

  std::cout << "TX2" << std::endl;

  /////
  std::shared_lock lock(sf_mutex_);
  std::shared_lock lock1(cf_mutex_);
  /////

  void *out = nullptr;
  if (sf_index_.search(to_sf_compound_key(s_id, sf_type), &out, (void*) sf_heap_, sizeof(SFRow)) == false) {
    std::cout << "TX2 END" << std::endl;
    return;
  }

  SFRow *sfrow = (SFRow *) out;
  if (sfrow->is_active != 1) {
    std::cout << "TX2 END" << std::endl;
    return;
  }

  int count = cf_index_.count_range(s_id, sf_type, start_time, end_time);

  if (count > 0) {
    tx2_succ++;
  }

  std::cout << "TX2 END" << std::endl;
  return;
}

template<typename IndexType>
void InMemArraysTATPDB<IndexType>::get_access_data(int s_id,
                                                   int ai_type,
                                                   int *data1,
                                                   int *data2,
                                                   std::string *data3,
                                                   std::string *data4) {
  // SELECT data1, data2, data3, data4 FROM Access_Info
  // WHERE s_id = <s_id rnd> AND ai_type = <ai_type rnd>;
  // (s_id, ai_type) is a composite primary key.
  tx3_total++;

  std::cout << "TX3" << std::endl;

  ////
  std::shared_lock lock(ai_mutex_);
  ///

  void *out;
  if (ai_index_.search(to_ai_compound_key(s_id, ai_type), &out, (void*) ai_heap_, sizeof(AIRow))
      == false) {
    std::cout << "TX3 END" << std::endl;
    return;
  }

  AIRow *airow = (AIRow *) out;

  *data1 = airow->data1;
  *data2 = airow->data2;
  data3->assign(airow->data3);
  data4->assign(airow->data4);
  tx3_succ++;

  std::cout << "TX3 END" << std::endl;
  return;
}

template<typename IndexType>
void InMemArraysTATPDB<IndexType>::update_subscriber_data(int s_id,
                                                          bool bit_1,
                                                          int sf_type,
                                                          int data_a) {

  tx4_total++;

  std::cout << "TX4" << std::endl;

  /////////
  std::unique_lock lock(s_mutex_);
  /////////

  // UPDATE Subscriber SET bit_1 = <bit_rnd>
  // WHERE s_id = <s_id rnd subid>;

  void *out;
  if (s_index_.search(s_id, &out, (void*) s_heap_, sizeof(SRow)) == false) {
    std::cout << "TX4 END" << std::endl;
    return;
  }

  SRow *srow = (SRow *) out;

  std::unique_lock lock1(sf_mutex_);

  // UPDATE Special_Facility SET data_a = <data_a rnd>
  // WHERE s_id = <s_id value subid> AND sf_type = <sf_type rnd>;
  void *out2;
  if (sf_index_.search(to_sf_compound_key(s_id, sf_type), &out2, (void*) sf_heap_, sizeof(SFRow))
      == false) {
    std::cout << "TX4 END" << std::endl;
    return;
  }

  SFRow *sf_rowid = (SFRow *) out2;

  // Perform both updates at the same time, instead of rolling back
  srow->bit[0] = bit_1;
  sf_rowid->data_a = data_a;
  tx4_succ++;

  std::cout << "TX4 END" << std::endl;
  return;
}

template<typename IndexType>
void InMemArraysTATPDB<IndexType>::update_location(const std::string &sub_nbr,
                                                   int vlr_location) {

  tx5_total++;

  std::cout << "TX5" << std::endl;

  ////////////
  std::unique_lock lock(s_mutex_);
  ///////////

  // UPDATE Subscriber SET vlr_location = <vlr_location rnd>
  // WHERE sub_nbr = <sub_nbr rndstr>;

  int s_id = std::stoi(sub_nbr);

  void *out;
  if (s_index_.search(s_id, &out, (void*) s_heap_, sizeof(SRow)) == false) {
    std::cout << "TX5 END" << std::endl;
    return;
  }

  SRow *srow = (SRow *) out;
  srow->vlr_location = vlr_location;
  tx5_succ++;

  std::cout << "TX5 END" << std::endl;
  return;
}

template<typename IndexType>
void InMemArraysTATPDB<IndexType>::insert_call_forwarding(std::string sub_nbr,
                                                          int sf_type,
                                                          int start_time,
                                                          int end_time,
                                                          std::string numberx) {
  tx6_total++;

  std::cout << "TX6" << std::endl;

  //////
  std::shared_lock lock(s_mutex_);
  std::shared_lock lock1(sf_mutex_);
  std::unique_lock lock2(cf_mutex_);
  //////

  // SELECT <s_id bind subid s_id> FROM Subscriber
  // WHERE sub_nbr = <sub_nbr rndstr>;

  int s_id = std::stoi(sub_nbr);
  void *out;
  if (s_index_.search(s_id, &out, (void*) s_heap_, sizeof(SRow)) == false) {
    std::cout << "TX6" << std::endl;
    return;
  }

  SRow *srow = (SRow *) out;

  // SELECT <sf_type bind sfid sf_type>
  // FROM Special_Facility
  // WHERE s_id = <s_id value subid>;

  void *out2;
  if (sf_index_.search(to_sf_compound_key(s_id, sf_type), &out2, (void*) sf_heap_, sizeof(SFRow))
      == false) {
    std::cout << "TX6 END" << std::endl;
    return;
  }
  SFRow *sfrow = (SFRow *) out2;

  // INSERT INTO Call_Forwarding
  // VALUES (<s_id value subid>, <sf_type rnd sf_type>,
  // <start_time rnd>, <end_time rnd>, <numberx rndstr>);

  void *out3;
  if (cf_index_.search(to_cf_compound_key(s_id, sf_type, start_time), &out3, (void*) cf_heap_, sizeof(CFRow))
      == false) {
    std::cout << "TX6 END" << std::endl;
    return;
  }

  cf_heap_[cf_heap_index_] =
      {s_id, sf_type, start_time, end_time, std::move(numberx)};

  cf_index_.insert(to_cf_compound_key(s_id, sf_type, start_time),
                   cf_heap_ + cf_heap_index_, cf_heap_index_);

  cf_heap_index_++;

  tx6_succ++;

  std::cout << "TX6 END" << std::endl;
  return;
}

template<typename IndexType>
void InMemArraysTATPDB<IndexType>::delete_call_forwarding(const std::string &sub_nbr,
                                                          int sf_type,
                                                          int start_time) {

  tx7_total++;

  std::cout << "TX7" << std::endl;

  //////
  std::shared_lock lock(s_mutex_);
  std::unique_lock lock1(cf_mutex_);
  //////

  // SELECT <s_id bind subid s_id> FROM Subscriber
  // WHERE sub_nbr = <sub_nbr rndstr>;
  int s_id = std::stoi(sub_nbr);
  void *out;
  if (s_index_.search(s_id, &out, (void*) s_heap_, sizeof(SRow)) == false) {
    std::cout << "TX7 END" << std::endl;
    return;
  }

  SRow *srow = (SRow *) out;

  // DELETE FROM Call_Forwarding
  // WHERE s_id = <s_id value subid> AND sf_type = <sf_type rnd>
  // AND start_time = <start_time rnd>;

  if (cf_index_.remove(to_cf_compound_key(s_id, sf_type, start_time))) {
    tx7_succ++;
  }

  std::cout << "TX7 END" << std::endl;
  return;
}

#endif //TXBENCH_SRC_EXAMPLES_INMEMORYARRAYS_INMEMARRAYSTATPDB_H_
