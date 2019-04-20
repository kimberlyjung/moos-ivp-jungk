/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.h                                          */
/*    DATE: 23 MAR 2019                           */
/************************************************************/

#ifndef GenPath_HEADER
#define GenPath_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "XYSegList.h"
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <cstdlib>
#include <stdint.h>
#include <sstream>

using namespace std;

class GenPath : public AppCastingMOOSApp
{
 public:
   GenPath();
   ~GenPath();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables

 private: // State variables
   std::vector<std::string> m_wpts_vec; //adding all waypoints as strings from VISIT_POINTS mail
   XYSegList m_ordered_list; //ordered list by shortest distance "greedy"
   double m_least_dist;
   bool m_finished, m_all_sent;
   int m_x_curr, m_y_curr;
   int m_visit_point_count, m_WPT_update;
   int m_wvecsize;
};

#endif 
