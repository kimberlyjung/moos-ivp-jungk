/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT                                             */
/*    FILE: CommunicationAngle_jungk.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef CommunicationAngle_jungk_HEADER
#define CommunicationAngle_jungk_HEADER

#include "MOOS/libMOOS/MOOSLib.h"

class CommunicationAngle_jungk : public CMOOSApp
{
 public:
   CommunicationAngle_jungk();
   ~CommunicationAngle_jungk();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();
   double ConvertDegrees(double radians) const; //converts radians to degrees
   void Calcs(); //Calculates Radius R, and angles for sperm and neptune
   void AvoidFloor(); //find another transmit area 
   double TransmissionLoss();
   std::string Transmit(double x, double y, double z); 

 private: // Configuration variables
   double const c_0          = 1480; // m/s  surface sound speed
   double const g            = 0.016; // (m/s)/m sound speed gradient
   double const water_depth  = 6000; // m
   double const pi           = 3.14159;  // pi
   double const max_depth    = 6000;
   double const angle_change = .0001; //delta theta for transmission loss calculation

 private: // State variables
   double R, x_R; //virtual circle radius and x-coordinate on the r-plane
   double range; //coordinate shift for x & y plane
   double sperm_angle; //arc angle with virtual circle
   double neptune_angle; //arc angle with virtual circle
   double circle_x;
   double neptune_x, neptune_y, neptune_z;
   double sperm_x, sperm_y, sperm_z;
   double s; //arclength
   double theta_N_0; //Neptune's declination angle
   double theta_S_0; //Spermwhale's declination angle
   double theta_S_1; //if need new angle for transmit, new Spermwhale declination angle
   double z_max; //maximum depth, bottom of the virtual circle
   double c_z0_N;//Neptune sound speed at depth
   double c_z0_S; //Spermwhale sound speed at depth
   double h1, h2; //z height distance between vehicles and center of circle
   bool commspossible;
   double base_angle, central_angle, phi;
   double R_new, x_R_new, sperm_x_new, sperm_y_new;
};

#endif 
