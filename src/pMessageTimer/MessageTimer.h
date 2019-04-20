/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT                                             */
/*    FILE: MessageTimer.h                                          */
/*    Lab 11 Extra Credit: Assignment 5 Charlie_Dana_Recover */
/************************************************************/

#ifndef MessageTimer_HEADER
#define MessageTimer_HEADER
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h" 
#include "MOOS/libMOOS/MOOSLib.h"

class MessageTimer : public AppCastingMOOSApp
{
 public:
   MessageTimer();
   ~MessageTimer();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   bool buildReport();

 protected:
   void RegisterVariables();

 private: // Configuration variables

 private: // State variables
};

#endif 
