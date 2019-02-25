/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT                                             */
/*    FILE: Odometry.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <cmath>
#include <iterator>
#include "MBUtils.h"
#include "Odometry.h"

using namespace std;
bool debug=false;

//---------------------------------------------------------
Odometry::Odometry() // Constructor. here I inialize the vars. 
{
   m_first_reading = true;// true if this is the first reading
   m_new_pair = true; // every time we get a new piece of mail that is NAV_X or NAV_Y, then we know that there exists a new possible pair. Otherwise no counter iteration and if the counters match, then it is an old pair of data of NAV_X and NAV_Y and we would do nothing
   m_current_x = 0;
   m_current_y = 0;
   m_previous_x = 0;
   m_previous_y = 0;
   m_total_distance = 0;
   m_counter_x = 0;
   m_counter_y = 0;
}

//---------------------------------------------------------
// Destructor

Odometry::~Odometry()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Odometry::OnNewMail(MOOSMSG_LIST &NewMail)
/*MOOSMSG_LIST NewMail is a discreet list of mail pieces 
that includes all of your MOOS vars, ie NAV_X or NAV_Y 
not necessarily in any order, even if you would rather 
have them in pairs, you need to figure out how to process*/
{
  if(debug) { cout << "I'm in ON NEW MAIL" << endl;}
  AppCastingMOOSApp::OnNewMail(NewMail);
  
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

     string key   = msg.GetKey(); //this is the MOOS var name
     double dval  = msg.GetDouble();//dval is the MOOS var double value

     if(debug) { cout << "Key = " << key << endl;}
     
    if(key=="NAV_X") { // check if the piece of mail is NAV_X
      if(debug) { cout << "X VALUE: " << dval << endl;}
      x_vals.push_back(dval);
    }
    if(key=="NAV_Y") { // check if the piece of mail is NAV_Y
      if(debug) { cout << "Y VALUE: " << dval << endl;}
      y_vals.push_back(dval);
    }
     /*
    string comm  = msg.GetCommunity();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
     */
     /* FIRST TRY ASSUMING X, X, Y, X, Y, Y, ETC.
     if(key=="NAV_X") { // check if the piece of mail is NAV_X
      
       m_current_x = dval;
       m_counter_x++;
       m_new_pair = true; 
     }
     if(key=="NAV_Y") { // check if the piece of mail is NAV_Y
      
       m_current_y = dval;
       m_counter_y++;
       m_new_pair = true;
     }
     */
  }
  return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Odometry::OnConnectToServer()
{

   // register for variables here by just calling RegisterVariables()
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Odometry::Iterate()
{
  if(debug) { cout << "I'm in ITERATE" << endl;}
  AppCastingMOOSApp::Iterate();
  
  //do not publish anything if first reading (special case)
  if(m_first_reading) {
    m_total_distance = 0;
    m_first_reading = false; //should set m_first_reading to false for the rest of time (since program starts)
  }

  while(!x_vals.empty() && !y_vals.empty()) {
    m_current_x = x_vals.back();
    if(debug) { cout << "current x is: " << m_current_x << endl; }
    x_vals.pop_back();  
    m_current_y = y_vals.back();
    if(debug) { cout << "current y is: " << m_current_y << endl; }    
    y_vals.pop_back();
    m_total_distance+=sqrt(pow(m_current_x-m_previous_x,2)+pow(m_current_y-m_previous_y,2));
    if(debug) { cout << "cum. total distance is: " << m_total_distance << endl; }

   // set the new x and y's
    m_previous_x = m_current_x;
    m_previous_y = m_current_y;
  }
  //publish MOOS variable odometry distance
   Notify("ODOMETRY_DIST", m_total_distance);

  //once published, clear x_vals and y_vals
   x_vals.clear();
   y_vals.clear();
  
  /*
  //publish a total distance based on a matched NAV_X, NAV_Y pair, if we have a matched pair && a new pair 
  else if(m_counter_x==m_counter_y && m_new_pair) {
    m_total_distance += sqrt(pow(m_current_x-m_previous_x,2)+pow(m_current_y-m_previous_y,2));
    Notify("ODOMETRY_DIST", m_total_distance);
    m_new_pair=false;//a new total distance has been published as a piece of mail (from x & y matched pairs), so then reset m_new_pair to false because it's no longer a new unpublished pair. It's been published now.
  */
  
    //reportEvent("New Odometry Distance Notified for a matched pair!" + "\textbackslash{n");

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Odometry::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();
  
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Odometry::RegisterVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  
  Register("NAV_X", 0); // second argument is in freq but likes integers. The value of 0 gives you as fast as freq possible for all mail sent of NAV_X/NAV_Y
  Register("NAV_Y", 0);
}

bool Odometry::buildReport()
{
  //m_msgs << "Count X: " << m_counter_x << ", Count Y: " << m_counter_y << endl;
  //m_msgs << "Current X: " << m_current_x << ", Current Y: " << m_current_y << endl;
  m_msgs << "Total Distance Traveled: "<< m_total_distance << endl << endl;

  return (true);
}
