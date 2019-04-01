/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT                                             */
/*    FILE: MessageTimer.cpp                                        */
/*    Lab 11 Extra Credit: Assignment 5 Charlie_Dana_Recover */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "MessageTimer.h"

using namespace std;

//---------------------------------------------------------
// Constructor

MessageTimer::MessageTimer()
{
}

//---------------------------------------------------------
// Destructor

MessageTimer::~MessageTimer()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool MessageTimer::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key   = msg.GetKey();

    if(key=="NODE_MESSAGE") {
      Notify("MESSAGE_TIMER_RESET", "true");
      Notify("MESSAGE_TIMER_RESET", "false");
    }
    
#if 0 // Keep these around just for template
    
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool MessageTimer::OnConnectToServer()
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

bool MessageTimer::Iterate()
{
  AppCastingMOOSApp::Iterate();
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool MessageTimer::OnStartUp()
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

void MessageTimer::RegisterVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("NODE_MESSAGE", 0);
}

bool MessageTimer::buildReport()
 {
   // m_msgs << "Number of good messages: " << m_good_message_count << endl;
   //m_msgs << "Number of bad  messages: " << m_bad_message_count  << endl;
   
   return(true);
 }
