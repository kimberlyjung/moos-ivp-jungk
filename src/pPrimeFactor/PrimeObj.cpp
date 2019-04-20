/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeObj.cpp                                         */
/*    DATE: 23FEB2019                                                */
/************************************************************/

#include "PrimeObj.h"

using namespace std;

PrimeObj::PrimeObj() { //Constructor
   m_number = 0;
   m_queue_num = 0;
   m_calculated_order = 0;
   m_timestamp_begin = 0;
   m_timestamp_end = 0;
   m_iteration_index = 3;
   m_is_complete = false;
   m_tempnum = 0;
   m_num_steps = 0;
}

string PrimeObj::primePrint()
{
  unsigned int listsize = m_list_primes.size();
  stringstream ss;

  ss << "PRIME_RESULT = orig=" << m_number << ",received=" << m_queue_num;
  ss << ",calculated=" << m_calculated_order << ",solve_time=";
  ss << m_timestamp_end-m_timestamp_begin << ",primes";

  while(m_list_primes.size()>0) {
      uint64_t front = m_list_primes.front();
      ss << ":" << front;
      m_list_primes.pop_front();
    }
  ss << ",username=jungk";
  cout << ss.str() << endl;
  return(ss.str());
}


void PrimeObj::factorPrimes()
{
  if(debug) {cout<<"Incomplete factorization for "<<m_number<<",numsteps="<<m_num_steps<<",Iterationindex="<<m_iteration_index<<endl;}	
  while(m_tempnum%2==0 && m_num_steps<100000) { // handle the even numbers
      m_list_primes.push_back(2);
      m_tempnum=m_tempnum/2;
      m_num_steps++;
      if(debug) {cout<<"#"<<m_number<<":numsteps="<<m_num_steps<<",Iteration index"<<m_iteration_index<<endl;}	
    }
  while((m_iteration_index<=sqrt(m_tempnum))&&(m_num_steps<100000)) { //only odd numbers
      if(debug) {cout<<"Odd,#"<<m_number<<",num_steps="<<m_num_steps<<",tempnum="<<m_tempnum<<"m_iteration_index="<<m_iteration_index<<endl;}
      while(m_tempnum%m_iteration_index==0) {
	  m_list_primes.push_back(m_iteration_index);
	  m_tempnum=m_tempnum/m_iteration_index;
	  m_num_steps++;
	  if(debug) {cout<<"Added"<<m_iteration_index<<",#"<<m_number<<",tempnum="<<m_tempnum<<",listprimessize="<<m_list_primes.size()<<",num_steps ="<<m_num_steps<<endl;}
	}
      m_num_steps++;
      m_iteration_index+=2;
    }
  if(m_iteration_index>=sqrt(m_tempnum)) { //factorization is now done
      if(debug) { cout << "iteration_index " << m_iteration_index << " is now greater or equal to sqrt(tempnum) " << sqrt(m_tempnum) << " and now set is_complete to true" << endl;}
      m_is_complete=true;
      if(m_tempnum>2) {//if the remaining prime number is greater than 2, add this factor to the list
	  if(debug) { cout << "post-while loops; Adding " << m_tempnum << " for #" << m_number << ", tempnum is " << m_tempnum << ", listprimessize is " << m_list_primes.size() << ", num_steps is " << m_num_steps << endl;}
	  m_list_primes.push_back(m_tempnum);
	  if(debug) {cout << "list primes size is " << m_list_primes.size() << endl;}
	}
    }
  return;
}

