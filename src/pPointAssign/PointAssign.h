/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT, 2.680, Lab 9                                   */
/*    FILE: PointsAssign.h                                          */
/*    DATE: 19MAR2019                             */
/************************************************************/

#ifndef PointAssign_HEADER
#define PointAssign_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "MOOS/libMOOS/MOOSLib.h"
#include <vector>
#include <iterator>
#include <cstdlib>
#include <stdint.h>
#include "MBUtils.h"
#include "XYPoint.h"

using namespace std;

class PointAssign : public AppCastingMOOSApp
{
 public:
   PointAssign();
   ~PointAssign();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   void	postViewPoint(double x, double y, std::string label, std::string color);

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables

 private: // State variables
   vector<string> m_vector;
   bool m_finished, m_sent_all;
   bool m_assign_by_region;

   //debug for buildReport
   unsigned int gilda_count, henry_count;


};

#endif 
