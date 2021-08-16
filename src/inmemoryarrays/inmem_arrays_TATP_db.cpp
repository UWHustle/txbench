#include "inmem_arrays_TATP_db.h"

#include <iostream>

SFRow::SFRow(int s_id,
             int sf_type,
             bool is_active,
             int error_cntrl,
             int data_a,
             const std::string &data_b)
    : s_id(s_id),
      sf_type(sf_type),
      is_active(is_active),
      error_cntrl(error_cntrl),
      data_a(data_a),
      data_b(data_b) {}

AIRow::AIRow(int s_id,
             int ai_type,
             int data_1,
             int data_2,
             const std::string &data_3,
             const std::string &data_4)
    : s_id(s_id),
      ai_type(ai_type),
      data1(data_1),
      data2(data_2),
      data3(data_3),
      data4(data_4) {}

SRow::SRow(int s_id,
           const std::string &sub_nbr,
           const std::array<bool, 10> &bit,
           const std::array<int, 10> &hex,
           const std::array<int, 10> &byte_2,
           int msc_location,
           int vlr_location)
    : s_id(s_id),
      sub_nbr(sub_nbr),
      bit(bit),
      hex(hex),
      byte2(byte_2),
      msc_location(msc_location),
      vlr_location(vlr_location),
      mtx_array_id(-1) {}

SRow::SRow(int s_id,
           const std::string &sub_nbr,
           const std::array<bool, 10> &bit,
           const std::array<int, 10> &hex,
           const std::array<int, 10> &byte_2,
           int msc_location,
           int vlr_location,
           int mtx_array_id)
    : s_id(s_id),
      sub_nbr(sub_nbr),
      bit(bit),
      hex(hex),
      byte2(byte_2),
      msc_location(msc_location),
      vlr_location(vlr_location),
      mtx_array_id(mtx_array_id) {}

CFRow::CFRow(int s_id,
             int sf_type,
             int start_time,
             int end_time,
             const std::string &numberx)
    : s_id(s_id),
      sf_type(sf_type),
      start_time(start_time),
      end_time(end_time),
      numberx(numberx) {}


int to_ai_compound_key(int s_id, int ai_type) {
  return s_id * 10 + ai_type;
}

int ai_compound_key_to_s_id(int ai_key) {
  return ai_key / 10;
}

int ai_compound_key_to_ai_type(int ai_key) {
  return ai_key % 10;
}

int to_sf_compound_key(int s_id, int sf_type) {
  return s_id * 10 + sf_type;
}

int sf_compound_key_to_s_id(int sf_key) {
  return sf_key / 10;
}

int sf_compound_key_to_sf_type(int sf_key) {
  return sf_key % 10;
}

int to_cf_compound_key(int s_id, int sf_type, int start_time) {

  return (s_id * 100) + (sf_type * 10) + (start_time / 8);
}


int cf_compound_key_to_s_id(int cf_key) {
  return cf_key / 100;
}

int cf_compound_key_to_sf_type(int cf_key) {
  return (cf_key % 100) / 10;
}

int cf_compound_key_to_start_time(int cf_key) {
  return (cf_key % 10) * 8;
}



