/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointObj.cpp                                         */
/*    DATE: 21MAR2019                                                */
/************************************************************/

#include "PointObj.h"

using namespace std;

PointObj::PointObj() { //Constructor
  x=0;
  y=0;
  distance=0;
}

void PointObj::setDistanceFromPoint(double x_curr, double y_curr) {
  distance=pow((pow(x_curr-x,2)+pow(y_curr-y,2)),0.5);
}
