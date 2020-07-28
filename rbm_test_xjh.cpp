#include <iostream>
#include "roaring.hh"
#include "roaring.c"
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include "string.h"
#include <sstream>
#include <memory>

using namespace std;

const uint64_t MAX_64 = 18000000000000000000ull;
const string file_src = "/data00/home/xujinghui.950615.sz/data/";
const vector<string> file_name_ip = 
{"ip_data/ip1.csv", 
"ip_data/ip2.csv",
"ip_data/ip3.csv",
"ip_data/ip4.csv",
"ip_data/ip5.csv",
"ip_data/ip6.csv",
"ip_data/ip7.csv",
"ip_data/ip8.csv",
"ip_data/ip9.csv",
"ip_data/ip10.csv",
"ip_data/ip11.csv",
"ip_data/ip12.csv",
"ip_data/ip13.csv",
"ip_data/ip14.csv"};
const vector<string> file_name_white = 
{"white_data/part-00000-0280e5dd-9908-4407-938c-159d32f87ee7-c000",
"white_data/part-00001-0280e5dd-9908-4407-938c-159d32f87ee7-c000",
"white_data/part-00002-0280e5dd-9908-4407-938c-159d32f87ee7-c000",
"white_data/part-00003-0280e5dd-9908-4407-938c-159d32f87ee7-c000",
"white_data/part-00004-0280e5dd-9908-4407-938c-159d32f87ee7-c000",
"white_data/part-00005-0280e5dd-9908-4407-938c-159d32f87ee7-c000",
"white_data/part-00006-0280e5dd-9908-4407-938c-159d32f87ee7-c000",
"white_data/part-00007-0280e5dd-9908-4407-938c-159d32f87ee7-c000",
"white_data/part-00008-0280e5dd-9908-4407-938c-159d32f87ee7-c000",
"white_data/part-00009-0280e5dd-9908-4407-938c-159d32f87ee7-c000"};
const vector<string> file_name_black = {
"black_data/black1.csv",
"black_data/black2.csv",
"black_data/black3.csv",
"black_data/black4.csv",
"black_data/black5.csv",
"black_data/black6.csv",
"black_data/black7.csv"};

void all_test_ip_32(uint64_t search_times, int repeat_times);
void all_test_black_64(uint64_t search_times, int repeat_times);
void all_test_white_64(uint64_t search_times, int repeat_times);
void mem_test_seq_32(uint32_t end);
void mem_test_ran_32(uint32_t end);
uint32_t range_rand(uint32_t max, uint32_t& min, uint32_t rest);
void mem_test_seq_64(uint64_t end);
void mem_test_ran_64(uint64_t end);
uint64_t range_rand_64(uint64_t max, uint64_t& min, uint64_t rest);
void search_test_seq_64(uint64_t end);
void load_file();
void mem_test();
int parse_ip(string &ip);
vector<string> split(const string &s, const string &seperator);

int main() {

  //all_test_ip_32(200000, 1);
  //all_test_black_64(200000, 1);
  //all_test_white_64(200000, 1);
  load_file();

  cout << "Fine" << endl;
}

void all_test_ip_32(uint64_t search_times, int repeat_times){
  Roaring* r1 = new Roaring;
  clock_t t_tmp;
  clock_t search_time_in = 0;
  clock_t search_time_no = 0;
  vector<uint32_t> num_in;

  t_tmp = clock();
  for(int f_i = 0; f_i < file_name_ip.size(); f_i++){
    ifstream data(file_src + file_name_ip[f_i]);
    string ip;
    uint32_t num;
    while(data >> ip){
      num = parse_ip(ip);
      r1->add(num);
      if((num_in.size() < search_times && (rand() % 100 < 3))){
        num_in.push_back(num);
      }
    }
    data.close();
    data.clear();
  }
  clock_t t_insert = clock() - t_tmp;
  t_tmp = clock();
  for(int f_i = 0; f_i < file_name_ip.size(); f_i++){
    ifstream data(file_src + file_name_ip[f_i]);
    string ip;
    uint32_t num;
    while(data >> ip){
      num = parse_ip(ip);
      if((num_in.size() < search_times && (rand() % 100 < 3))){
        num_in.push_back(num);
      }
    }
    data.close();
    data.clear();
  }
  clock_t t_parse_ip = clock() - t_tmp;
  std::cout << "Size proto: " << r1->getSizeInBytes() << std::endl;
  t_tmp = clock();
  bool ret1 = r1->runOptimize();
  clock_t t_runOptimize = clock() - t_tmp;
  std::cout << "Size runOptimize: " << r1->getSizeInBytes() << std::endl;
  t_tmp = clock();
  size_t ret2 = r1->shrinkToFit();
  clock_t t_shrinkToFit = clock() - t_tmp;
  std::cout << "Size shrinkToFit: " << r1->getSizeInBytes() << std::endl;
  std::cout << "run_container exist: " << ret1 << std::endl;
  std::cout << "shrink byte: " << ret2 << std::endl;
  std::cout << "insert time: " << t_insert - t_parse_ip << std::endl;
  std::cout << "runOptimize time: " << t_runOptimize << std::endl;
  std::cout << "shrinkToFit time: " << t_shrinkToFit << std::endl;
  std::cout << "cardinality: " << r1->cardinality() << std::endl;

  //container size
  /*std::map<int, uint64_t> res_type;
  std::vector<int> res_num;
  r1->each_container_type(res_type, res_num);
  cout << "each type container size: " << endl;
  for(auto tmp : res_type){
    cout << tmp.first << " : " << tmp.second << endl;
  }
  cout << "total_containers_num: " << res_num.size() << endl;
  double av_container_size = (double)accumulate(res_num.begin(), res_num.end(), 0) / (double)res_num.size();
  cout << "av_container_size: " << av_container_size << endl;
  double S = 0;
  for (int i = 0; i < res_num.size(); i++){
      S += pow((double)res_num[i] - av_container_size, 2);
  }
  cout << "s_container_size: " << S / (res_num.size() - 1) << endl;
  std::sort(res_num.begin(), res_num.end());
  cout << "0.01 rate distribute: " << endl;
  cout << res_num[0] << ",";
  for(double i = 0.01; i <= 1; i += 0.01){
    cout << res_num[(int)(res_num.size() * i) - 1] << ",";
  }
  cout << endl;*/

  //search time
  for(int i = 0; i < repeat_times; i++){
  //int count = 0;
  //while(1){
    t_tmp = clock();
    for(uint32_t i = 1; i <= search_times; i++){
      r1->contains(num_in[i]);
    }
    search_time_in += (clock() - t_tmp);
  //  count++;
  //  cout << count << endl;
  }

  for(int i = 0; i < repeat_times; i++){
    t_tmp = clock();
    for(uint32_t i = 1; i <= search_times; i++){
      r1->contains(i);
    }
    search_time_no += (clock() - t_tmp);
  }
  std::cout << "search_time_in: " << search_time_in / repeat_times << std::endl;
  std::cout << "search_time_no: " << search_time_no / repeat_times << std::endl;

  std::cout << "sizeof_r1: " << sizeof(*r1) << std::endl;
  delete r1;
}

void all_test_black_64(uint64_t search_times, int repeat_times){
  Roaring64Map* r1 = new Roaring64Map;
  clock_t t_tmp;
  clock_t search_time_in = 0;
  clock_t search_time_no = 0;
  vector<uint64_t> num_in;

  t_tmp = clock();
  for(int f_i = 0; f_i < file_name_black.size(); f_i++){
    ifstream data(file_src + file_name_black[f_i]);
    double num;
    while(data >> num){
      r1->add((uint64_t)num);
      if((num_in.size() < search_times && (rand()%100 < 4))){
        num_in.push_back((uint64_t)num);
      }
    }
    data.close();
    data.clear();
  }
  clock_t t_insert = clock() - t_tmp;
  std::cout << "Size proto: " << r1->getSizeInBytes() << std::endl;
  t_tmp = clock();
  bool ret1 = r1->runOptimize();
  clock_t t_runOptimize = clock() - t_tmp;
  std::cout << "Size runOptimize: " << r1->getSizeInBytes() << std::endl;
  t_tmp = clock();
  size_t ret2 = r1->shrinkToFit();
  clock_t t_shrinkToFit = clock() - t_tmp;
  std::cout << "Size shrinkToFit: " << r1->getSizeInBytes() << std::endl;
  std::cout << "run_container exist: " << ret1 << std::endl;
  std::cout << "shrink byte: " << ret2 << std::endl;
  std::cout << "insert time: " << t_insert << std::endl;
  std::cout << "runOptimize time: " << t_runOptimize << std::endl;
  std::cout << "shrinkToFit time: " << t_shrinkToFit << std::endl;
  std::cout << "cardinality: " << r1->cardinality() << std::endl;

  //container size
  /*std::map<int, uint64_t> res_type;
  std::vector<int> res_num;
  int hb_num;
  r1->each_container_type(res_type, res_num, hb_num);
  cout << "each type container size: " << endl;
  for(auto tmp : res_type){
    cout << tmp.first << " : " << tmp.second << endl;
  }
  cout << "hb_num: " << hb_num << endl;
  cout << "total_containers_num: " << res_num.size() << endl;
  double av_container_size = (double)accumulate(res_num.begin(), res_num.end(), 0) / (double)res_num.size();
  cout << "av_container_size: " << av_container_size << endl;
  double S = 0;
  for (int i = 0; i < res_num.size(); i++){
      S += pow((double)res_num[i] - av_container_size, 2);
  }
  cout << "s_container_size: " << S / (res_num.size() - 1) << endl;
  std::sort(res_num.begin(), res_num.end());
  cout << "0.01 rate distribute: " << endl;
  cout << res_num[0] << ",";
  for(double i = 0.01; i <= 1; i += 0.01){
    cout << res_num[(int)(res_num.size() * i) - 1] << ",";
  }
  cout << endl;*/

  //search time
  for(int i = 0; i < repeat_times; i++){
  //int count = 0;
  //while(1){
    t_tmp = clock();
    for(uint64_t i = 1; i <= search_times; i++){
      r1->contains(num_in[i]);
    }
    search_time_in += (clock() - t_tmp);
  //  count++;
  //  cout << count << endl;
  }

  for(int i = 0; i < repeat_times; i++){
    t_tmp = clock();
    for(uint64_t i = 1; i <= search_times; i++){
      r1->contains(i);
    }
    search_time_no += (clock() - t_tmp);
  }
  std::cout << "search_time_in: " << search_time_in / repeat_times << std::endl;
  std::cout << "search_time_no: " << search_time_no / repeat_times << std::endl;

  std::cout << "sizeof_r1: " << sizeof(*r1) << std::endl;
  delete r1;
}

void all_test_white_64(uint64_t search_times, int repeat_times){
  Roaring64Map* r1 = new Roaring64Map;
  clock_t t_tmp;
  clock_t search_time_in = 0;
  clock_t search_time_no = 0;
  vector<uint64_t> num_in;

  //base test
  t_tmp = clock();
  for(int f_i = 0; f_i < file_name_white.size(); f_i++){
    ifstream data(file_src + file_name_white[f_i]);
    uint64_t num;
    while(data >> num){
      r1->add(num);
      if((num_in.size() < search_times && (rand()%100 < 1))){
        num_in.push_back(num);
      }
    }
  }
  clock_t t_insert = clock() - t_tmp;
  std::cout << "Size proto: " << r1->getSizeInBytes() << std::endl;
  t_tmp = clock();
  bool ret1 = r1->runOptimize();
  clock_t t_runOptimize = clock() - t_tmp;
  std::cout << "Size runOptimize: " << r1->getSizeInBytes() << std::endl;
  t_tmp = clock();
  size_t ret2 = r1->shrinkToFit();
  clock_t t_shrinkToFit = clock() - t_tmp;
  std::cout << "Size shrinkToFit: " << r1->getSizeInBytes() << std::endl;
  std::cout << "run_container exist: " << ret1 << std::endl;
  std::cout << "shrink byte: " << ret2 << std::endl;
  std::cout << "insert time: " << t_insert << std::endl;
  std::cout << "runOptimize time: " << t_runOptimize << std::endl;
  std::cout << "shrinkToFit time: " << t_shrinkToFit << std::endl;
  std::cout << "cardinality: " << r1->cardinality() << std::endl;
  
  //container size test
  /*std::map<int, uint64_t> res_type;
  std::vector<int> res_num;
  int hb_num;
  r1->each_container_type(res_type, res_num, hb_num);
  cout << "each type container size: " << endl;
  for(auto tmp : res_type){
    cout << tmp.first << " : " << tmp.second << endl;
  }
  cout << "hb_num: " << hb_num << endl;
  cout << "total_containers_num: " << res_num.size() << endl;
  double av_container_size = (double)accumulate(res_num.begin(), res_num.end(), 0) / (double)res_num.size();
  cout << "av_container_size: " << av_container_size << endl;
  double S = 0;
  for (int i = 0; i < res_num.size(); i++){
      S += pow((double)res_num[i] - av_container_size, 2);
  }
  cout << "s_container_size: " << S / (res_num.size() - 1) << endl;
  std::sort(res_num.begin(), res_num.end());
  cout << "0.01 rate distribute: " << endl;
  cout << res_num[0] << ",";
  for(double i = 0.01; i <= 1; i += 0.01){
    cout << res_num[(int)(res_num.size() * i) - 1] << ",";
  }
  cout << endl;*/

  //search time test
  for(int i = 0; i < repeat_times; i++){
    t_tmp = clock();
    for(uint64_t i = 1; i <= search_times; i++){
      r1->contains(num_in[i]);
    }
    search_time_in += (clock() - t_tmp);
  }

  for(int i = 0; i < repeat_times; i++){
    t_tmp = clock();
    for(uint64_t i = 1; i <= search_times; i++){
      r1->contains(i);
    }
    search_time_no += (clock() - t_tmp);
  }
  std::cout << "search_time_in: " << search_time_in / repeat_times << std::endl;
  std::cout << "search_time_no: " << search_time_no / repeat_times << std::endl;

  //serialize test
  /*cout << endl << "---------serialize test---------------" << endl;
  uint64_t portable_size = r1->getSizeInBytes(true);
  uint64_t un_portable_size = r1->getSizeInBytes(false);
  cout << "portable_size: " << portable_size << endl;
  cout << "un_portable_size: " << un_portable_size << endl;
  char* portable_buf = new char[portable_size];
  char* un_portable_buf = new char[un_portable_size];
  t_tmp = clock();
  uint64_t portable_write_size = r1->write(portable_buf, true);
  clock_t t_portable_write = clock() - t_tmp;
  cout << "t_portable_write: " << t_portable_write << endl;
  t_tmp = clock();
  uint64_t un_portable_write_size =r1->write(un_portable_buf, false);
  clock_t t_un_portable_write = clock() - t_tmp;
  cout << "t_un_portable_write: " << t_un_portable_write << endl;
  ofstream ser_data1("portable_white_serilize_data");
  ser_data1.write(portable_buf, portable_size);
  ser_data1.close();
  ofstream ser_data2("un_portable_white_serilize_data");
  ser_data2.write(un_portable_buf, un_portable_size);
  ser_data2.close();
  cout << "portable_write_size: " << portable_write_size << endl;
  cout << "un_portable_write_size: " << un_portable_write_size << endl; 


  t_tmp = clock(); 
  Roaring64Map save_portable_RBM64 = Roaring64Map::readSafe(portable_buf, portable_size);
  clock_t t_save_portable_read = clock() - t_tmp;
  t_tmp = clock();
  Roaring64Map norm_portable_RBM64 = Roaring64Map::read(portable_buf, true);
  clock_t t_norm_portable_read = clock() - t_tmp;
  t_tmp = clock();
  Roaring64Map save_un_portable_RBM64 = Roaring64Map::readSafe(un_portable_buf, un_portable_size);
  clock_t t_save_un_portable_read = clock() - t_tmp;
  t_tmp = clock();
  Roaring64Map norm_un_portable_RBM64 = Roaring64Map::read(un_portable_buf, false);
  clock_t t_norm_un_portable_read = clock() - t_tmp;
  cout << "t_save_portable_read: " << t_save_portable_read << endl;
  cout << "t_norm_portable_read: " << t_norm_portable_read << endl;
  cout << "t_save_un_portable_read: " << t_save_un_portable_read << endl;
  cout << "t_norm_un_portable_read: " << t_norm_un_portable_read << endl;*/
  delete r1;
}

void load_file(){
  clock_t t_tmp;
  char* tmp_buf;
  uint64_t portable_size = 691204504;
  uint64_t un_portable_size = 508744262;
  ifstream seri_data1("./bloom.bin");
  //test length
  seri_data1.seekg(0, ios::end);
  uint64_t length1 = seri_data1.tellg();
  seri_data1.seekg(0, ios::beg);
  cout << "length1: " << length1 << endl;
  //test length end
  char* portable_buf = new char[length1];
  seri_data1.read(portable_buf, length1);
  seri_data1.close();
  seri_data1.clear();
  ifstream seri_data2("./un_portable_white_serilize_data");
  //test length
  seri_data2.seekg(0, ios::end);
  uint64_t length2 = seri_data2.tellg();
  seri_data2.seekg(0, ios::beg);
  cout << "length2: " << length2 << endl;
  //test length end
  char* un_portable_buf = new char[un_portable_size];
  seri_data2.read(un_portable_buf, un_portable_size);
  seri_data2.close();
  seri_data2.clear();
  //test
  std::stringstream bitmap_data;
  bitmap_data.write(portable_buf, length1);
  char * portable_buf_out = new char[length1];
  bitmap_data.read(portable_buf_out, length1);
  Roaring64Map save_un_portable_RBM64 = Roaring64Map::readSafe(portable_buf_out, length1);
  cout << "card: " << save_un_portable_RBM64.cardinality() << endl;
  //test end

  t_tmp = clock(); 
  //tmp_buf = portable_buf;
  Roaring64Map save_portable_RBM64 = Roaring64Map::readSafe(portable_buf, portable_size);
  clock_t t_save_portable_read = clock() - t_tmp;
  t_tmp = clock();
  cout << "card: " << save_portable_RBM64.cardinality() << endl;//test
  //tmp_buf = portable_buf;
  Roaring64Map norm_portable_RBM64 = Roaring64Map::read(portable_buf, true);
  clock_t t_norm_portable_read = clock() - t_tmp;
  t_tmp = clock();
  cout << "card: " << save_portable_RBM64.cardinality() << endl;//test
  if (save_portable_RBM64 == norm_portable_RBM64){
    cout << "same" << endl;
  }
  tmp_buf = un_portable_buf;
  //Roaring64Map save_un_portable_RBM64 = Roaring64Map::readSafe(tmp_buf, un_portable_size);
  clock_t t_save_un_portable_read = clock() - t_tmp;
  t_tmp = clock();
  tmp_buf = un_portable_buf;
  Roaring64Map norm_un_portable_RBM64 = Roaring64Map::read(tmp_buf, false);
  clock_t t_norm_un_portable_read = clock() - t_tmp;
  cout << "t_save_portable_read: " << t_save_portable_read << endl;
  cout << "t_norm_portable_read: " << t_norm_portable_read << endl;
  cout << "t_save_un_portable_read: " << t_save_un_portable_read << endl;
  cout << "t_norm_un_portable_read: " << t_norm_un_portable_read << endl;

  auto new_data = std::make_shared<Roaring64Map>(Roaring64Map::read(portable_buf, true));
  auto now_data = new_data;
  cout << "now_data ok, cardinality = " << now_data->cardinality() << endl;

  delete portable_buf;
  delete un_portable_buf;
}

void mem_test_seq_32(uint32_t end){
  Roaring* r1 = new Roaring;
  uint32_t rest = end;
  uint32_t res = 0;
  uint32_t min = 1;
  clock_t t_tmp = clock();
  for (uint32_t i = 1; i <= end; i++) {
    r1->add(i);
  }
  clock_t t_insert = clock() - t_tmp;
  std::cout << "Size proto: " << r1->getSizeInBytes() << std::endl;
  t_tmp = clock();
  bool ret1 = r1->runOptimize();
  clock_t t_runOptimize = clock() - t_tmp;
  std::cout << "Size runOptimize: " << r1->getSizeInBytes() << std::endl;
  t_tmp = clock();
  size_t ret2 = r1->shrinkToFit();
  clock_t t_shrinkToFit = clock() - t_tmp;
  std::cout << "Size shrinkToFit: " << r1->getSizeInBytes() << std::endl;
  std::cout << "run_container exist: " << ret1 << std::endl;
  std::cout << "shrink byte: " << ret2 << std::endl;
  std::cout << "insert time: " << t_insert << std::endl;
  std::cout << "runOptimize time: " << t_runOptimize << std::endl;
  std::cout << "shrinkToFit time: " << t_shrinkToFit << std::endl;
  std::cout << "sizeof_r1: " << sizeof(*r1) << std::endl;
  delete r1;
}

void mem_test_ran_32(uint32_t end){
  Roaring* r1 = new Roaring;
  uint32_t rest = end;
  uint32_t res = 0;
  uint32_t min = 1;
  clock_t t_tmp = clock();
  for (uint32_t i = 1; i <= end; i++) {
    //res = range_rand(2000000000, min, rest);
    //if(i % (end/10) == 0){
    //  std::cout << "ran_res: " << res << std::endl;
    //  std::cout << "cardinality: " << r1->cardinality() << std::endl;
    //}
    r1->add(rand());
    //rest--;
  }
  clock_t t_insert = clock() - t_tmp;
  std::cout << "Size proto: " << r1->getSizeInBytes() << std::endl;
  t_tmp = clock();
  bool ret1 = r1->runOptimize();
  clock_t t_runOptimize = clock() - t_tmp;
  std::cout << "Size runOptimize: " << r1->getSizeInBytes() << std::endl;
  t_tmp = clock();
  size_t ret2 = r1->shrinkToFit();
  clock_t t_shrinkToFit = clock() - t_tmp;
  std::cout << "Size shrinkToFit: " << r1->getSizeInBytes() << std::endl;
  std::cout << "run_container exist: " << ret1 << std::endl;
  std::cout << "shrink byte: " << ret2 << std::endl;
  std::cout << "insert time: " << t_insert << std::endl;
  std::cout << "runOptimize time: " << t_runOptimize << std::endl;
  std::cout << "shrinkToFit time: " << t_shrinkToFit << std::endl;
  std::cout << "cardinality: " << r1->cardinality() << std::endl;
  std::cout << "sizeof_r1: " << sizeof(*r1) << std::endl;
  delete r1;
}

uint32_t range_rand(uint32_t max, uint32_t& min, uint32_t rest){
  uint32_t range = max - min;
  uint32_t res = 0;
  //srand((unsigned)time(NULL));
  uint32_t ran = rand()%10000;
  if(ran <= (10000 * ((double)rest/(double)range))){
    res = min;
    min = min + 1;
    return res;
  }
  else{
    min = min + 1;
    return range_rand(max, min, rest);
  }
}

void mem_test_seq_64(uint64_t end){
  Roaring64Map* r1 = new Roaring64Map;
  uint64_t high_flag = 0;
  uint64_t low_num[300];
  for(int i = 0; i < 300; i++){
    low_num[i] = 1;
  }
  clock_t t_tmp = clock();
  for (uint64_t i = 1; i <= end; i++) {
    //if(i % (end/50) == 0){
    //  std::cout << i << std::endl;
    //}
    //high_flag = rand() % 210000;
    r1->add(i * 4500000);
  }
  clock_t t_insert = clock() - t_tmp;
  std::cout << "Size proto: " << r1->getSizeInBytes() << std::endl;
  t_tmp = clock();
  bool ret1 = r1->runOptimize();
  clock_t t_runOptimize = clock() - t_tmp;
  std::cout << "Size runOptimize: " << r1->getSizeInBytes() << std::endl;
  t_tmp = clock();
  size_t ret2 = r1->shrinkToFit();
  clock_t t_shrinkToFit = clock() - t_tmp;
  std::cout << "Size shrinkToFit: " << r1->getSizeInBytes() << std::endl;
  std::cout << "run_container exist: " << ret1 << std::endl;
  std::cout << "shrink byte: " << ret2 << std::endl;
  std::cout << "insert time: " << t_insert << std::endl;
  std::cout << "runOptimize time: " << t_runOptimize << std::endl;
  std::cout << "shrinkToFit time: " << t_shrinkToFit << std::endl;
  std::cout << "cardinality: " << r1->cardinality() << std::endl;
  std::cout << "sizeof_r1: " << sizeof(*r1) << std::endl;
  delete r1;
}

void mem_test_ran_64(uint64_t end){
  Roaring64Map* r1 = new Roaring64Map;
  uint64_t rest = end;
  uint64_t res = 0;
  uint64_t min = 1;
  for (uint64_t i = 1; i <= end; i++) {
    res = range_rand_64(MAX_64, min, rest);
    if(i % (end/10) == 0){
      std::cout << "ran_res: " << res << std::endl;
      std::cout << "cardinality: " << r1->cardinality() << std::endl;
    }
    r1->add(res);
    rest--;
  }
  std::cout << "Size proto: " << r1->getSizeInBytes() << std::endl;
  bool ret1 = r1->runOptimize();
  std::cout << "Size runOptimize: " << r1->getSizeInBytes() << std::endl;
  size_t ret2 = r1->shrinkToFit();
  std::cout << "Size shrinkToFit: " << r1->getSizeInBytes() << std::endl;
  std::cout << "run_container exist: " << ret1 << std::endl;
  std::cout << "shrink byte: " << ret2 << std::endl;
  std::cout << "sizeof_r1: " << sizeof(*r1) << std::endl;
  delete r1;
}

uint64_t range_rand_64(uint64_t max, uint64_t& min, uint64_t rest){
  uint64_t range = max - min;
  uint64_t res = 0;
  //srand((unsigned)time(NULL));
  uint64_t ran = rand()%1000000000;
  if(ran <= (1000000000 * ((double)rest/(double)range))){
    res = min;
    min = min + 1;
    return res;
  }
  else{
    min = min + 1;
    return range_rand_64(max, min, rest);
  }
}

void search_test_seq_64(uint64_t end){
  Roaring64Map* r1 = new Roaring64Map;
  uint64_t gap = (9 * pow(10, 14) / end) - 1;
  clock_t t_tmp;
  clock_t search_time_in = 0;
  clock_t search_time_no = 0;

  for (uint64_t i = 1; i <= end; i++) {
    r1->add(i * gap);
  }
  for(int i = 0; i < 10; i++){
    t_tmp = clock();
    for(uint64_t i = 1; i <= 200000; i++){
      r1->contains(i * gap);
    }
    search_time_in += (clock() - t_tmp);
  }

  for(int i = 0; i < 10; i++){
    t_tmp = clock();
    for(uint64_t i = 1; i <= 200000; i++){
      r1->contains(i);
    }
    search_time_no += (clock() - t_tmp);
  }


  std::cout << "nums: " << end << std::endl;
  std::cout << "search_time_in: " << search_time_in / 10 << std::endl;
  std::cout << "search_time_no: " << search_time_no / 10 << std::endl;
  std::cout << "sizeof_r1: " << sizeof(*r1) << std::endl;
  delete r1;
}



void mem_test(){
  Roaring64Map* r1 = new Roaring64Map;

  for (uint64_t i = 1; i <= 20000000; i++) {
    r1->add(((uint64_t)rand() << 32) + (uint64_t)rand());
  }  
  cout << "men: " << r1->getSizeInBytes() << endl;

  delete r1;
}

void short_test_real_64(int file_num, uint64_t search_times, int repeat_times){
  Roaring64Map* r1 = new Roaring64Map;
  clock_t t_tmp;
  clock_t search_time_in = 0;
  clock_t search_time_no = 0;
  vector<uint64_t> num_in;

  t_tmp = clock();
  for(int f_i = 0; f_i < 1; f_i++){
    ifstream data(file_src + file_name_ip[f_i]);
    uint64_t num;
    int count = 0;
    while(data >> num){
      if(count > 210000){
        break;
      }
      r1->add(num);
      if((num_in.size() < search_times)){
        num_in.push_back(num);
      }
      count ++;
    }
    data.close();
    data.clear();
  }
  clock_t t_insert = clock() - t_tmp;
  std::cout << "Size proto: " << r1->getSizeInBytes() << std::endl;
  t_tmp = clock();
  bool ret1 = r1->runOptimize();
  clock_t t_runOptimize = clock() - t_tmp;
  std::cout << "Size runOptimize: " << r1->getSizeInBytes() << std::endl;
  t_tmp = clock();
  size_t ret2 = r1->shrinkToFit();
  clock_t t_shrinkToFit = clock() - t_tmp;
  std::cout << "Size shrinkToFit: " << r1->getSizeInBytes() << std::endl;
  std::cout << "run_container exist: " << ret1 << std::endl;
  std::cout << "shrink byte: " << ret2 << std::endl;
  std::cout << "insert time: " << t_insert << std::endl;
  std::cout << "runOptimize time: " << t_runOptimize << std::endl;
  std::cout << "shrinkToFit time: " << t_shrinkToFit << std::endl;
  std::cout << "cardinality: " << r1->cardinality() << std::endl; 

  for(int i = 0; i < repeat_times; i++){
    t_tmp = clock();
    for(uint64_t i = 1; i <= search_times; i++){
      r1->contains(num_in[i]);
    }
    search_time_in += (clock() - t_tmp);
  }

  for(int i = 0; i < repeat_times; i++){
    t_tmp = clock();
    for(uint64_t i = 1; i <= search_times; i++){
      r1->contains(i);
    }
    search_time_no += (clock() - t_tmp);
  }
  std::cout << "search_time_in: " << search_time_in / repeat_times << std::endl;
  std::cout << "search_time_no: " << search_time_no / repeat_times << std::endl;

  std::cout << "sizeof_r1: " << sizeof(*r1) << std::endl;
  delete r1;
}

int parse_ip(string &ip){
  int res = 0;
  vector<string> ip_parts = split(ip, ".");
  for (int i = 0; i < ip_parts.size(); i++){
    res += stoi(ip_parts[i]) << ((ip_parts.size() - 1 - i) * 8);
  }
  return res;
}

vector<string> split(const string &s, const string &seperator){
  vector<string> result;
  typedef string::size_type string_size;
  string_size i = 0;
  
  while(i != s.size()){
    //找到字符串中首个不等于分隔符的字母；
    int flag = 0;
    while(i != s.size() && flag == 0){
      flag = 1;
      for(string_size x = 0; x < seperator.size(); ++x){
        if(s[i] == seperator[x]){
          ++i;
          flag = 0;
          break;
        }
      }
    }
    
    //找到又一个分隔符，将两个分隔符之间的字符串取出；
    flag = 0;
    string_size j = i;
    while(j != s.size() && flag == 0){
      for(string_size x = 0; x < seperator.size(); ++x){
        if(s[j] == seperator[x]){
          flag = 1;
          break;
        }
      }
      if(flag == 0) 
        ++j;
    }
    if(i != j){
      result.push_back(s.substr(i, j-i));
      i = j;
    }
  }
  return result;
}