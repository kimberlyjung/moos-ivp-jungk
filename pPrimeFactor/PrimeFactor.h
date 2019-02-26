/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.h                                          */
/*    DATE: 23FEB2019                                    */
/************************************************************/

#ifndef PrimeFactor_HEADER
#define PrimeFactor_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "PrimeObj.h"

class PrimeFactor : public CMOOSApp
{
 public:
   PrimeFactor();
   ~PrimeFactor();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   string Print(PrimeObj x);

 protected:
   void RegisterVariables();

 private: // Configuration variables

 private: // State variables

   list<PrimeObj> list_allnums;
   int counter; //keeps track of how many pieces of mail have been received and increments
   int order; //keeps track of order of processed pieces of mail
   bool debug; //debugger
};

#endif 
