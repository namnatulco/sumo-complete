/****************************************************************************/
/// @file    Command_SaveTLCoupledLaneDet.cpp
/// @author  Daniel Krajzewicz
/// @date    15 Feb 2004
/// @version $Id$
///
// Realises the output of a lane's detector values if the lane has green light
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// copyright : (C) 2001-2007
//  by DLR (http://www.dlr.de/) and ZAIK (http://www.zaik.uni-koeln.de/AFS)
/****************************************************************************/
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include "Command_SaveTLCoupledLaneDet.h"
#include <microsim/MSNet.h>
#include <microsim/traffic_lights/MSTrafficLightLogic.h>
#include <microsim/MSEventControl.h>
#include <microsim/output/MSDetectorFileOutput.h>
#include <microsim/MSLinkCont.h>
#include <utils/common/UtilExceptions.h>
#include <utils/common/MsgHandler.h>
#include <utils/iodevices/OutputDevice.h>

#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif // CHECK_MEMORY_LEAKS


// ===========================================================================
// used namespaces
// ===========================================================================
using namespace std;


// ===========================================================================
// method definitions
// ===========================================================================
Command_SaveTLCoupledLaneDet::Command_SaveTLCoupledLaneDet(
    MSTLLogicControl::TLSLogicVariants &tlls,
    MSDetectorFileOutput *dtf,
    unsigned int begin, OutputDevice& device, MSLink *link)
        : Command_SaveTLCoupledDet(tlls, dtf, begin, device),
        myLink(link), myLastState(MSLink::LINKSTATE_TL_RED),
        myHadOne(false)
{}


Command_SaveTLCoupledLaneDet::~Command_SaveTLCoupledLaneDet()
{
}


bool
Command_SaveTLCoupledLaneDet::execute()
{
    // !!! we have to do this to have the correct information set
    myLogics.getActive()->maskRedLinks();
    if (myLink->getState()==myLastState&&myHadOne) {
        return true;
    }
    myHadOne = true;
    if (myLastState==MSLink::LINKSTATE_TL_RED&&myLink->getState()!=MSLink::LINKSTATE_TL_RED) {
        SUMOTime end = MSNet::getInstance()->getCurrentTimeStep();
        if(myStartTime!=end) {
            myDetector->writeXMLOutput(myDevice, myStartTime, end);
            myStartTime = end;
        }
    }
    if(myLink->getState()!=MSLink::LINKSTATE_TL_RED) {
        myDetector->reset();
        myStartTime = MSNet::getInstance()->getCurrentTimeStep();
    }
    myLastState = myLink->getState();
    return true;
}



/****************************************************************************/

