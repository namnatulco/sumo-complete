//---------------------------------------------------------------------------//
//                        Command_SaveTLCoupledDet.cpp -
//  Realises the output of a tls values on each switch
//                           -------------------
//  project              : SUMO - Simulation of Urban MObility
//  begin                : 15 Feb 2004
//  copyright            : (C) 2004 by Daniel Krajzewicz
//  organisation         : IVF/DLR http://ivf.dlr.de
//  email                : Daniel.Krajzewicz@dlr.de
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//---------------------------------------------------------------------------//
namespace
{
    const char rcsid[] =
    "$Id$";
}
// $Log$
// Revision 1.2  2004/02/16 14:02:57  dkrajzew
// e2-link-dependent detectors added
//
//
/* =========================================================================
 * included modules
 * ======================================================================= */
#include "Action.h"
#include "Command_SaveTLCoupledDet.h"
#include <microsim/MSNet.h>
#include <microsim/MSTrafficLightLogic.h>
#include <microsim/MSEventControl.h>
#include <microsim/MSDetectorFileOutput.h>
#include <utils/common/UtilExceptions.h>
#include <utils/common/MsgHandler.h>


/* =========================================================================
 * used namespaces
 * ======================================================================= */
using namespace std;


/* =========================================================================
 * method definitions
 * ======================================================================= */
Command_SaveTLCoupledDet::Command_SaveTLCoupledDet(MSTrafficLightLogic *tll,
                                                   MSDetectorFileOutput *dtf,
                                                   unsigned int begin,
                                                   const std::string &file)
    : myLogic(tll), myDetector(dtf), myFile(file.c_str()),
    myStartTime(begin)
{
    if(!myFile.good()) {
        MsgHandler::getErrorInstance()->inform(
            string("The file '") + file
            + string("'to save the tl-states into could not be opened."));
        throw ProcessError();
    }
    myFile << myDetector->getXMLHeader()
        << myDetector->getXMLDetectorInfoStart() << endl;
    tll->addSwitchAction(this);
}


Command_SaveTLCoupledDet::~Command_SaveTLCoupledDet()
{
    myFile << myDetector->getXMLDetectorInfoEnd() << endl;
}


bool
Command_SaveTLCoupledDet::execute()
{
    unsigned int end =
        MSNet::getInstance()->getCurrentTimeStep();
    myFile << "<interval start=\"" << myStartTime << "\" stop=\"" << end
        << "\" " << myDetector->getXMLOutput( end-myStartTime )
        << " />" << endl;
    myStartTime = end;
    return true;
}


/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

// Local Variables:
// mode:C++
// End:



