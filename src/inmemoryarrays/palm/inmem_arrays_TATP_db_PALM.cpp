#include "../inmem_arrays_TATP_db.h"
#include "inmem_arrays_TATP_db_PALM.h"

#include <iostream>

void PalmTATPDB::print_stats() {
  std::cout << "Table sizes" << std::endl;
//  std::cout << "\tS: " << s_palm_->  .size() << std::endl;
//  std::cout << "\tAI: " << ai_.size() << std::endl;
//  std::cout << "\tSF: " << sf_.size() << std::endl;
//  std::cout << "\tCF: " << cf_.size() << std::endl;
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

void PalmTATPDB::new_subscriber_row(int s_id,
                                    std::string sub_nbr,
                                    std::array<bool, 10> bit,
                                    std::array<int, 10> hex,
                                    std::array<int, 10> byte2,
                                    int msc_location,
                                    int vlr_location) {

  std::cout << "insert sid: " << s_id << " index: " << s_heap_index_
            << std::endl;

//  s_heap_[s_heap_index_] = {s_id, sub_nbr, bit, hex, byte2,
//                            msc_location, vlr_location};
  s_palm_->insert(s_id, s_id); //s_heap_index_++);

}

void PalmTATPDB::new_access_info_row(int s_id,
                                     int ai_type,
                                     int data1,
                                     int data2,
                                     std::string data3,
                                     std::string data4) {
//  ai_.emplace_back(s_id, ai_type, data1, data2,
//                   std::move(data3), std::move(data4));
//  ai_palm_l_->insert(to_ai_compound_key(s_id, ai_type), ai_heap_index_++);
//

//  ai_heap_[ai_heap_index_] = {s_id, ai_type, data1, data2,
//                              std::move(data3), std::move(data4)};
//  ai_palm_->insert(to_ai_compound_key(s_id, ai_type), ai_heap_index_++);

}

void PalmTATPDB::new_special_facility_row(int s_id,
                                          int sf_type,
                                          bool is_active,
                                          int error_cntrl,
                                          int data_a,
                                          std::string data_b) {
//  sf_.emplace_back(s_id, sf_type, is_active, error_cntrl,
//                   data_a, std::move(data_b));
//
//  sf_palm_->insert(to_sf_compound_key(s_id, sf_type), sf_heap_index_++);
//  sf_heap_[sf_heap_index_] = {s_id, sf_type, is_active, error_cntrl,
//                              data_a, std::move(data_b)};
//  sf_palm_->insert(to_sf_compound_key(s_id, sf_type), sf_heap_index_++);

}

void PalmTATPDB::new_call_forwarding_row(int s_id,
                                         int sf_type,
                                         int start_time,
                                         int end_time,
                                         std::string numberx) {
//  cf_heap_[cf_heap_index_] =
//      {s_id, sf_type, start_time, end_time, std::move(numberx)};
//
//  cf_palm_->insert(to_cf_compound_key(s_id, sf_type, start_time),
//                   cf_heap_index_++);
}

void PalmTATPDB::get_subscriber_data(int s_id,
                                     std::string *sub_nbr,
                                     std::array<bool, 10> &bit,
                                     std::array<int, 10> &hex,
                                     std::array<int, 10> &byte2,
                                     int *msc_location,
                                     int *vlr_location) {
//  std::cout << "TX1 " << s_id << std::endl;
  tx1_total++;
  if (tx1_total > 100) { return; }
  int srow_id = -1;
  bool found = s_palm_->find(5, srow_id);
  if (found) {
    std::cout << "SUCCES: ";
  }
  std::cout << "TX1 sid: " << s_id << " - rowidres: " << srow_id << std::endl;

//  << " - "
//            << s_heap_[srow_id].s_id << " - " << tx1_total
//            << " END" << std::endl;

//  if (found) {
//      sub_nbr->assign(srow.sub_nbr);
//      bit = srow.bit;
//      hex = srow.hex;
//      byte2 = srow.byte2;
//      *msc_location = srow.msc_location;
//      *vlr_location = srow.vlr_location;
//      tx1_succ++;
//      std::cout << "TX1 " << s_id << " - " << tx1_total << " END" << std::endl;
//      return;
//    }

  return;
}

void PalmTATPDB::get_new_destination(int s_id,
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
//  int res = 0;
//  for (int i = 0; i < sf_.size(); ++i) {
//    if (sf_[i].s_id == s_id && sf_[i].sf_type == sf_type
//        && sf_[i].is_active == 1) {
//      res = 1;
//      break;
//    }
//  }
//
//  if (res == 0) {return; }
//
//  for (int i = 0; i < cf_.size(); ++i) {
//    if (cf_[i].s_id == s_id && cf_[i].sf_type == sf_type
//        && cf_[i].start_time <= start_time
//        && cf_[i].end_time > end_time) {
//      res++;
//      numberxs->emplace_back(cf_[i].numberx);
//    }
//  }
//
//  if (res > 1) {
//    tx2_succ++;
//  }

  return;
}

void PalmTATPDB::get_access_data(int s_id,
                                 int ai_type,
                                 int *data1,
                                 int *data2,
                                 std::string *data3,
                                 std::string *data4) {

  // SELECT data1, data2, data3, data4 FROM Access_Info
  // WHERE s_id = <s_id rnd> AND ai_type = <ai_type rnd>;
  // (s_id, ai_type) is a composite primary key.
  tx3_total++;
//
//  for (int i = 0; i < ai_.size(); ++i) {
//    if (ai_[i].s_id == s_id && ai_[i].ai_type == ai_type) {
//      *data1 = ai_[i].data1;
//      *data2 = ai_[i].data2;
//      data3->assign(ai_[i].data3);
//      data4->assign(ai_[i].data4);
//      tx3_succ++;
//      return;
//    }
//  }

  return;
}

void PalmTATPDB::update_subscriber_data(int s_id,
                                        bool bit_1,
                                        int sf_type,
                                        int data_a) {

  tx4_total++;
  // UPDATE Subscriber SET bit_1 = <bit_rnd>
  // WHERE s_id = <s_id rnd subid>;
//  int s_rowid = -1;
//  for (int i = 0; i < s_.size(); ++i) {
//    if (s_[i].s_id == s_id) {
//      s_rowid = i;
//      break;
//    }
//  }
//
//  if (s_rowid == -1) { return; }
//
//  // UPDATE Special_Facility SET data_a = <data_a rnd>
//  // WHERE s_id = <s_id value subid> AND sf_type = <sf_type rnd>;
//  int sf_rowid = -1;
//  for (int i = 0; i < sf_.size(); ++i) {
//    if (sf_[i].s_id == s_id && sf_[i].sf_type == sf_type) {
//      sf_rowid = i;
//      break;
//    }
//  }
//
//  if (sf_rowid == -1) { return; }
//
//  // Perform both updates at the same time, instead of rolling back
//  s_[s_rowid].bit[0] = bit_1;
//  sf_[sf_rowid].data_a = data_a;
//  tx4_succ++;

  return;
}

void PalmTATPDB::update_location(const std::string &sub_nbr,
                                 int vlr_location) {
  tx5_total++;
  // UPDATE Subscriber SET vlr_location = <vlr_location rnd>
  // WHERE sub_nbr = <sub_nbr rndstr>;
//  for (int i = 0; i < s_.size(); ++i) {
//    if (s_[i].sub_nbr.compare(sub_nbr) == 0) {
//      s_[i].vlr_location = vlr_location;
//      tx5_succ++;
//      return;
//    }
//  }

  return;
}

void PalmTATPDB::insert_call_forwarding(std::string sub_nbr,
                                        int sf_type,
                                        int start_time,
                                        int end_time,
                                        std::string numberx) {
  tx6_total++;

//  // SELECT <s_id bind subid s_id> FROM Subscriber
//  // WHERE sub_nbr = <sub_nbr rndstr>;
//  int s_id = -1;
//  for (int i = 0; i < s_.size(); ++i) {
//    if (s_[i].sub_nbr.compare(sub_nbr) == 0) {
//      s_id = s_[i].s_id;
//      break;
//    }
//  }
//
//  if (s_id == -1) { return; }
//
//  // SELECT <sf_type bind sfid sf_type>
//  // FROM Special_Facility
//  // WHERE s_id = <s_id value subid>;
//  bool res = false;
//  for (int i = 0; i < sf_.size(); ++i) {
//    if (sf_[i].s_id == s_id) {
//      res = true;
//      break;
//    }
//  }
//
//  if (res == false) { return; }
//
//  // INSERT INTO Call_Forwarding
//  // VALUES (<s_id value subid>, <sf_type rnd sf_type>,
//  // <start_time rnd>, <end_time rnd>, <numberx rndstr>);
//  for (int i = 0; i < cf_.size(); ++i) {
//    if (cf_[i].s_id == s_id && cf_[i].sf_type == sf_type
//        && cf_[i].start_time == start_time) {
//      return;
//    }
//  }
//
//  cf_.emplace_back(s_id, sf_type, start_time, end_time, std::move(numberx));
//  tx6_succ++;

  return;
}

void PalmTATPDB::delete_call_forwarding(const std::string &sub_nbr,
                                        int sf_type,
                                        int start_time) {
  tx7_total++;

//  // SELECT <s_id bind subid s_id> FROM Subscriber
//  // WHERE sub_nbr = <sub_nbr rndstr>;
//  int s_id = -1;
//  for (int i = 0; i < s_.size(); ++i) {
//    if (s_[i].sub_nbr.compare(sub_nbr)) {
//      s_id = s_[i].s_id;
//      break;
//    }
//  }
//
//  if (s_id == -1) { return; }
//
//  // DELETE FROM Call_Forwarding
//  // WHERE s_id = <s_id value subid> AND sf_type = <sf_type rnd>
//  // AND start_time = <start_time rnd>;
//
//  for (int i = 0; i < cf_.size(); ++i) {
//    if (cf_[i].s_id == s_id && cf_[i].sf_type == sf_type
//        && cf_[i].start_time == start_time) {
//      cf_.erase(cf_.begin() + i);
//      tx7_succ++;
//      break;
//    }
//  }

  return;
}







