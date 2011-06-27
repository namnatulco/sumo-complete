/****************************************************************************/
/// @file    NBLoadedSUMOTLDef.h
/// @author  Jakob Erdmann
/// @date    Mar 2011
/// @version $Id$
///
// A complete traffic light logic loaded from a sumo-net. (opted to reimplement
// since NBLoadedTLDef is quite vissim specific)
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// Copyright (C) 2001-2011 DLR (http://www.dlr.de/) and contributors
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

#include <vector>
#include <set>
#include <cassert>
#include <iterator>
#include <utils/common/MsgHandler.h>
#include <utils/common/ToString.h>
#include <utils/options/OptionsCont.h>
#include "NBTrafficLightLogic.h"
#include "NBTrafficLightDefinition.h"
#include "NBLoadedSUMOTLDef.h"
#include "NBNode.h"

#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif // CHECK_MEMORY_LEAKS


// ===========================================================================
// method definitions
// ===========================================================================

NBLoadedSUMOTLDef::NBLoadedSUMOTLDef(const std::string &id, const std::string &programID, SUMOTime offset) throw() : 
    NBTrafficLightDefinition(id, programID),
    myTLLogic(0),
    myAmComputed(false)
{
    // the third parameter noLinks is never used within
    // NBTrafficLightLogic
    myTLLogic = new NBTrafficLightLogic(id, programID, -1);
    myTLLogic->setOffset(offset);
}


NBLoadedSUMOTLDef::~NBLoadedSUMOTLDef() throw() 
{
    if (!myAmComputed) {
        delete myTLLogic;
    }
}


NBTrafficLightLogic *
NBLoadedSUMOTLDef::myCompute(const NBEdgeCont &ec, unsigned int brakingTime) throw() {
    // @todo what to do with those parameters?
    UNUSED_PARAMETER(ec);
    UNUSED_PARAMETER(brakingTime);
    myTLLogic->closeBuilding();
    myAmComputed = true;
    return myTLLogic;
}


void 
NBLoadedSUMOTLDef::addConnection(NBEdge *from, NBEdge *to, int fromLane, int toLane, int linkno) {
    // maybe there is more to it but we have to start somewhere
    from->setControllingTLInformation(fromLane, to, toLane, getID(), linkno);
    addNode(from->getToNode());
    addNode(to->getFromNode());
}


void
NBLoadedSUMOTLDef::setTLControllingInformation(const NBEdgeCont &ec) const throw() {
    UNUSED_PARAMETER(ec);
}


void
NBLoadedSUMOTLDef::remapRemoved(NBEdge*, const EdgeVector&, const EdgeVector&) throw() {}


void
NBLoadedSUMOTLDef::replaceRemoved(NBEdge*, int, NBEdge*, int) throw() {}


void 
NBLoadedSUMOTLDef::addPhase(SUMOTime duration, const std::string &state) {
    myTLLogic->addStep(duration, state);
}

/****************************************************************************/

