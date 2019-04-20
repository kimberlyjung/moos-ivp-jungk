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

//---------------------------------------------------------
// Constructor

PrimeFactor::PrimeFactor()// Constructor. here I inialize the var
{
  counter = 1; //first PrimeObj element to be added to the STL list
  order = 1; //the first PrimeObj to be complete will have an order it was completed
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
      //if(debug) { cout << "%%%% received NUM_VALUE key %%%%" << endl;}  
      x.number=strtoul(sval.c_str(), NULL, 0); // convert sval string into a uint64_t type
      x.tempnum=x.number;
      x.is_complete=false;
      x.queue_num=counter; counter++; // set the received queue_num of this PrimeObj to current counter and then iterate counter
      x.timestamp_begin=MOOSTime(); // set beginning time stamp for this PrimeObj
      x.iteration_index=3; // start at the value of 3 for iteration index
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
  //if(debug) { cout << "%%%% I'm in Iterate() %%%%" << endl;}
  list<PrimeObj>::iterator p;
  
  while(list_allnums.size()!==0)  //Iterate through list 
  {
    p=list_allnums.begin();
    PrimeObj &x = *p;
    if(debug) {cout << "created new PrimeObj" << endl;}
    if(x.is_complete) //if factorization is complete for this element, then print it out and pop it out of the list_allnums queue
      {
        if(debug) { cout << "%%%% I'm in a completed element where factorization is complete for " << x.is_complete  << " %%%%" << endl;}	
        x.timestamp_end=MOOSTime(); //set final timestamp end
	x.calculated_order=order; order++;//set final calculated order, iterate order
     	Notify("PRIME_RESULT", x.Print()); // print out the element and returns a string that goes into Notify to publish the MOOS var PRIME_RESULT
        list_allnums.pop_front(); //pop out that completed element which is next in queue
      }
    else //if factorization not yet complete for this element, then continue factoring it and then add it back to the end of the queue
      {
	if(debug) { cout << "%%%% In a factorization not complete for " << x.number << " %%%%" << endl;}	
	while((x.num_steps<10) && (x.is_complete==false)) // factor for an iterating iteration_index
	  {
	    x.Factor();
	  }
	x.num_steps=0; //reset 10 num_steps to 0 num_steps
	if(debug) { cout << "Finished batch of factorization for #" << x.number << " and push back and pop front for list_allnums" << endl;}
	list_allnums.push_back(x);//add new x with updated values, back to the end of the queue
	if(debug) { cout << "successfully pushed_back to end of list_allnums for #" << x.number << ", list_allnums size is " << list_allnums.size() <<  endl;}
	list_allnums.pop_front();//remove first element from the queue which was old x
	if(debug) { cout << "successfully pop_front from list_allnums for #" << x.number << ", list_allnums size is " << list_allnums.size() << endl;}
      }
  }
  if(debug) {cout << "Now exiting Iterate" << endl;}
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
