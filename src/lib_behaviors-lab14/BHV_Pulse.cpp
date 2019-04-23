/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_Pulse.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "BHV_Pulse.h"
#include "XYRangePulse.h"

using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_Pulse::BHV_Pulse(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Initializing variables
  m_curr_time = 0;
  m_wpt_time_old = 0;
  m_wpt_time_new = 0;
  m_wpt_index_old = 0;
  m_wpt_index_new= 0;
  m_ox = 0;
  m_oy = 0;
  m_post_time = 0;
  m_debug = true;
  m_range = 40;
  m_pulse_duration= 4;
  m_cycle_complete = 0;
  m_pulse_color="yellow";


  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y");
  addInfoVars("WPT_INDEX","no_warning");
  addInfoVars("CYCLE_COMPLETE", "no_warning");
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_Pulse::setParam(string param, string val)
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  // Get the numerical value of the param argument for convenience once
  double double_val = atof(val.c_str());
  
  if((param == "pulse_range") && isNumber(val)) {
    m_range = double_val;
    // Set local member variables here
    return(true);
  }
  else if ((param == "pulse_duration") && isNumber(val)){
    m_pulse_duration = double_val;
    return(true);
  }
  else if (param == "pulse_color"){
    m_pulse_color = tolower(val);
    return(true);
  }

  // If not handled above, then just return false;
  return(false);
}

//---------------------------------------------------------------
// Procedure: onSetParamComplete()
//   Purpose: Invoked once after all parameters have been handled.
//            Good place to ensure all required params have are set.
//            Or any inter-param relationships like a<b.

void BHV_Pulse::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_Pulse::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_Pulse::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_Pulse::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_Pulse::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_Pulse::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_Pulse::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_Pulse::onRunState()
{
  // Part 1: Build the IvP function
  IvPFunction *ipf = 0;
  bool ok = true;
  if(ok) m_ox = getBufferDoubleVal("NAV_X", ok);
  if(!ok) postEMessage("NAV_X not found.");
  if(ok) m_oy = getBufferDoubleVal("NAV_Y", ok);
  if(!ok) postEMessage("NAV_Y not found.");
  if(ok) m_curr_time = getBufferCurrTime();
  if(!ok) postEMessage("curr buffer time not found");
  //if(ok) m_cycle_complete = getBufferDoubleVal("CYCLE_COMPLETE", ok);
  /*if(ok) m_wpt_time_new = getBufferTimeVal("WPT_INDEX");
  if(m_debug) {
  	std::stringstream ss;
  	ss << "m_wpt_time_new=" << m_wpt_time_new;
  	postWMessage(ss.str());
 	 ss.str("");
 	}

  if(!ok) postEMessage("WPT_INDEX time not found"); */
  if(ok) m_wpt_index_new = getBufferDoubleVal("WPT_INDEX", ok);
  //if(!ok) postEMessage("WPT_INDEX value not found.");

  if(m_cycle_complete==1) {
    m_wpt_index_old = 0;
    m_wpt_index_new = 0;
    m_cycle_complete = 0;
  }

  if(m_wpt_index_new!=m_wpt_index_old) {
	m_post_time = m_curr_time + 5; //post time is 5 seconds later
	m_wpt_index_old = m_wpt_index_new;
	XYRangePulse pulse;
 	pulse.set_x(m_ox);
	pulse.set_y(m_oy);
	pulse.set_label("pulse");
	pulse.set_rad(m_range);
	pulse.set_duration(m_pulse_duration);
	pulse.set_time(m_post_time);
	pulse.set_color("edge", m_pulse_color);
	pulse.set_color("fill", m_pulse_color);
	string spec = pulse.get_spec();
	postMessage("VIEW_RANGE_PULSE", spec);
	}

  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}
