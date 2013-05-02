/****************************************************************************/
/// @file    MSAbstractLaneChangeModel.h
/// @author  Daniel Krajzewicz
/// @author  Friedemann Wesner
/// @author  Sascha Krieg
/// @author  Michael Behrisch
/// @author  Jakob Erdmann
/// @date    Fri, 29.04.2005
/// @version $Id: MSAbstractLaneChangeModel.h 13107 2012-12-02 13:57:34Z behrisch $
///
// Interface for lane-change models
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// Copyright (C) 2001-2012 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This file is part of SUMO.
//   SUMO is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
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

#include "MSAbstractLaneChangeModel.h"
#include "MSNet.h"
#include "MSEdge.h"
#include "MSLane.h"
#include "MSGlobals.h"

/* -------------------------------------------------------------------------
 * MSAbstractLaneChangeModel-methods
 * ----------------------------------------------------------------------- */

MSAbstractLaneChangeModel::MSAbstractLaneChangeModel(MSVehicle& v) : 
    myVehicle(v), 
    myOwnState(0),
    myLastLaneChangeOffset(0),
    myLaneChangeCompletion(1.0),
    myLaneChangeDirection(0),
    myLaneChangeMidpointPassed(false),
    myAlreadyMoved(false),
    myShadowLane(0),
    myHaveShadow(false),
#ifndef NO_TRACI
    myChangeRequest(MSVehicle::REQUEST_NONE),
#endif
    myCarFollowModel(v.getCarFollowModel()) {
    }


MSAbstractLaneChangeModel::~MSAbstractLaneChangeModel() { 
    //std::cout << "destructing MSAbstractLaneChangeModel of " << myVehicle.getID() << "\n";
    removeLaneChangeShadow();
}


bool 
MSAbstractLaneChangeModel::congested(const MSVehicle* const neighLeader) {
    if (neighLeader == 0) {
        return false;
    }
    // Congested situation are relevant only on highways (maxSpeed > 70km/h)
    // and congested on German Highways means that the vehicles have speeds
    // below 60km/h. Overtaking on the right is allowed then.
    if ((myVehicle.getLane()->getSpeedLimit() <= 70.0 / 3.6) || (neighLeader->getLane()->getSpeedLimit() <= 70.0 / 3.6)) {

        return false;
    }
    if (myVehicle.congested() && neighLeader->congested()) {
        return true;
    }
    return false;
}


bool 
MSAbstractLaneChangeModel::predInteraction(const MSVehicle* const leader) {
    if (leader == 0) {
        return false;
    }
    // let's check it on highways only
    if (leader->getSpeed() < (80.0 / 3.6)) {
        return false;
    }
    SUMOReal gap = leader->getPositionOnLane() - leader->getVehicleType().getLength() - myVehicle.getVehicleType().getMinGap() - myVehicle.getPositionOnLane();
    return gap < myCarFollowModel.interactionGap(&myVehicle, leader->getSpeed());
}


bool 
MSAbstractLaneChangeModel::startLaneChangeManeuver(MSLane* source, MSLane* target, int direction) {
    target->enteredByLaneChange(&myVehicle);
    if (MSGlobals::gLaneChangeDuration > DELTA_T) {
        //std::cout << getID() << " started continuous lane change\n";
        myLaneChangeCompletion = 0;
        myShadowLane = target;
        myHaveShadow = true;
        myLaneChangeMidpointPassed = false;
        myLaneChangeDirection = direction;
        continueLaneChangeManeuver(false);
        return true;
    } else {
        //std::cout << getID() << " performed instant lane change\n";
        myVehicle.leaveLane(MSMoveReminder::NOTIFICATION_LANE_CHANGE);
        source->leftByLaneChange(&myVehicle);
        myVehicle.enterLaneAtLaneChange(target);
        myLastLaneChangeOffset = 0;
        changed();
        return false;
    }
}


void 
MSAbstractLaneChangeModel::continueLaneChangeManeuver(bool moved) {
    if (moved && myHaveShadow) {
        // move shadow to next lane
        removeLaneChangeShadow();
        const int shadowDirection = myLaneChangeMidpointPassed ? -myLaneChangeDirection : myLaneChangeDirection;
        myShadowLane = myVehicle.getLane()->getParallelLane(shadowDirection);
        if (myShadowLane == 0) {
            // abort lane change
            WRITE_WARNING("Vehicle '" + myVehicle.getID() + "' could not finish continuous lane change (lane disappeared) time=" + 
                    time2string(MSNet::getInstance()->getCurrentTimeStep()) + ".");
            endLaneChangeManeuver();
            return;
        }
        myHaveShadow = true;
        //std::cout << time2string(MSNet::getInstance()->getCurrentTimeStep())
        //    << " " << myVehicle.getID() << " new shadow lane: " << myShadowLane->getID() << " completion=" << myLaneChangeCompletion << "\n";
    }
    //std::cout << time2string(MSNet::getInstance()->getCurrentTimeStep())
    //    << " " << myVehicle.getID() << " continueLaneChangeManeuver myLane=" << myVehicle.getLane()->getID() << " completion=" << myLaneChangeCompletion << "\n";
    myLaneChangeCompletion += (SUMOReal)DELTA_T / (SUMOReal)MSGlobals::gLaneChangeDuration;
    //std::cout << getID() << " continues lane change (completion=" << myLaneChangeCompletion << ")\n";
    if (!myLaneChangeMidpointPassed && myLaneChangeCompletion >= 
            myVehicle.getLane()->getWidth() / (myVehicle.getLane()->getWidth() + myShadowLane->getWidth())) {
        //std::cout << "     midpoint reached\n";
        // maneuver midpoint reached, swap myLane and myShadowLane
        myLaneChangeMidpointPassed = true;
        MSLane* tmp = myVehicle.getLane();
        //std::cout << getID() << " before myLane=" << myLane->getID() << " myCurrentLaneInBestLanes=" << (*myCurrentLaneInBestLanes).lane->getID() << "\n";
        myVehicle.leaveLane(MSMoveReminder::NOTIFICATION_LANE_CHANGE);
        myVehicle.enterLaneAtLaneChange(myShadowLane);
        myShadowLane = tmp;
        //std::cout << myVehicle.getID() << " after enterLaneAtLaneChange myLane=" << myVehicle.getLane()->getID() << "\n";
        if (myVehicle.getLane()->getEdge().getPurpose() == MSEdge::EDGEFUNCTION_INTERNAL) {
            // internal lanes do not appear in bestLanes so we need to update
            // myCurrentLaneInBestLanes explicitly
            myVehicle.getBestLanes(false, myVehicle.getLane()->getLogicalPredecessorLane());
            //std::cout << getID() << " after manual update myCurrentLaneInBestLanes=" << (*myCurrentLaneInBestLanes).lane->getID() << "\n";
            if (myVehicle.fixContinuations()) {
                WRITE_WARNING("vehicle '" + myVehicle.getID() + "' could not reconstruct bestLanes when changing lanes on lane '" + myVehicle.getLane()->getID() + " time=" 
                        + time2string(MSNet::getInstance()->getCurrentTimeStep()) + ".");
            }
        }
        if (myVehicle.fixPosition()) {
            WRITE_WARNING("vehicle '" + myVehicle.getID() + "' set back by " + toString(myVehicle.getPositionOnLane() - myVehicle.getLane()->getLength()) + 
                    "m when changing lanes on lane '" + myVehicle.getLane()->getID() + " time=" + 
                    time2string(MSNet::getInstance()->getCurrentTimeStep()) + ".");
        }
        //std::cout << "after leaveLane myCurrentLaneInBestLanes=" << (*myCurrentLaneInBestLanes).lane->getID() << "\n";
        myLastLaneChangeOffset = 0;
        changed();
        myAlreadyMoved = true;
    } 
    // remove shadow as soon as the vehicle leaves the original lane geometrically
    if (myLaneChangeMidpointPassed && myHaveShadow) {
        const SUMOReal sourceHalfWidth = myShadowLane->getWidth() / 2.0;
        const SUMOReal targetHalfWidth = myVehicle.getLane()->getWidth() / 2.0;
        if (myLaneChangeCompletion * (sourceHalfWidth + targetHalfWidth) - myVehicle.getVehicleType().getWidth() / 2.0 > sourceHalfWidth) {
            //std::cout << " removing shadow of " << myVehicle.getID() << " at completion " << myLaneChangeCompletion << "\n";
            removeLaneChangeShadow();
        }
    }
    // finish maneuver
    if (!isChangingLanes()) {
        //std::cout << "     finished\n";
        assert(myLaneChangeMidpointPassed);
        endLaneChangeManeuver();
    }
}


void 
MSAbstractLaneChangeModel::removeLaneChangeShadow() {
    //std::cout << time2string(MSNet::getInstance()->getCurrentTimeStep())
    //    << " " << myVehicle.getID() << " trying to remove shadow for vehicle on lane=" << myVehicle.getLane()->getID() << " completion=" << myLaneChangeCompletion << "\n";
    if (myShadowLane != 0 && myHaveShadow) {
    //    std::cout << time2string(MSNet::getInstance()->getCurrentTimeStep())
    //        << " " << myVehicle.getID() << " removed shadow from lane=" << myShadowLane->getID() << " completion=" << myLaneChangeCompletion << "\n";
        myShadowLane->removeVehicle(&myVehicle, MSMoveReminder::NOTIFICATION_LANE_CHANGE);
        myHaveShadow = false;
    }
}
