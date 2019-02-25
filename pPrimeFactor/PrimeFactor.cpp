/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.cpp                                        */
/*    DATE: 23FEB2019                                        */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "PrimeFactor.h"

using namespace std;
bool debug=false;

//---------------------------------------------------------
// Constructor

PrimeFactor::PrimeFactor()// Constructor. here I inialize the var
{
  counter = 1; //first PrimeObj element to be added to the STL list
  order = 1; //the first PrimeObj to be complete will have an order it was completed
  debug = true; //debugger 
}

//---------------------------------------------------------
// Destructor

PrimeFactor::~PrimeFactor()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PrimeFactor::OnNewMail(MOOSMSG_LIST &NewMail)
{
  if(debug) { cout << "I'm in ON NEW MAIL" << endl;}
  MOOSMSG_LIST::iterator p;
  
  for(p=NewMail.begin(); p!=NewMail.end(); p++)
    {
    CMOOSMsg &msg = *p;
    double dval  = msg.GetDouble();
    string key   = msg.GetKey();
    string sval = msg.GetString();
    string toprint = "";

    if(key=="NUM_VALUE")
      { 
      PrimeObj x; //create a new PrimeObj
      if(debug) { cout << "%%%% received NUM_VALUE key %%%%" << endl;}  
      x.set_number(strtoul(sval.c_str(), NULL, 0)); // convert sval string into a uint64_t type
      x.set_queue_num(counter); counter++; // set the received queue_num of this PrimeObj to current counter and then iterate counter
      x.set_timestamp_begin(MOOSTime()); // set beginning time stamp for this PrimeObj
      x.set_iteration_index(2); // start at the value of 2 for iteration index
      list_allnums.push_back(x); // add newly created PrimeObj x to the working list 
      }
    } //end of the NewMail for-loop
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PrimeFactor::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PrimeFactor::Iterate()
{
  if(debug) { cout << "%%%% I'm in Iterate() %%%%" << endl;}
  list<PrimeObj>::iterator p;
  
  for (p=list_allnums.begin(); p!=list_allnums.end(); p++)  //Iterate through list
  {
    PrimeObj &x = *p;
    if(x.get_is_complete()) //if factorization is complete for this element, then print it out and pop it out of the list_allnums queue
      {
        if(debug) { cout << "%%%% I'm in a completed x where factorization is complete for " << x.get_number() << " %%%%" << endl;}	
        x.set_timestamp_end(MOOSTime()); //set final timestamp end
	x.set_calculated_order(order); //set final calculated order
	order++; //iterate order
	Notify("PRIME_RESULT", Print(x)); // print out the element and returns a string that goes into Notify to publish the MOOS var PRIME_RESULT
        list_allnums.pop_front(); //pop out that completed element which is next in queue
      }
    else //if factorization not yet complete for this element, then continue factoring it and then add it back to the end of the queue
      {
	if(debug) { cout << "%%%% In a factorization not complete for " << x.get_number() << " %%%%" << endl;}	
	unsigned int k = 0;
	std::uint64_t tempnum = x.get_number();

	if(x.get_iteration_index()<=2) // if iteration is at beginning, ie no previous work has been done
	  {
	    if(debug) { cout << "%%%% Iteration at beginning for " << x.get_number() << " for iteration_index " << x.get_iteration_index() << " %%%%"<< endl;}	
	    if(tempnum%2==0) //if num is even, add 2 to primes
	      {
		x.add_list_primes(2); //add 2 to the list of primes
		tempnum=tempnum/2;
	      }
	    x.set_iteration_index(3); // either way, even or odd, set iteration index to 3
	    if(debug) { cout << "%%%% Iteration Index for " << x.get_number() << "is " << x.get_iteration_index() << " %%%%" << endl;}	
	  }
	for (int j=x.get_iteration_index(); k<10 && j<=sqrt(tempnum); j=j+2) //only need to check odd #s now since if even number, prime 2 would have taken care of it, so iterate by 2 from now on, ie 3, 5, 7, etc. or start from previous existing work at saved iteration index
	  {
	    if(debug) { cout << "%%%% Inside Prime Factorization Calculation Loop for " << x.get_number() << " with iteration_index " << x.get_iteration_index() << " %%%%" << endl;}	
	    if(j>=sqrt(tempnum)) //if we've reached the end of all possible factorization iterations, then factorization is complete
	      {
		x.set_is_complete(true);
		break; //break out of the loop since the factorization is complete, but need to wait till next Iterate loop to process/print the finished element
	      }
            if(tempnum%j==0) //Check if temp_num is divisible by j   
	     {
	        x.add_list_primes(j);
                tempnum = tempnum/j; //reduce needed iterations, i.e. k, because divide tempnum is divisible by that prime factor
	     }
	     x.set_iteration_index(j+2); //iteration_index+=2, keeps track of pre-existing work
	     k++;//keeping track of maximum of 10 iterations per mail batch
	     if(debug) { cout << "%%%% At end of Prime Factorization Calculation Loop for " << x.get_number() << ", tempnum = " << tempnum << " with iteration_index" << x.get_iteration_index() << " and k = " << k << ", j = " << j << " %%%%"  << endl;}	
           }
	list_allnums.pop_front();//take out old x, ie the PrimeFactor object from the front of the queue
	list_allnums.push_back(x);//add new x with updated values, back to the end of the queue
	}
    if(list_allnums.size()==0) {break;} //check if list is empty. if so, then break out of loop/Iterate()
  }
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PrimeFactor::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
      
      if(param == "foo") {
        //handled
      }
      else if(param == "bar") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void PrimeFactor::RegisterVariables()
{
  if(debug) { cout << "I'm in Register Variables()" << endl;}  
  Register("NUM_VALUE", 0);
}

//----------------------------------------------------------
// Procedure: Return the difference between time stamps and convert to Minutes

string PrimeFactor::Print(PrimeObj x)
{
  if(debug) { cout << "%%%% I'm in PRINT for " << x.get_number() << " %%%%" << endl;}  
  stringstream ss;
  int listsize = x.size_list_primes();
  
  ss << "PRIME_RESULT = orig=" << x.get_number() << ",received=" << x.get_queue_num() << ",calculated=" << x.get_calculated_order() << ",solve_time=" << (x.get_timestamp_end()-x.get_timestamp_begin()) << ",primes" << endl;

  for(unsigned int i=0; i<listsize; i++)
    {
      ss << ":" << x.popnext_list_primes();
    }
  ss << ",username=jungk";
  cout << endl << ss.str();
  return(ss.str());
}



/*BELONGS IN ONNEWMAIL()
#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
    #endif  */
