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
 public: 
  PrimeObj(); //declares the Constructor
  ~PrimeObj() {} //declares the Destructor (doesn't need a semicolon because not doing anything)
  
 public:
  // declare variables
  std::uint64_t m_number; // actual number
  std::uint64_t m_tempnum; //divided-into number, processed by Factor()
  int m_queue_num; // number in the queue on receiving
  unsigned int m_calculated_order; // order in which prime factors had completed calculation
  long double m_timestamp_begin; // time stamp in seconds from launch of MOOSDB
  long double m_timestamp_end;// time stamp in seconds from launch of MOOSDB for finished prime factorization
  int m_iteration_index; // keeps track of the prime factorization so far done, between batches
  bool m_is_complete;
  int m_num_steps; //keeps track of less than 10 calculations per batch process
  std::list<uint64_t> m_list_primes;
  bool debug = false;
  
 public:
  string primePrint();
  void factorPrimes();
};

#endif 
