/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.cpp                                        */
/*    DATE:  23 MAR 2019                                               */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "GenPath.h"
#include "PointObj.cpp"

using namespace std;

//---------------------------------------------------------
// Constructor

GenPath::GenPath()
{
  m_least_dist=9999999;
  m_finished=false;
  m_x_curr=0;
  m_y_curr=0;
  m_visit_point_count=0;
  m_WPT_update=0;
  m_wvecsize=0;
}

//---------------------------------------------------------
// Destructor

GenPath::~GenPath()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool GenPath::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();
    string sval = msg.GetString();
    double dval = msg.GetDouble();

    if(key=="VISIT_POINT") {
      m_visit_point_count++;
      if(sval=="lastpoint") {m_finished=true;}
      else if(sval!="firstpoint" && !m_finished) {
	m_wvecsize++;
	m_wpts_vec.push_back(sval);
      }
    }

    else if(key=="NAV_X") {m_x_curr=(int) dval;} 
    else if(key=="NAV_Y") {m_y_curr=(int) dval;} 
    else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
      reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool GenPath::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool GenPath::Iterate()
{
  AppCastingMOOSApp::Iterate();
  
  PointObj waypoint;
  unsigned int index_shortest = 0;
  int m_x_wpt, m_y_wpt;
  double m_distance;

  if(m_finished) {
    //first for-loop iterates through shrinking list
    while(m_wpts_vec.size()>0) {
      m_least_dist=9999999;
      //second for-loop finds next shortest point
      for(unsigned int i=0; i<m_wpts_vec.size(); i++) {   
	m_x_wpt = stoi(tokStringParse(m_wpts_vec[i],"x",',','='));
	m_y_wpt = stoi(tokStringParse(m_wpts_vec[i],"y",',','='));
	m_distance=pow((pow(m_x_curr-m_x_wpt,2)+pow(m_y_curr-m_y_wpt,2)),0.5);
	if(m_distance < m_least_dist) {index_shortest=i; m_least_dist=m_distance;}
      }
      //set the shortest point as waypoint object
      waypoint.x=stoi(tokStringParse(m_wpts_vec[index_shortest],"x",',','='));
      waypoint.y=stoi(tokStringParse(m_wpts_vec[index_shortest],"y",',','='));
      waypoint.distance=m_least_dist;
      //add next waypoint to ordered list
      m_ordered_list.add_vertex(waypoint.x, waypoint.y);
      //set next current point
      m_x_curr=waypoint.x; 
      m_y_curr=waypoint.y;
      //shrink wpts_vec list by one
      m_wpts_vec.erase (m_wpts_vec.begin()+index_shortest);
    //Notify entire ordered list, now that list is finished
    string str = "points = ";
    str += m_ordered_list.get_spec();
    Notify("WPT_UPDATE", str);
    m_WPT_update++;
    }
  }

  
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool GenPath::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "foo") {
      handled = true;
    }
    else if(param == "bar") {
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void GenPath::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
  Register("VISIT_POINT", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool GenPath::buildReport() 
{
  m_msgs << "Ordered waypoint list size: " << m_ordered_list.size() << endl;
  m_msgs << "# of VISIT_POINT msgs received: " << m_visit_point_count << endl;
  m_msgs << "Least distance calculated: " << m_least_dist << endl;
  m_msgs << "Bool Finished: " << m_finished << endl;
  m_msgs << "WPT UPDATES #: " << m_WPT_update << endl;
  m_msgs << "waypoint vec largest size: " << m_wvecsize << endl;
						 
 
  /*
  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();
  */

  return(true);
}




