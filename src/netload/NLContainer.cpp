/***************************************************************************
                          NLContainer.cpp
			  Holds the builded structures or structures that
			  hold these
                             -------------------
    project              : SUMO
    begin                : Mon, 9 Jul 2001
    copyright            : (C) 2001 by DLR/IVF http://ivf.dlr.de/
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
     const char rcsid[] = "$Id$";
}
// $Log$
// Revision 1.3  2002/10/21 09:52:57  dkrajzew
// support for route multireferencing added
//
// Revision 1.2  2002/10/17 10:34:48  dkrajzew
// possibility of retrival of the preinitialised net during loading implemented for trigger handling
//
// Revision 1.1  2002/10/16 15:36:50  dkrajzew
// moved from ROOT/sumo/netload to ROOT/src/netload; new format definition parseable in one step
//
// Revision 1.9  2002/07/31 17:34:50  roessel
// Changes since sourceforge cvs request.
//
// Revision 1.10  2002/07/30 15:18:59  croessel
// Changes because MSNet is singleton now.
//
// Revision 1.9  2002/07/22 12:44:32  dkrajzew
// Source loading structures added
//
// Revision 1.8  2002/06/11 14:39:26  dkrajzew
// windows eol removed
//
// Revision 1.7  2002/06/11 13:44:32  dkrajzew
// Windows eol removed
//
// Revision 1.6  2002/06/10 08:36:07  dkrajzew
// Conversion of strings generalized
//
// Revision 1.5  2002/06/07 14:39:57  dkrajzew
// errors occured while building larger nets and adaption of new netconverting methods debugged
//
// Revision 1.4  2002/04/17 11:17:01  dkrajzew
// windows-newlines removed
//
// Revision 1.3  2002/04/16 06:05:03  dkrajzew
// Output of the number of build detectors added
//
// Revision 1.2  2002/04/15 07:05:35  dkrajzew
// new loading paradigm implemented
//
// Revision 1.1.1.1  2002/04/08 07:21:24  traffic
// new project name
//
// Revision 2.1  2002/03/15 09:32:23  traffic
// Handling of map logics removed; Catching of invalid logics implemented
//
// Revision 2.0  2002/02/14 14:43:21  croessel
// Bringing all files to revision 2.0. This is just cosmetics.
//
// Revision 1.4  2002/02/13 15:40:40  croessel
// Merge between SourgeForgeRelease and tesseraCVS.
//
// Revision 1.1  2001/12/06 13:36:04  traffic
// moved from netbuild
//
//
/* =========================================================================
 * included modules
 * ======================================================================= */
#include <util/PlatformUtils.hpp>
#include <util/TransService.hpp>
#include <sax2/SAX2XMLReader.hpp>
#include <sax2/XMLReaderFactory.hpp>
#include <sstream>
#include "NLContainer.h"
#include <microsim/MSLane.h>
#include <microsim/MSEdge.h>
#include <microsim/MSNet.h>
#include <microsim/MSVehicleType.h>
#include <microsim/MSVehicle.h>
#include <microsim/MSEmitControl.h>
#include <microsim/MSEventControl.h>
#include <microsim/MSEdgeControl.h>
#include <microsim/MSJunctionControl.h>
#include <microsim/MSRoute.h>
#include "NLEdgeControlBuilder.h"
#include "NLJunctionControlBuilder.h"
#include "NLRoutesBuilder.h"
#include "NLNetBuilder.h"
#include "NLSucceedingLaneBuilder.h"
#include <utils/common/SLogging.h>
#include <utils/common/SErrorHandler.h>
#include <utils/xml/XMLBuildingExceptions.h>


/* =========================================================================
 * used namespaces
 * ======================================================================= */
using namespace std;

/* =========================================================================
 * method definitions
 * ======================================================================= */
NLContainer::NLContainer(NLEdgeControlBuilder * const edgeBuilder)
    :
    m_pECB(edgeBuilder),
    m_pJCB(new NLJunctionControlBuilder(this)),
    m_pRCB(new NLRoutesBuilder()),
    m_pSLB(new NLSucceedingLaneBuilder()),
    m_pVehicles(0),
    m_pDetectors(0),
    m_EventControl(new MSEventControl("")),
    noEdges(0),
    noLanes(0),
    noJunctions(0),
    noVehicles(0),
    noVehicleTypes(0),
    noRoutes(0),
    noDetectors(0)
{
    // ... the storage for the detectors
    m_pDetectors = new MSNet::DetectorCont();
//    m_pDetectors->reserve(noDetectors);
    // ... the storage for junctions
    m_pVehicles = new MSEmitControl::VehCont();
//    m_pVehicles->reserve(noVehicles);
}


NLContainer::~NLContainer()
{
    delete m_pECB;
    delete m_pJCB;
    delete m_pRCB;
    delete m_pSLB;
}




// counting of data
void
NLContainer::incEdges()
{
    noEdges++;
}


void
NLContainer::incLanes()
{
    noLanes++;
}


void
NLContainer::incJunctions()
{
    noJunctions++;
}


void
NLContainer::incVehicles()
{
    noVehicles++;
}


void
NLContainer::incVehicleTypes()
{
    noVehicleTypes++;
}


void
NLContainer::incRoutes()
{
    noRoutes++;
}

void
NLContainer::incDetectors()
{
    noRoutes++;
}

void
NLContainer::addKey(string key) {
    m_LogicKeys.insert(LogicKeyCont::value_type(key, 0));
}


void
NLContainer::setEdgeNumber(size_t noEdges)
{
    // preallocate the storage for the edges
    m_pECB->prepare(noEdges);
}


void
NLContainer::setNodeNumber(size_t noNodes)
{
    // preallocate the storage for the edges
    m_pJCB->prepare(noEdges);
}




// interface to use the edge control builder
void
NLContainer::addEdge(const string &id)
{
    try {
        m_pECB->addEdge(id);
    } catch (XMLIdAlreadyUsedException &e) {
        throw e;
    }
}


void
NLContainer::chooseEdge(const string &id)
{
    try {
        m_pECB->chooseEdge(id);
    } catch (XMLIdNotKnownException &e) {
        throw e;
    }
}


void
NLContainer::addLane(const string &id, const bool isDepartLane,
                     const float maxSpeed, const float length,
                     const float changeUrge)
{
    MSEdge *edge = m_pECB->getActiveReference();
    if(/* NLNetBuilder::check && */ edge==0) {
        throw XMLInvalidParentException();
    }
    MSLane *lane = new MSLane(id, maxSpeed, length, edge);
    if(/* NLNetBuilder::check && */ !MSLane::dictionary(id, lane))
        throw XMLIdAlreadyUsedException("Lanes", id);
    else {
        try {
            m_pECB->addLane(lane, isDepartLane);
        } catch (XMLDepartLaneDuplicationException &e) {
            throw e;
        }
    }
}


void
NLContainer::closeLanes()
{
    m_pECB->closeLanes();
}


void
NLContainer::openAllowedEdge(const string &id)
{
    MSEdge *edge = MSEdge::dictionary(id);
    if(/* NLNetBuilder::check && */ edge==0) {
        throw XMLIdNotKnownException("edge", id);
    }
    m_pECB->openAllowedEdge(edge);
}


void
NLContainer::addAllowed(const string &id)
{
    MSLane *lane = MSLane::dictionary(id);
    if(/* NLNetBuilder::check && */ lane==0) {
        throw XMLIdNotKnownException("lane", id);
    }
    try {
        m_pECB->addAllowed(lane);
    } catch (XMLInvalidChildException &e) {
        throw e;
    }
}


void
NLContainer::closeAllowedEdge()
{
    m_pECB->closeAllowedEdge();
}


void
NLContainer::closeEdge()
{
    m_pECB->closeEdge();
}







/// interfaces for the building of succeeding lanes
void
NLContainer::openSuccLane(const string &laneId)
{
    m_pSLB->openSuccLane(laneId);
}


void
NLContainer::setSuccJunction(const string &junctionId)
{
    m_pSLB->setSuccJunction(junctionId);
}


void
NLContainer::addSuccLane(bool yield, const string &laneId)
{
    m_pSLB->addSuccLane(yield, laneId);
}


void
NLContainer::closeSuccLane()
{
    m_pSLB->closeSuccLane();
}


std::string
NLContainer::getSuccingLaneName() const
{
    return m_pSLB->getSuccingLaneName();
}




// interfaces to the junction builder
void
NLContainer::openJunction(const string &id, const string &key, string type,
                          double x, double y)
{
    m_pJCB->openJunction(id, key, type, x, y);
}


void
NLContainer::addInLane(const string &id)
{
    MSLane *lane = MSLane::dictionary(id);
    if(/* NLNetBuilder::check && */ lane==0) {
        throw XMLIdNotKnownException("lane", id);
    }
    m_pJCB->addInLane(lane);
}

/*
void
NLContainer::setKey(string key)
{
  m_pJCB->setKey(key);
}
*/

void
NLContainer::closeJunction()
{
    try {
        m_pJCB->closeJunction();
    } catch (XMLIdAlreadyUsedException &e) {
        throw e;
    } catch (XMLIdNotKnownException &e) {
        throw e;
    }
}




// vehicle types
void
NLContainer::addVehicleType(const string &id, const float length,
                            const float maxspeed, const float bmax,
                            const float dmax, const float sigma)
{
    MSVehicleType *vtype =
        new MSVehicleType(id, length, maxspeed, bmax, dmax, sigma);
    if(!MSVehicleType::dictionary(id, vtype))
        /*if(NLNetBuilder::check)*/
            throw XMLIdAlreadyUsedException("VehicleType", id);
}




// handling of routes
void
NLContainer::openRoute(const string &id, bool multiReferenced)
{
    m_pRCB->openRoute(id, multiReferenced);
}


void
NLContainer::addRoutesEdge(const string &id)
{
    MSEdge *edge = MSEdge::dictionary(id);
    if(/* NLNetBuilder::check && */ edge==0) {
        throw XMLIdNotKnownException("routes edge", id);
    }
    m_pRCB->addEdge(edge);
}


void
NLContainer::closeRoute()
{
    try {
        m_pRCB->closeRoute();
    } catch (XMLListEmptyException &e) {
        throw e;
    } catch (XMLIdAlreadyUsedException &e) {
        throw e;
    }
}




// ----- handling of vehicles
void
NLContainer::addVehicle(const string &id, const string &vtypeid,
                        const string &routeid, const long depart)
{
    MSVehicleType *vtype = MSVehicleType::dictionary(vtypeid);
    if(/* NLNetBuilder::check && */ vtype==0) {
        throw XMLIdNotKnownException("vtype", vtypeid);
    }
    MSRoute *route = MSRoute::dictionary(routeid);
    if(/* NLNetBuilder::check && */ route==0) {
        throw XMLIdNotKnownException("route", routeid);
    }
    MSVehicle *vehicle =
        new MSVehicle(id, route, depart, vtype);
    if(!MSVehicle::dictionary(id, vehicle)) {
        throw XMLIdAlreadyUsedException("vehicle", id);
    }
    m_pVehicles->push_back(vehicle);
}


// ----- handling of detectors
void
NLContainer::addDetector(MSDetector *detector) {
    m_pDetectors->push_back(detector);
}



MSEventControl &
NLContainer::getEventControl() const
{
    return *m_EventControl;
}



std::string
NLContainer::getStatistics() const
{
    ostringstream buf;
    buf << "Edges build: " << noEdges << endl;
    buf << "Lanes build: " << noLanes << endl;
    buf << "Junctions build: " << noJunctions << endl;
    buf << "Vehicles build: " << noVehicles << endl;
    buf << "VehiclesTypes build: " << noVehicleTypes << endl;
    buf << "Routes build: " << noRoutes << endl;
    buf << "Detectors build: " << noDetectors << endl;
    buf << ends;
    return buf.str();
}




// end of operations
MSNet *
NLContainer::buildMSNet(MSNet::TimeVector dumpMeanDataIntervalls, 
                        std::string baseNameDumpFiles)
{
    MSEdgeControl *edges = m_pECB->build();
    MSJunctionControl *junctions = m_pJCB->build();
    MSEmitControl *emitters = new MSEmitControl("", m_pVehicles);
    MSNet::init( m_Id, edges, junctions, emitters,
                 m_EventControl,
                 m_pDetectors,
                 dumpMeanDataIntervalls, baseNameDumpFiles, false);

    return MSNet::getInstance();
}


// end of operations
MSEmitControl *
NLContainer::buildVehicles()
{
    return new MSEmitControl("", m_pVehicles);
}


MSNet::DetectorCont *
NLContainer::getDetectors() {
    return m_pDetectors;
}

MSNet &
NLContainer::getNet()
{
    return *(MSNet::getInstance());
}

/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/
//#ifdef DISABLE_INLINE
//#include "NLContainer.icc"
//#endif

// Local Variables:
// mode:C++
//

