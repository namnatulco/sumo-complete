/****************************************************************************/
/// @file    MSDevice_BTreceiver.cpp
/// @author  Daniel Krajzewicz
/// @date    14.08.2013
/// @version $Id$
///
// A BT sender
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// Copyright (C) 2001-2013 DLR (http://www.dlr.de/) and contributors
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

#include <utils/common/TplConvert.h>
#include <utils/options/OptionsCont.h>
#include <utils/iodevices/OutputDevice.h>
#include <utils/common/SUMOVehicle.h>
#include <microsim/MSNet.h>
#include <microsim/MSLane.h>
#include <microsim/MSEdge.h>
#include <microsim/MSVehicle.h>
#include "MSDevice_Tripinfo.h"
#include "MSDevice_BTreceiver.h"

#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif // CHECK_MEMORY_LEAKS



// ===========================================================================
// static member definitions
// ===========================================================================
NamedRTree MSDevice_BTreceiver::myLanesRTree;
bool MSDevice_BTreceiver::myWasInitialised = false;


// ===========================================================================
// method definitions
// ===========================================================================
// ---------------------------------------------------------------------------
// static initialisation methods
// ---------------------------------------------------------------------------
void
MSDevice_BTreceiver::insertOptions(OptionsCont& oc) {
    insertDefaultAssignmentOptions("btreceiver", "Communication", oc);

    oc.doRegister("device.btreceiver.range", new Option_Float(300));
    oc.addDescription("device.btreceiver.range", "Communication", "The range of the bt receiver");
}


void
MSDevice_BTreceiver::buildVehicleDevices(SUMOVehicle& v, std::vector<MSDevice*>& into) {
    OptionsCont& oc = OptionsCont::getOptions();
    if (equippedByDefaultAssignmentOptions(oc, "btreceiver", v)) {
        MSDevice_BTreceiver* device = new MSDevice_BTreceiver(v, "btreceiver_" + v.getID(), oc.getFloat("device.btreceiver.range"));
        into.push_back(device);
        MSNet::getInstance()->addVehicleStateListener(device);
        if(!myWasInitialised) {
            MSLane::fill(myLanesRTree);
            myWasInitialised = true;
        }
    }
}


// ---------------------------------------------------------------------------
// MSDevice_BTreceiver-methods
// ---------------------------------------------------------------------------
MSDevice_BTreceiver::MSDevice_BTreceiver(SUMOVehicle& holder, const std::string& id,  SUMOReal range)
    : MSDevice(holder, id), myRange(range) {

}


MSDevice_BTreceiver::~MSDevice_BTreceiver() {
}


bool
MSDevice_BTreceiver::notifyMove(SUMOVehicle& veh, SUMOReal oldPos,
                                SUMOReal newPos, SUMOReal newSpeed) {
    if(!myHolder.isOnRoad()) {    
        return true;
    }
    Position p = static_cast<MSVehicle&>(veh).getPosition();
    // collect edges around
    std::set<std::string> tmp;
    Named::StoringVisitor sv(tmp);
    Boundary b;
    b.add(static_cast<MSVehicle&>(veh).getPosition());
    b.grow(myRange);
    const float cmin[2] = {(float) b.xmin(), (float) b.ymin()};
    const float cmax[2] = {(float) b.xmax(), (float) b.ymax()};
    myLanesRTree.Search(cmin, cmax, sv);

    // check vehicles in range first;
    //  determine when they've entered range
    std::set<std::string> inRange;
    for (std::set<std::string>::const_iterator i = tmp.begin(); i != tmp.end(); ++i) {
        MSLane* l = MSLane::dictionary(*i);
        if (l == 0) {
            continue;
        }

        const MSLane::VehCont& vehs = l->getVehiclesSecure();
        for (MSLane::VehCont::const_iterator j = vehs.begin(); j != vehs.end(); ++j) {
            if (static_cast<MSVehicle&>(myHolder).getPosition().distanceTo((*j)->getPosition()) > myRange) {
                continue;
            }
            if(static_cast<MSVehicle*>(*j)->getDevice(typeid(MSDevice_BTreceiver))==0) {
                // skipping not equipped vehicles
                continue;
            }
            // save, we have to investigate vehicles we do not see anymore
            inRange.insert((*j)->getID());
            // add new vehicles to myCurrentlySeen
            if (myCurrentlySeen.find((*j)->getID()) == myCurrentlySeen.end()) {
                MeetingPoint mp(MSNet::getInstance()->getCurrentTimeStep(),
                                static_cast<MSVehicle&>(myHolder).getPosition(), myHolder.getSpeed(), (*j)->getPosition(), (*j)->getSpeed());
                SeenDevice* sd = new SeenDevice(mp);
                myCurrentlySeen[(*j)->getID()] = sd;
            }
        }
        l->releaseVehicles();
    }
    // check vehicles that are not longer in range
    //  set their range exit information
    for (std::map<std::string, SeenDevice*>::const_iterator i = myCurrentlySeen.begin(); i != myCurrentlySeen.end(); ++i) {
        if (inRange.find((*i).first) != inRange.end()) {
            // skip those which are still visible
            continue;
        }
        MSVehicle* v = static_cast<MSVehicle*>(MSNet::getInstance()->getVehicleControl().getVehicle((*i).first));
        MeetingPoint mp(MSNet::getInstance()->getCurrentTimeStep(),
                        static_cast<MSVehicle&>(myHolder).getPosition(), myHolder.getSpeed(), v->getPosition(), v->getSpeed());
        myCurrentlySeen[(*i).first]->meetingEnd = mp;
        if (mySeen.find((*i).first) == mySeen.end()) {
            mySeen[(*i).first] = std::vector<SeenDevice*>();
        }
        mySeen[(*i).first].push_back(myCurrentlySeen[(*i).first]);
        myCurrentlySeen.erase(myCurrentlySeen.find((*i).first));
    }
    return true; // keep the device
}


void 
MSDevice_BTreceiver::vehicleStateChanged(const SUMOVehicle* const vehicle, MSNet::VehicleState to) {
    if(to!=MSNet::VEHICLE_STATE_STARTING_TELEPORT && to!=MSNet::VEHICLE_STATE_ARRIVED) {
        // want to know about vehicles leaving the network, only
        return;
    }
    std::string id = vehicle->getID();
    if(myCurrentlySeen.find(id)==myCurrentlySeen.end()) {
        // not seen before
        return;
    }
    const MSVehicle* v = static_cast<const MSVehicle*>(vehicle);
    MeetingPoint mp(MSNet::getInstance()->getCurrentTimeStep(),
                        static_cast<MSVehicle&>(myHolder).getPosition(), myHolder.getSpeed(), v->getPosition(), v->getSpeed());
    myCurrentlySeen[id]->meetingEnd = mp;
    if (mySeen.find(id) == mySeen.end()) {
        mySeen[id] = std::vector<SeenDevice*>();
    }
    mySeen[id].push_back(myCurrentlySeen[id]);
    myCurrentlySeen.erase(myCurrentlySeen.find(id));
}


void
MSDevice_BTreceiver::generateOutput() const {
    OutputDevice& os = OutputDevice::getDeviceByOption("bt-output");
    os.openTag("bt").writeAttr("id", myHolder.getID());
    for(std::map<std::string, std::vector<SeenDevice*> >::const_iterator i=mySeen.begin(); i!=mySeen.end(); ++i) {
        const std::vector<SeenDevice*> &sts = (*i).second;
        for(std::vector<SeenDevice*>::const_iterator j=sts.begin(); j!=sts.end(); ++j) {
            os.openTag("seen").writeAttr("id", (*i).first);
            os.writeAttr("tBeg", (*j)->meetingBegin.t)
                .writeAttr("observerPosBeg", (*j)->meetingBegin.observerPos).writeAttr("observerSpeedBeg", (*j)->meetingBegin.observerSpeed)
                .writeAttr("seenPosBeg", (*j)->meetingBegin.seenPos).writeAttr("seenSpeedBeg", (*j)->meetingBegin.seenSpeed);
            os.writeAttr("tEnd", (*j)->meetingEnd.t)
                .writeAttr("observerPosEnd", (*j)->meetingEnd.observerPos).writeAttr("observerSpeedEnd", (*j)->meetingEnd.observerSpeed)
                .writeAttr("seenPosEnd", (*j)->meetingEnd.seenPos).writeAttr("seenSpeedEnd", (*j)->meetingEnd.seenSpeed);
            os.closeTag();
        }
    }
    os.closeTag();
}



/****************************************************************************/
