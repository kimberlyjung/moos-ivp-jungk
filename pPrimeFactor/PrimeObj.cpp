/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeObj.cpp                                         */
/*    DATE: 23FEB2019                                                */
/************************************************************/

#include "PrimeObj.h"

using namespace std;

PrimeObj::~PrimeObj() {
}

PrimeObj::PrimeObj() {
   number = 0;
   queue_num = 0;
   calculated_order = 0;
   timestamp_begin = 0;
   timestamp_end = 0;
   iteration_index = 2;
   is_complete = false;
   list_primes.push_back(1); //add the first prime number "1" to the list
}

void set_number(uint64_t a) {number=a;}
uint64_t get_number() {return number;}
void set_queue_num(int a) {queue_num=a;}
int get_queue_num() {return queue_num;}
void set_calculated_order(int a) {calculated_order=a;}
int get_calculated_order() {return calculated_order;}
void set_timestamp_begin(int a) {timestamp_begin=a;}
int get_timestamp_begin() {return timestamp_begin;}
void set_timestamp_end(int a) {timestamp_end=a;}
int get_timestamp_end() {return timestamp_end;}
void set_iteration_index(int a) {iteration_index=a;}
void add_iteration_index() {iteration_index++;}
int get_iteration_index() {return iteration_index;}
void set_is_complete(bool a) {is_complete=a;}
bool get_is_complete() {return is_complete;}
void add_list_primes(uint64_t k) {list_primes.push_back(k);}
int size_list_primes() {return list_primes.size();}
uint64_t popnext_list_primes()
   {
     uint64_t front = list_primes.front();
     list_primes.pop_front();
     return front;
   }
