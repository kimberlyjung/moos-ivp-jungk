/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeObj.h                                          */
/*    DATE: 23FEB2019                                                */
/************************************************************/

#ifndef PrimeObj_HEADER
#define PrimeObj_HEADER

using namespace std;

#include <vector>
#include <stdint.h>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <list>

class PrimeObj 
{
 protected:
  // declare variables
  std::uint64_t number; // actual number
  int queue_num; // number in the queue on receiving
  int calculated_order; // order in which prime factors had completed calculation
  int timestamp_begin; // time stamp in seconds from launch of MOOSDB
  int timestamp_end;// time stamp in seconds from launch of MOOSDB for finished prime factorization
  int iteration_index; // keeps track of the prime factorization so far done, between batches
  bool is_complete;
  std::list<uint64_t> list_primes;
  
 public:
  void set_number(uint64_t a);
  uint64_t get_number(); 
  void set_queue_num(int a);
  int get_queue_num(); 
  void set_calculated_order(int a); 
  int get_calculated_order(); 
  void set_timestamp_begin(int a);
  int get_timestamp_begin(); 
  void set_timestamp_end(int a); 
  int get_timestamp_end(); 
  void set_iteration_index(int a); 
  void add_iteration_index(); 
  int get_iteration_index(); 
  void set_is_complete(bool a);
  bool get_is_complete(); 
  void add_list_primes(uint64_t k);
  int size_list_primes();
  
  uint64_t popnext_list_primes();
};

#endif 
