/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT                                             */
/*    FILE: CommunicationAngle_jungk.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include <cstdlib>
#include <sstream>
#include <string>
#include <iostream>
#include "CommunicationAngle_jungk.h"

using namespace std;
bool debug = true;

//---------------------------------------------------------
// Constructor

CommunicationAngle_jungk::CommunicationAngle_jungk()
{
  R = 0;
  x_R=0;
  sperm_angle = 0;
  neptune_angle = 0;
  neptune_x = 0;
  neptune_y = 0;
  neptune_z = 0;
  sperm_x = 0;
  sperm_y = 0;
  sperm_z = 0;
  circle_x = 0;
  s=0;
  theta_N_0=0;
  theta_S_0=0;
  c_z0_N=0;
  c_z0_S=0;
  h1=0;
  h2=0;
  commspossible=false;
  base_angle=0;
  central_angle=0;
  R_new=0;
  theta_S_1=0;
  x_R_new=0;
  phi=0;
  sperm_x_new=0;
  sperm_y_new=0;
}

//---------------------------------------------------------
// Destructor

CommunicationAngle_jungk::~CommunicationAngle_jungk()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool CommunicationAngle_jungk::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key   = msg.GetKey();
    double dval  = msg.GetDouble();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
emacs    bool   mstr  = msg.IsString();
#endif
    
    if(key=="NEPTUNE_NAV_X") {neptune_x=dval;}
    if(key=="NEPTUNE_NAV_Y") {neptune_y=dval;}
    if(key=="NEPTUNE_NAV_DEPTH") {neptune_z=dval;}
    if(key=="NAV_X") {sperm_x=dval;}
    if(key=="NAV_Y") {sperm_y=dval;}
    if(key=="NAV_DEPTH") {sperm_z=dval;}
   }
  
   commspossible=false;
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool CommunicationAngle_jungk::OnConnectToServer()
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

bool CommunicationAngle_jungk::Iterate()
{
  while(!commspossible)
    {
      Calcs();
      TransmissionLoss();

      //Transmit if does not hit the ocean floor
     if(z_max<=max_depth) {
        commspossible=true;
        Notify("TRANSMIT_SUCCESS",Transmit(sperm_x, sperm_y, sperm_z));
     }
     else {
       AvoidFloor();
     }
     Notify("COMMSPOSSIBLE",to_string(commspossible));
  }
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool CommunicationAngle_jungk::OnStartUp()
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

void CommunicationAngle_jungk::RegisterVariables()
{
  Register("VEHICLE_NAME", 0);
  Register("COLLABORATOR_NAME", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
  Register("NAV_DEPTH", 0);
  Register("NAV_HEADING", 0);
  Register("NAV_SPEED", 0);
  Register("NEPTUNE_NAV_X", 0);
  Register("NEPTUNE_NAV_Y", 0);
  Register("NEPTUNE_NAV_DEPTH", 0);
  Register("NEPTUNE_NAV_HEADING", 0);
  Register("NEPTUNE_NAV_SPEED", 0);
}

void CommunicationAngle_jungk::Calcs()
{
  stringstream ss2;
  double plane_s, plane_n, A, p;
  
  //Establish sound speeds at depth and z-heights from vehicles to Virtual Circle 
  h1=c_0/g+sperm_z;
  h2=c_0/g+neptune_z;
  c_z0_N = c_0 + g*neptune_z; //sound speed at depth Neptune
  c_z0_S = c_0 + g*sperm_z; //sound speed at depth Spermwhale
  
  //Find 3-D Range between Sperm and Neptune: x, y, z
  range=pow(((pow((sperm_x-neptune_x),2) + pow((sperm_x-neptune_y), 2)) + pow(sperm_z-neptune_z, 2)), 0.5);
  ss2<<"range="<<to_string(range);

  //Find x-y Range between Sperm and Neptune: x, y
  x_R=pow((pow((sperm_x-neptune_x),2) + pow((sperm_x-neptune_y), 2)), 0.5);
  //x_R=pow(neptune_z,2)/(2*range)+(c_0*neptune_z)/(g*range)+(range/2)-pow(sperm_z,2)/(2*range)-(sperm_z*c_0/(range*g));
	  
  //Find Radius of Virtual Circle using geometry
        //R=sqrt(pow(h1,2)+pow(x_R,2));
  plane_s=sqrt(pow(sperm_x,2)+pow(sperm_y,2));
  plane_n=sqrt(pow(neptune_x,2)+pow(neptune_y,2));
  A=plane_n-plane_s;
  p=(pow(h2,2)-pow(h1,2)+pow(A,2))/(2*A);
  R=sqrt(pow(h1,2)+pow(p,2));
  ss2<<",radius="<<to_string(R);

  //Find base triangle angle and central angle
  base_angle=acos((pow(range,2))/(2*R*range));             
  central_angle=pi-2*base_angle;
  ss2<<",central_angle="<<to_string(ConvertDegrees(central_angle));

  //Find arc length
  s=R*central_angle;
  ss2<<",arclength="<<to_string(s);

  //Find maximum depth from Virtual Circle minimum
  z_max = c_z0_S/(g*cos(theta_S_0))-c_0/g;
  ss2<<"z_max="<<to_string(z_max);

  //Find declination angle: sperm to neptune, from horizontal
  theta_S_0 = acos(h1/R);
  theta_N_0 = acos(h2/R);
  Notify("ELEV_ANGLE_SPERM",to_string(ConvertDegrees(-theta_S_0)));
  ss2<<"neptune_angle"<<to_string(ConvertDegrees(theta_N_0));

  //for debugging
  if(debug) {Notify("STATUS",ss2.str());}
}

void CommunicationAngle_jungk::AvoidFloor()
{
  //calculate the angle and location to transmit
  R_new=h1/acos(c_z0_S/(max_depth+c_0/g));
  theta_S_1=acos(h1/R);
  x_R_new=sqrt(pow(R,2)-pow(h1,2));
  phi=acos(x_R);//angle in x-y plane, cos(phi)=x-comp, sin(phi)=y-comp
  sperm_x_new=x_R_new*cos(phi);
  sperm_y_new=x_R_new*sin(phi);
  Notify("NEW_TRANSMIT_LOC",Transmit(sperm_x_new, sperm_y_new, sperm_z));
}

double CommunicationAngle_jungk::ConvertDegrees(double radians) const
{
  return(radians*180/pi);
}

double CommunicationAngle_jungk::TransmissionLoss()
{
  double new_R = c_z0_S/(g*cos(theta_S_0+angle_change));
  double new_range = sqrt(pow(new_R,2)-pow(h2,2));
  double J = (range/sin(theta_N_0))*((new_range-range)/angle_change);
  double pressure_field = 1/(4*pi)*sqrt(c_z0_N*cos(theta_S_0)/(c_z0_S*J));
  if(debug) {cout<<"J="<<J<<",pressure="<<pressure_field<<endl;}
  double m_t_loss = -20*log10(pressure_field);
  Notify("TRANSMISSION_LOSS=",to_string(m_t_loss));
  return (m_t_loss);
}

std::string CommunicationAngle_jungk::Transmit(double x, double y, double z) 
{
  stringstream ss;
  ss<<"X:"<<to_string(x)<<",Y:"<<to_string(y)<<",Z:"<<to_string(z);
  return(ss.str());
}


