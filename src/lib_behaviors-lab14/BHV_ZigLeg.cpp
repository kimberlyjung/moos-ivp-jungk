/************************************************************/
/*    NAME: Kimberly Jung                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_ZigLeg.cpp                                    */
/*    DATE: 22 April 2019                                                */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "BHV_ZigLeg.h"
#include "ZAIC_PEAK.h"
#include <math.h>

using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_ZigLeg::BHV_ZigLeg(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "default_zigleg");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Initializing variables
  m_priority_wt = 201;
  m_curr_time = 0;
  m_wpt_index_old = 0;
  m_wpt_index_new= 0;
  m_ox = 0;
  m_oy = 0;
  m_post_time = 9999999;
  m_debug = true;
  m_bool_true_heading = true;
  m_zig_offset = 45;
  m_zig_time = 10;
  m_end_time = 9999999;
  m_true_heading = 0;
  m_cycle_complete = 0;


  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y");
  addInfoVars("WPT_INDEX","no_warning");
  addInfoVars("NAV_HEADING", "no_warning");
  addInfoVars("CYCLE_COMPLETE", "no_warning");
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_ZigLeg::setParam(string param, string val)
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  // Get the numerical value of the param argument for convenience once
  double double_val = atof(val.c_str());
  
  if((param == "zig_offset") && isNumber(val)) {
    m_zig_offset = double_val;
    return(true);
  }
  else if ((param == "zig_time") && isNumber(val)) {
    m_zig_time= double_val;
    return(true);
  }
  else if ((param == "pwt") && isNumber(val)){
    m_priority_wt= double_val;
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

void BHV_ZigLeg::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_ZigLeg::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_ZigLeg::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_ZigLeg::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_ZigLeg::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_ZigLeg::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_ZigLeg::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_ZigLeg::onRunState()
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
  if(ok) m_wpt_index_new = getBufferDoubleVal("WPT_INDEX", ok);
  if(ok) m_cycle_complete = getBufferDoubleVal("CYCLE_COMPLETE", ok);

  if(m_cycle_complete==1) {
  	postEmessage("CYCLE COMPLETE");
  	m_wpt_index_old = 0;
  	m_wpt_index_new = 0;
  	m_cycle_complete = 0;
  }

  if(m_wpt_index_new > m_wpt_index_old) {
  	postEMessage("new wpt index!!!!")
	m_post_time = m_curr_time + 5; //post time is 5 seconds later
	m_wpt_index_old = m_wpt_index_new;
	m_end_time = m_post_time + m_zig_time;
	m_bool_true_heading = false;
	}

  if(!m_bool_true_heading && (m_post_time <= m_curr_time)) {
	m_true_heading = getBufferDoubleVal("NAV_HEADING", ok);
	m_bool_true_heading=true;
    }

  if(m_end_time <= m_curr_time) m_bool_true_heading=false;
  if((m_curr_time >= m_post_time) && (m_curr_time <= m_end_time)) {
  	ipf = buildIvPFxnZAIC();
  }

  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}

IvPFunction *BHV_ZigLeg::buildIvPFxnZAIC()
{
  // Step 1 - Create the IvPDomain, the function's domain
  //IvPDomain domain;
  //domain.addDomain("depth", 0, 600, 601);
     
  // Step 2 - Create the ZAIC_PEAK with the domain and variable name
  ZAIC_PEAK  zaic_peak(m_domain, "course");
   
  // Step 3 - Configure the ZAIC_LEQ parameters
  zaic_peak.setSummit(m_true_heading + m_zig_offset);
  zaic_peak.setPeakWidth(0);
  zaic_peak.setBaseWidth(180);
  zaic_peak.setSummitDelta(0);
  zaic_peak.setValueWrap(true);
  if(!zaic_peak.stateOK()) {
  	string warnings = "ZAIC not working: " + zaic_peak.getWarnings();
  	postWMessage(warnings);
    return(0);
  }
  
  // Step 4 - Extract the IvP function
  IvPFunction *ivp_function = zaic_peak.extractIvPFunction();
  return(ivp_function);    
}