/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT                                             */
/*    FILE: Odometry.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef Odometry_HEADER
#define Odometry_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <vector>

class Odometry : public AppCastingMOOSApp
{
 public:
   Odometry();
   ~Odometry();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   bool buildReport();

 protected:
   void RegisterVariables();

 private: // Configuration variables

 private: // State variables - these are c++ vars
   bool   m_first_reading;
   bool   m_new_pair;
   double m_current_x;
   double m_current_y;
   double m_previous_x;
   double m_previous_y;
   double m_total_distance;
   int    m_counter_x; // keep track of the pairs of NAV_X and NAV_Y, for NAV_X
   int    m_counter_y;// keep track of the pairs of NAV_X and NAV_Y, for NAV_Y
   std::vector<double> x_vals; // keep track of x values in a new package of mail
   std::vector<double> y_vals; // keep track of y values in a new package of mail

};

#endif 
