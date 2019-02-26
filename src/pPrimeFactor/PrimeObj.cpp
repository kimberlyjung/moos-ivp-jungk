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

string PrimeObj::Print()
{
  int listsize = m_list_primes.size();
  stringstream ss;

  ss << "PRIME_RESULT = orig=" << m_number << ",received=" << m_queue_num << ",calculated=" << m_calculated_order << ",solve_time=" << m_timestamp_end-m_timestamp_begin << ",primes" << endl;

  while(m_list_primes.size()>0) 
    {
      if(debug) {cout << "\n%%%printing inside for loop" << endl;}
      uint64_t front = m_list_primes.front();
      ss << ":" << front;
      m_list_primes.pop_front();
    }
  ss << ",username=jungk";
  cout << ss.str() << endl;
  return(ss.str());

    /* if(true)
    {
    std:list<uint64_t> copy_list_primes=m_list_primes;
      cout << "%%% In PRINT %%% \n List_size of #" << m_number << " is: " << listsize << endl;
      for(unsigned int k=0; k<listsize; k++)
	{
	  cout << m_number << "-" << k+1 << ":" << copy_list_primes.front() << ",  ";
	  copy_list_primes.pop_front();
	}
    }
  */
}


void PrimeObj::Factor()
{
  while(m_tempnum%2==0 && m_num_steps<10) // handle the even numbers
    {
      m_list_primes.push_back(2);
      m_tempnum=m_tempnum/2;
      m_num_steps++;
    }
  while((m_iteration_index<sqrt(m_tempnum)) && (m_num_steps<10)) // now, only odd numbers
    {
      if(debug) { cout << "In odd numbers factorization first while loop for #" << m_number << " and num_steps is " << m_num_steps << endl;}
      while(m_tempnum%m_iteration_index == 0)
	{
	  if(debug) { cout << "In 2nd while loop" << endl;}
	  m_list_primes.push_back(m_iteration_index);
	  m_tempnum=m_tempnum/m_iteration_index;
	  m_num_steps++;
	  if(debug) { cout << "Added " << m_iteration_index << " for #" << m_number << ", tempnum is " << m_tempnum << ", listprimessize is " << m_list_primes.size() << ", num_steps is " << m_num_steps << endl;}
	}
      m_num_steps++;
      m_iteration_index+=2;
    }
  if(m_iteration_index>=sqrt(m_tempnum)) //factorization is now done
    {
      if(debug) { cout << "iteration_index " << m_iteration_index << " is now greater or equal to sqrt(tempnum) " << sqrt(m_tempnum) << " and now set is_complete to true" << endl;}
      m_is_complete=true;
      if(m_tempnum>2)//if the remaining prime number is greater than 2, add this factor to the list
	{
	  if(debug) { cout << "post-while loops; Adding " << m_tempnum << " for #" << m_number << ", tempnum is " << m_tempnum << ", listprimessize is " << m_list_primes.size() << ", num_steps is " << m_num_steps << endl;}
	  m_list_primes.push_back(m_tempnum);
	  if(debug) {cout << "list primes size is " << m_list_primes.size() << endl;}
	}
    }
}

