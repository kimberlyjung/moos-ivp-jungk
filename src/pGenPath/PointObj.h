/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointObj.h                                          */
/*    DATE: 21MAR2019                                                */
/************************************************************/

#ifndef PointObj_HEADER
#define PointObj_HEADER

using namespace std;

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <cstdint>
#include <cstdlib>

class PointObj 
{
 public: 
  PointObj(); //declares the Constructor
  ~PointObj() {} //destructor
  
 public:
  // declare variable
  int x, y;
  double distance;
  
 public:
  void setDistanceFromPoint(double x_curr, double y_curr);
  

};

#endif 
