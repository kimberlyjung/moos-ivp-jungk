/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT, 2.680, Lab 9                                   */
/*    FILE: PointsAssign.cpp                                        */
/*    DATE: 19MAR2019                                                */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "PointAssign.h"
#include <string>
#include <iostream>

using namespace std;

//---------------------------------------------------------
// Constructor

PointAssign::PointAssign()
{
  m_finished=false;
  m_sent_all=false;
  m_assign_by_region=false;
  gilda_count=0;
  henry_count=0;
  m_genpath_ready=false;
}

//---------------------------------------------------------
// Destructor

PointAssign::~PointAssign()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PointAssign::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);
  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();
    string sval  = msg.GetString();
    cout << "Key: " << key << ", val=" << sval << endl;
    if(key=="VISIT_POINT")
      {
	if(sval=="lastpoint")
	  {
	    m_finished = true;
	  }
	m_vector.push_back(sval);
      }
    else if(key=="ASSIGN_REGION") {m_assign_by_region=true;}
    else if(key=="GENPATH_READY") {m_genpath_ready=true;}
    
#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
    
    else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
      reportRunWarning("Unhandled Mail: " + key);
  }
  cout << "DONE" << endl;
  return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PointAssign::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PointAssign::Iterate()
{
  AppCastingMOOSApp::Iterate();

  if(!m_finished || m_sent_all || !m_genpath_ready) {return true;} //if not yet finished, don't process yet
  for(unsigned int i=0; i<m_vector.size(); i++)
    {
      if(m_vector[i]=="firstpoint" || m_vector[i]=="lastpoint")
	{
	  Notify("VISIT_POINT_GILDA", m_vector[i]);
	  Notify("VISIT_POINT_HENRY", m_vector[i]);
	}
      else 
	{
	  int x=stoi(tokStringParse(m_vector[i],"x", ',', '='));
	  int y=stoi(tokStringParse(m_vector[i],"y", ',', '='));
	  string id=tokStringParse(m_vector[i],"id", ',', '=');
	  
	  if(!m_assign_by_region)
	    //assign to every other to Henry/Gilda
	    {
	      if(i%2==0)
		{
		  postViewPoint(x,y,id, "red");
		  Notify("VISIT_POINT_HENRY", m_vector[i]);
		  henry_count++;
		} 
	      else
		{
		  postViewPoint(x,y,id, "yellow");
		  Notify("VISIT_POINT_GILDA", m_vector[i]);
		  gilda_count++;
		}
	    }
	  else 
	    //assign by region to Henry/Gilda
	    {
	      if(x>112) //HENRY=west
		{
		  postViewPoint(x,y,id, "red");
		  Notify("VISIT_POINT_HENRY", m_vector[i]);
		  henry_count++;
		} 
	      else //GILDA=east
		{
		  postViewPoint(x,y,id, "yellow");
		  Notify("VISIT_POINT_GILDA", m_vector[i]);
		  gilda_count++;
		} 
	    }
         }
    }
  m_sent_all=true;
  Notify("POINTS_RECEIVED", "true");
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PointAssign::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  Notify("UTS_PAUSE","false");
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

    //this below creates a configuration parameter
    if(param == "assign_by_region") {
      handled = true;
      if(tolower(value) == "true") m_assign_by_region=true;
    }
    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void PointAssign::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("VISIT_POINT", 0);
  Register("GENPATH_READY",0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool PointAssign::buildReport() 
{
  m_msgs << "Gilda point count" << gilda_count<< endl;
  m_msgs << "Henry point count" << henry_count << endl;
  
  /*ACTable actab(4);
  actab << "VectorSize | vp_counter | vp_first ";
  actab.addHeaderLines();
  actab << to_string(m_vector.size()) << to_string(vp_counter) << to_string(vp_first);
  m_msgs << actab.getFormattedString();
  */
  
  return(true);
}

//---------------------------------------------------------
//Procedure: postViewPoint

void PointAssign::postViewPoint(double x, double y, std::string label, std::string color)
{
  XYPoint point(x,y);
  point.set_label(label);
  point.set_color("vertex", color);
  point.set_param("vertex_size", "2");

  string spec = point.get_spec();
  Notify("VIEW_POINT", spec);
}
