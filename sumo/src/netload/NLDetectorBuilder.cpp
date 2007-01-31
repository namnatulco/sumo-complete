/***************************************************************************
                          NLDetectorBuilder.cpp
                          A building helper for the detectors
                             -------------------
    begin                : Mon, 15 Apr 2002
    copyright            : (C) 2001 by DLR http://ivf.dlr.de/
    author               : Daniel Krajzewicz
    email                : Daniel.Krajzewicz@dlr.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
namespace
{
     const char rcsid[] =
         "$Id$";
}
// $Log$
// Revision 1.38  2006/11/16 12:30:54  dkrajzew
// warnings removed
//
// Revision 1.37  2006/11/14 13:04:12  dkrajzew
// warnings removed
//
// Revision 1.36  2006/08/01 05:54:35  dkrajzew
// E3 detectors refactored partially
//
// Revision 1.35  2006/07/06 12:22:32  dkrajzew
// debugged construction of e3-detectors
//
// Revision 1.34  2006/05/15 05:47:50  dkrajzew
// got rid of the cell-to-meter conversions
//
// Revision 1.34  2006/05/08 11:14:27  dkrajzew
// got rid of the cell-to-meter conversions
//
// Revision 1.33  2006/03/08 13:15:00  dkrajzew
// friendly_pos usage debugged
//
// Revision 1.32  2006/02/27 12:09:49  dkrajzew
// variants container named properly
//
// Revision 1.31  2006/02/23 11:27:57  dkrajzew
// tls may have now several programs
//
// Revision 1.30  2006/02/13 07:22:20  dkrajzew
// detector position may now be "friendly"
//
// Revision 1.29  2005/11/09 06:42:54  dkrajzew
// TLS-API: MSEdgeContinuations added
//
// Revision 1.28  2005/10/07 11:41:49  dkrajzew
// THIRD LARGE CODE RECHECK: patched problems on Linux/Windows configs
//
// Revision 1.27  2005/09/23 06:04:11  dkrajzew
// SECOND LARGE CODE RECHECK: converted doubles and floats to SUMOReal
//
// Revision 1.26  2005/09/15 12:04:36  dkrajzew
// LARGE CODE RECHECK
//
// Revision 1.25  2005/07/12 12:36:11  dkrajzew
// made errors on detector building more readable
//
// Revision 1.24  2005/05/04 08:39:45  dkrajzew
// level 3 warnings removed; a certain SUMOTime time description added
//
// Revision 1.23  2004/12/16 12:23:03  dkrajzew
// got rid of an unnecessary detector parameter/debugging
//
// Revision 1.22  2004/11/23 10:12:45  dkrajzew
// new detectors usage applied
//
// Revision 1.21  2004/04/02 11:23:51  dkrajzew
// extended traffic lights are now no longer templates;
//  MSNet now handles all simulation-wide output
//
// Revision 1.20  2004/02/18 05:32:51  dkrajzew
// missing pass of lane continuation to detector builder added
//
// Revision 1.19  2004/02/16 13:49:08  dkrajzew
// loading of e2-link-dependent detectors added
//
// Revision 1.18  2004/02/06 09:02:39  dkrajzew
// false detector positioning when negative values are used debugged
//
// Revision 1.17  2004/01/26 11:06:54  dkrajzew
// position setting reapplied
//
// Revision 1.16  2004/01/26 07:07:36  dkrajzew
// work on detectors: e3-detectors loading and visualisation;
//  variable offsets and lengths for lsa-detectors;
//  coupling of detectors to tl-logics;
//  different detector visualistaion in dependence to his controller
//
// Revision 1.15  2004/01/13 14:28:46  dkrajzew
// added alternative detector description; debugging
//
// Revision 1.14  2004/01/12 14:46:21  dkrajzew
// handling of e2-detectors within the gui added
//
// Revision 1.13  2004/01/12 14:37:32  dkrajzew
// reading of e2-detectors from files added
//
// Revision 1.12  2003/11/11 08:05:45  dkrajzew
// logging (value passing) moved from utils to microsim
//
// Revision 1.11  2003/09/22 12:29:36  dkrajzew
// construction using two sample intervals inserted (using one by now)
//
// Revision 1.10  2003/08/14 13:52:34  dkrajzew
// new building scheme applied
//
// Revision 1.9  2003/08/04 11:35:52  dkrajzew
// only GUIVehicles need a color definition; process of building cars changed
//
// Revision 1.8  2003/07/22 15:12:16  dkrajzew
// new usage of detectors applied
//
// Revision 1.7  2003/07/21 18:07:44  roessel
// Adaptions due to new MSInductLoop.
//
// Revision 1.6  2003/07/18 12:35:05  dkrajzew
// removed some warnings
//
// Revision 1.5  2003/03/18 15:00:32  roessel
// Changed Loggedvalue to LoggedValue in
//
// Revision 1.4  2003/03/17 14:24:30  dkrajzew
// windows eol removed
//
// Revision 1.3  2003/03/03 15:06:33  dkrajzew
// new import format applied; new detectors applied
//
// Revision 1.2  2003/02/07 11:18:56  dkrajzew
// updated
//
// Revision 1.1  2002/10/16 15:36:50  dkrajzew
// moved from ROOT/sumo/netload to ROOT/src/netload; new format definition
//  parseable in one step
//
// Revision 1.8  2002/07/31 17:34:50  roessel
// Changes since sourceforge cvs request.
//
// Revision 1.9  2002/07/26 10:49:41  dkrajzew
// Detector-output destination may now be specified using relative pathnames
//
// Revision 1.8  2002/07/22 12:44:32  dkrajzew
// Source loading structures added
//
// Revision 1.7  2002/06/11 14:39:26  dkrajzew
// windows eol removed
//
// Revision 1.6  2002/06/11 13:44:32  dkrajzew
// Windows eol removed
//
// Revision 1.5  2002/06/07 14:39:58  dkrajzew
// errors occured while building larger nets and adaption of new netconverting
// methods debugged
//
// Revision 1.4  2002/04/17 11:17:01  dkrajzew
// windows-newlines removed
//
// Revision 1.3  2002/04/16 06:50:20  dkrajzew
// documentation added; coding standard attachements added
//
/* =========================================================================
 * compiler pragmas
 * ======================================================================= */
#pragma warning(disable: 4786)


/* =========================================================================
 * included modules
 * ======================================================================= */
#ifdef HAVE_CONFIG_H
#ifdef WIN32
#include <windows_config.h>
#else
#include <config.h>
#endif
#endif // HAVE_CONFIG_H

#include <string>
#include <iostream>
#include <microsim/MSNet.h>
#include <microsim/output/MSInductLoop.h>
#include <microsim/output/e2_detectors/MSE2Collector.h>
#include <microsim/output/e2_detectors/MS_E2_ZS_CollectorOverLanes.h>
#include <microsim/output/MSDetector2File.h>
#include <microsim/MSGlobals.h>
#include <microsim/actions/Command_SaveTLCoupledDet.h>
#include <microsim/actions/Command_SaveTLCoupledLaneDet.h>
#include <utils/common/UtilExceptions.h>
#include <utils/common/FileHelpers.h>
#include <utils/common/StringUtils.h>
#include <utils/common/UtilExceptions.h>
#include "NLDetectorBuilder.h"
#include <microsim/output/MSDetectorControl.h>

#ifdef HAVE_MESOSIM
#include <mesosim/MEInductLoop.h>
#include <mesosim/MELoop.h>
#endif

#ifdef _DEBUG
#include <utils/dev/debug_new.h>
#endif // _DEBUG


/* =========================================================================
 * used namespaces
 * ======================================================================= */
using namespace std;


/* =========================================================================
 * method definitions
 * ======================================================================= */
/* -------------------------------------------------------------------------
 * NLDetectorBuilder::E3DetectorDefinition-methods
 * ----------------------------------------------------------------------- */
NLDetectorBuilder::E3DetectorDefinition::E3DetectorDefinition(
        const std::string &id,
        OutputDevice *device,
        MSUnit::Seconds haltingTimeThreshold,
        MSUnit::MetersPerSecond haltingSpeedThreshold,
        SUMOTime deleteDataAfterSeconds,
        const E3MeasuresVector &measures,
        int splInterval) :
    myID(id), myDevice(device),
    myHaltingTimeThreshold(haltingTimeThreshold),
    myHaltingSpeedThreshold(haltingSpeedThreshold),
    myDeleteDataAfterSeconds(deleteDataAfterSeconds),
    myMeasures(measures), mySampleInterval(splInterval)
{
}


NLDetectorBuilder::E3DetectorDefinition::~E3DetectorDefinition()
{
}


/* -------------------------------------------------------------------------
 * NLDetectorBuilder-methods
 * ----------------------------------------------------------------------- */
NLDetectorBuilder::NLDetectorBuilder(MSNet &net)
    : myNet(net), myE3Definition(0)
{
}


NLDetectorBuilder::~NLDetectorBuilder()
{
}


void
NLDetectorBuilder::buildInductLoop(const std::string &id,
        const std::string &lane, SUMOReal pos, int splInterval,
        OutputDevice *device, bool friendly_pos,
		const std::string &/*style*/)
{
    // get and check the lane
    MSLane *clane = getLaneChecking(lane, id);
    if(pos<0) {
        pos = clane->length() + pos;
    }
#ifdef HAVE_MESOSIM
    if(!MSGlobals::gUseMesoSim) {
#endif
     // get the output style
//   MSDetector::OutputStyle cstyle = convertStyle(id, style);
     // check whether the file must be converted into a relative path
    // compute position
    if(pos>clane->length()) {
		if(friendly_pos) {
			pos = clane->length() - (SUMOReal) 0.1;
		} else {
			throw InvalidArgument("The position of detector '" + id + "' lies beyond the lane's '" + lane + "' length.");
		}
    }
    // build the loop
    MSInductLoop *loop = createInductLoop(id, clane, pos, splInterval);
    // add the file output
    myNet.getDetectorControl().add(loop, device, splInterval);
#ifdef HAVE_MESOSIM
    } else {
        MESegment *s = MSGlobals::gMesoNet->getSegmentForEdge(clane->getEdge());
        MESegment *prev = s;
        SUMOReal cpos = 0;
        while(cpos+prev->getLength()<pos&&s!=0) {
            prev = s;
            cpos += s->getLength();
            s = s->getNextSegment();
        }
        SUMOReal rpos = pos-cpos;//-prev->getLength();
        if(rpos>prev->getLength()||rpos<0) {
		    if(friendly_pos) {
			    rpos = prev->getLength() - (SUMOReal) 0.1;
		    } else {
			    throw InvalidArgument("The position of detector '" + id + "' lies beyond the lane's '" + lane + "' length.");
		    }
        }
        MEInductLoop *loop =
            createMEInductLoop(id, prev, rpos, splInterval);
        myNet.getDetectorControl().add(loop, device, splInterval);
    }
#endif
}


void
NLDetectorBuilder::buildE2Detector(const MSEdgeContinuations &edgeContinuations,
        const std::string &id,
        const std::string &lane, SUMOReal pos, SUMOReal length,
        bool cont, int splInterval,
        const std::string &/*style*/,
        OutputDevice *device,
        const std::string &measures,
        SUMOReal haltingTimeThreshold,
        MSUnit::MetersPerSecond haltingSpeedThreshold,
        SUMOReal jamDistThreshold,
        SUMOTime deleteDataAfterSeconds )
{
    MSLane *clane = getLaneChecking(lane, id);
    // check whether the detector may lie over more than one lane
    MSDetectorFileOutput *det = 0;
    if(!cont) {
        convUncontE2PosLength(id, clane, pos, length);
        det = buildSingleLaneE2Det(id, DU_USER_DEFINED,
            clane, pos, length,
            haltingTimeThreshold, haltingSpeedThreshold,
            jamDistThreshold, deleteDataAfterSeconds,
            measures);
        myNet.getDetectorControl().add(
            static_cast<MSE2Collector*>(det), device, splInterval);
    } else {
        convContE2PosLength(id, clane, pos, length);
        det = buildMultiLaneE2Det(edgeContinuations, id, DU_USER_DEFINED,
            clane, pos, length,
            haltingTimeThreshold, haltingSpeedThreshold,
            jamDistThreshold, deleteDataAfterSeconds,
            measures);
        myNet.getDetectorControl().add(
            static_cast<MS_E2_ZS_CollectorOverLanes*>(det), device, splInterval);
    }
}


void
NLDetectorBuilder::buildE2Detector(const MSEdgeContinuations &edgeContinuations,
        const std::string &id,
        const std::string &lane, SUMOReal pos, SUMOReal length,
        bool cont,
        const MSTLLogicControl::TLSLogicVariants &tlls,
        const std::string &/*style*/, OutputDevice *device,
        const std::string &measures,
        SUMOReal haltingTimeThreshold,
        MSUnit::MetersPerSecond haltingSpeedThreshold,
        SUMOReal jamDistThreshold,
        SUMOTime deleteDataAfterSeconds )
{
    MSLane *clane = getLaneChecking(lane, id);
    // check whether the detector may lie over more than one lane
    MSDetectorFileOutput *det = 0;
    if(!cont) {
        convUncontE2PosLength(id, clane, pos, length);
        det = buildSingleLaneE2Det(id, DU_USER_DEFINED,
            clane, pos, length,
            haltingTimeThreshold, haltingSpeedThreshold,
            jamDistThreshold, deleteDataAfterSeconds,
            measures);
        myNet.getDetectorControl().add(
            static_cast<MSE2Collector*>(det));
    } else {
        convContE2PosLength(id, clane, pos, length);
        det = buildMultiLaneE2Det(edgeContinuations, id, DU_USER_DEFINED,
            clane, pos, length,
            haltingTimeThreshold, haltingSpeedThreshold,
            jamDistThreshold, deleteDataAfterSeconds,
            measures);
        myNet.getDetectorControl().add(
            static_cast<MS_E2_ZS_CollectorOverLanes*>(det));
    }
    // add the file output
    new Command_SaveTLCoupledDet(tlls, det,
        myNet.getCurrentTimeStep(), device);
}


void
NLDetectorBuilder::buildE2Detector(const MSEdgeContinuations &edgeContinuations,
        const std::string &id,
        const std::string &lane, SUMOReal pos, SUMOReal length,
        bool cont,
        const MSTLLogicControl::TLSLogicVariants &tlls,
        const std::string &tolane,
        const std::string &/*style*/, OutputDevice *device,
        const std::string &measures,
        SUMOReal haltingTimeThreshold,
        MSUnit::MetersPerSecond haltingSpeedThreshold,
        SUMOReal jamDistThreshold,
        SUMOTime deleteDataAfterSeconds )
{
    MSLane *clane = getLaneChecking(lane, id);
    MSLane *ctoLane = getLaneChecking(tolane, id);
    MSLink *link = MSLinkContHelper::getConnectingLink(*clane, *ctoLane);
    if(link==0) {
        throw InvalidArgument(
            "The detector output can not be build as no connection between lanes '"
            + lane + "' and '" + tolane + "' exists.");
    }
    // check whether the detector may lie over more than one lane
    MSDetectorFileOutput *det = 0;
    if(!cont) {
        convUncontE2PosLength(id, clane, pos, length);
        det = buildSingleLaneE2Det(id, DU_USER_DEFINED,
            clane, pos, length,
            haltingTimeThreshold, haltingSpeedThreshold,
            jamDistThreshold, deleteDataAfterSeconds,
            measures);
        myNet.getDetectorControl().add(
            static_cast<MSE2Collector*>(det));
    } else {
        convContE2PosLength(id, clane, pos, length);
        det = buildMultiLaneE2Det(edgeContinuations, id, DU_USER_DEFINED,
            clane, pos, length,
            haltingTimeThreshold, haltingSpeedThreshold,
            jamDistThreshold, deleteDataAfterSeconds,
            measures);
        myNet.getDetectorControl().add(
            static_cast<MS_E2_ZS_CollectorOverLanes*>(det));
    }
    // add the file output
    new Command_SaveTLCoupledLaneDet(tlls, det,
        myNet.getCurrentTimeStep(), device, link);
}


void
NLDetectorBuilder::convUncontE2PosLength(const std::string &id,
                                         MSLane *clane,
                                         SUMOReal pos,
                                         SUMOReal length)
{
    if(pos<0) {
        pos = clane->length() + pos;
    }
    // compute length
    if(length<0) {
        pos = pos + length;
        length *= -1;
    }
    // patch position
    if(pos<0.1) {
        pos = (SUMOReal) 0.1;
    }
    // patch length
    if(pos+length>clane->length()-(SUMOReal) 0.1) {
        length = clane->length() - (SUMOReal) 0.1 - pos;
    }
    if(length<=0) {
        throw InvalidArgument("The length of detector '" + id + "' is not positive.");
    }
}


void
NLDetectorBuilder::convContE2PosLength(const std::string &id,
                                       MSLane * /*clane*/,
                                       SUMOReal pos,
                                       SUMOReal length)
{
    if(pos<0) {
        pos *= -1.0;//clane->length() + pos;
    }
    if(length<=0) {
        throw InvalidArgument("The length of the continuated detector " + id + " is not positive.");
    }
}


void
NLDetectorBuilder::beginE3Detector(const std::string &id,
        OutputDevice *device, int splInterval,
        const std::string &measures,
        SUMOReal /*haltingTimeThreshold*/,
        MSUnit::MetersPerSecond haltingSpeedThreshold,
        SUMOTime deleteDataAfterSeconds)
{
    E3MeasuresVector toAdd = parseE3Measures(measures);
    myE3Definition = new E3DetectorDefinition(id, device,
        haltingSpeedThreshold, haltingSpeedThreshold, deleteDataAfterSeconds,
        toAdd, splInterval);
}


void
NLDetectorBuilder::addE3Entry(const std::string &lane,
                              SUMOReal pos)
{
    MSLane *clane = getLaneChecking(lane, myE3Definition->myID);
    if(myE3Definition==0) {
        throw InvalidArgument("Something is wrong with a detector description.");
    }
    if(pos<0) {
        pos = clane->length() + pos;
    }
    myE3Definition->myEntries.push_back(MSCrossSection(clane, pos));
}


void
NLDetectorBuilder::addE3Exit(const std::string &lane,
                             SUMOReal pos)
{
    MSLane *clane = getLaneChecking(lane, myE3Definition->myID);
    if(myE3Definition==0) {
        throw InvalidArgument("Something is wrong with a detector description.");
    }
    if(pos<0) {
        pos = clane->length() + pos;
    }
    myE3Definition->myExits.push_back(MSCrossSection(clane, pos));
}


void
NLDetectorBuilder::endE3Detector()
{
    if(myE3Definition==0) {
        throw InvalidArgument("Something is wrong with a detector description.");
    }
    MSE3Collector *det = createE3Detector(
        myE3Definition->myID,
        myE3Definition->myEntries,
        myE3Definition->myExits,
        myE3Definition->myHaltingTimeThreshold,
        myE3Definition->myHaltingSpeedThreshold,
        myE3Definition->myDeleteDataAfterSeconds);
    E3MeasuresVector &toAdd = myE3Definition->myMeasures;
    for(E3MeasuresVector::iterator i=toAdd.begin(); i!=toAdd.end(); i++) {
        det->addDetector(*i);
    }
    // add to net
    myNet.getDetectorControl().add(
        static_cast<MSE3Collector*>(det), myE3Definition->myDevice, myE3Definition->mySampleInterval);
    // clean up
    delete myE3Definition;
    myE3Definition = 0;
}

// -------------------

MSE2Collector *
NLDetectorBuilder::buildSingleLaneE2Det(const std::string &id,
        DetectorUsage usage,
        MSLane *lane, SUMOReal pos, SUMOReal length,
        SUMOReal haltingTimeThreshold,
        MSUnit::MetersPerSecond haltingSpeedThreshold,
        SUMOReal jamDistThreshold,
        SUMOTime deleteDataAfterSeconds,
        const std::string &measures)
{
    MSE2Collector *ret = createSingleLaneE2Detector(id, usage, lane, pos,
        length, haltingTimeThreshold, haltingSpeedThreshold,
        jamDistThreshold, deleteDataAfterSeconds);
    E2MeasuresVector toAdd = parseE2Measures(measures);
    for(E2MeasuresVector::iterator i=toAdd.begin(); i!=toAdd.end(); i++) {
        ret->addDetector(*i);
    }
    return ret;
}


MS_E2_ZS_CollectorOverLanes *
NLDetectorBuilder::buildMultiLaneE2Det(const MSEdgeContinuations &edgeContinuations,
            const std::string &id, DetectorUsage usage,
            MSLane *lane, SUMOReal pos, SUMOReal length,
            SUMOReal haltingTimeThreshold,
            MSUnit::MetersPerSecond haltingSpeedThreshold,
            SUMOReal jamDistThreshold ,
            SUMOTime deleteDataAfterSeconds,
            const std::string &measures)
{
    MS_E2_ZS_CollectorOverLanes *ret = createMultiLaneE2Detector(id, usage,
        lane, pos, haltingTimeThreshold, haltingSpeedThreshold,
        jamDistThreshold, deleteDataAfterSeconds);
    ret->init(lane, length, edgeContinuations);
    E2MeasuresVector toAdd = parseE2Measures(measures);
    for(E2MeasuresVector::iterator i=toAdd.begin(); i!=toAdd.end(); i++) {
        ret->addDetector(*i);
    }
    return ret;
}




NLDetectorBuilder::E2MeasuresVector
NLDetectorBuilder::parseE2Measures(const std::string &measures)
{
    string my = measures;
    StringUtils::upper(my);
    E2MeasuresVector ret;
    if(my.find("DENSITY")!=string::npos) {
        ret.push_back(E2::DENSITY);
    }
    if(my.find("MAX_JAM_LENGTH_IN_VEHICLES")!=string::npos) {
        ret.push_back(E2::MAX_JAM_LENGTH_IN_VEHICLES);
    }
    if(my.find("MAX_JAM_LENGTH_IN_METERS")!=string::npos) {
        ret.push_back(E2::MAX_JAM_LENGTH_IN_METERS);
    }
    if(my.find("JAM_LENGTH_SUM_IN_VEHICLES")!=string::npos) {
        ret.push_back(E2::JAM_LENGTH_SUM_IN_VEHICLES);
    }
    if(my.find("JAM_LENGTH_SUM_IN_METERS")!=string::npos) {
        ret.push_back(E2::JAM_LENGTH_SUM_IN_METERS);
    }
    if(my.find("QUEUE_LENGTH_AHEAD_OF_TRAFFIC_LIGHTS_IN_VEHICLES")!=string::npos) {
        ret.push_back(E2::QUEUE_LENGTH_AHEAD_OF_TRAFFIC_LIGHTS_IN_VEHICLES);
    }
    if(my.find("QUEUE_LENGTH_AHEAD_OF_TRAFFIC_LIGHTS_IN_METERS")!=string::npos) {
        ret.push_back(E2::QUEUE_LENGTH_AHEAD_OF_TRAFFIC_LIGHTS_IN_METERS);
    }
    if(my.find("N_VEHICLES")!=string::npos) {
        ret.push_back(E2::N_VEHICLES);
    }
    if(my.find("OCCUPANCY_DEGREE")!=string::npos) {
        ret.push_back(E2::OCCUPANCY_DEGREE);
    }
    if(my.find("SPACE_MEAN_SPEED")!=string::npos) {
        ret.push_back(E2::SPACE_MEAN_SPEED);
    }
    if(my.find("CURRENT_HALTING_DURATION_SUM_PER_VEHICLE")!=string::npos) {
        ret.push_back(E2::CURRENT_HALTING_DURATION_SUM_PER_VEHICLE);
    }
    if(my.find("N_STARTED_HALTS")!=string::npos) {
        ret.push_back(E2::N_STARTED_HALTS);
    }
    if(my.find("HALTING_DURATION_SUM")!=string::npos) {
        ret.push_back(E2::HALTING_DURATION_SUM);
    }
    if(my.find("HALTING_DURATION_MEAN")!=string::npos) {
        ret.push_back(E2::HALTING_DURATION_MEAN);
    }
    if(my.find("APPROACHING_VEHICLES_STATES")!=string::npos) {
        ret.push_back(E2::APPROACHING_VEHICLES_STATES);
    }
    if(my.find("ALL")!=string::npos) {
        ret.push_back(E2::ALL);
    }
    return ret;
}



NLDetectorBuilder::E3MeasuresVector
NLDetectorBuilder::parseE3Measures(const std::string &measures)
{
    string my = measures;
    StringUtils::upper(my);
    E3MeasuresVector ret;
    if(my.find("MEAN_TRAVELTIME")!=string::npos) {
        ret.push_back(MSE3Collector::MEAN_TRAVELTIME);
    }
    if(my.find("MEAN_NUMBER_OF_HALTINGS_PER_VEHICLE")!=string::npos) {
        ret.push_back(MSE3Collector::MEAN_NUMBER_OF_HALTINGS_PER_VEHICLE);
    }
    if(my.find("NUMBER_OF_VEHICLES")!=string::npos) {
        ret.push_back(MSE3Collector::NUMBER_OF_VEHICLES);
    }
    if(my.find("ALL")!=string::npos) {
        ret.push_back(MSE3Collector::ALL);
    }
    return ret;
}


MSInductLoop *
NLDetectorBuilder::createInductLoop(const std::string &id,
                                    MSLane *lane, SUMOReal pos,
                                    int splInterval)
{
    return new MSInductLoop(id, lane, pos, splInterval);
}


#ifdef HAVE_MESOSIM
MEInductLoop *
NLDetectorBuilder::createMEInductLoop(const std::string &id,
                                      MESegment *s, SUMOReal pos,
                                      int splInterval)
{
    return new MEInductLoop(id, s, pos, splInterval);
}
#endif


MSE2Collector *
NLDetectorBuilder::createSingleLaneE2Detector(const std::string &id,
        DetectorUsage usage, MSLane *lane, SUMOReal pos, SUMOReal length,
        SUMOReal haltingTimeThreshold,
        MSUnit::MetersPerSecond haltingSpeedThreshold,
        SUMOReal jamDistThreshold,
        SUMOTime deleteDataAfterSeconds)
{
    return new MSE2Collector(id, usage, lane, pos, length,
        haltingTimeThreshold, haltingSpeedThreshold,
        jamDistThreshold, deleteDataAfterSeconds);

}


MS_E2_ZS_CollectorOverLanes *
NLDetectorBuilder::createMultiLaneE2Detector(const std::string &id,
        DetectorUsage usage, MSLane *lane, SUMOReal pos,
        SUMOReal haltingTimeThreshold,
        MSUnit::MetersPerSecond haltingSpeedThreshold,
        SUMOReal jamDistThreshold,
        SUMOTime deleteDataAfterSeconds)
{
    return new MS_E2_ZS_CollectorOverLanes( id, usage, lane, pos,
            haltingTimeThreshold, haltingSpeedThreshold,
            jamDistThreshold, deleteDataAfterSeconds);
}


MSE3Collector *
NLDetectorBuilder::createE3Detector(const std::string &id,
        const CrossSectionVector &entries,
        const CrossSectionVector &exits,
        SUMOReal haltingTimeThreshold,
        MSUnit::MetersPerSecond haltingSpeedThreshold,
        SUMOTime deleteDataAfterSeconds)
{
    return new MSE3Collector( id, entries, exits,
        haltingTimeThreshold, haltingSpeedThreshold, deleteDataAfterSeconds);
}



            /*
MSDetector::OutputStyle NLDetectorBuilder::convertStyle(const std::string &id,
        const std::string &style)
{
     if(style=="GNUPLOT" || style=="GPLOT")
         return MSDetector::GNUPLOT;
     if(style=="CSV")
         return MSDetector::CSV;
     throw InvalidArgument("Unknown output style '" + style + "' while parsing the detector '" + id + "' occured.");
}
*/


MSLane *
NLDetectorBuilder::getLaneChecking(const std::string &id,
                                   const std::string &detid)
{
    // get and check the lane
    MSLane *clane = MSLane::dictionary(id);
    if(clane==0) {
        throw InvalidArgument("While building detector '" + detid + "':\n"
            + "The lane with the id '" + id + "' is not known.");
    }
    return clane;
}


/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

// Local Variables:
// mode:C++
// End:


