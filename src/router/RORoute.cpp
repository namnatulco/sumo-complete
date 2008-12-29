/****************************************************************************/
/// @file    RORoute.cpp
/// @author  Daniel Krajzewicz
/// @date    Sept 2002
/// @version $Id$
///
// A complete router's route
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

#include <string>
#include <iostream>
#include <utils/common/Named.h>
#include <utils/common/StringUtils.h>
#include "ROEdge.h"
#include "RORoute.h"
#include "ROHelper.h"
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
RORoute::RORoute(const std::string &id, SUMOReal costs, SUMOReal prop,
                 const std::vector<const ROEdge*> &route,
                 const RGBColor * const color) throw()
        : Named(StringUtils::convertUmlaute(id)), myCosts(costs),
        myProbability(prop), myRoute(route), myColor(color)
{}


RORoute::RORoute(const RORoute &src) throw()
        : Named(src.myID), myCosts(src.myCosts),
        myProbability(src.myProbability), myRoute(src.myRoute), myColor(0)
{
    if (src.myColor!=0) {
        myColor = new RGBColor(*src.myColor);
    }
}


RORoute::~RORoute() throw()
{
    delete myColor;
}


void
RORoute::add(ROEdge *edge) throw()
{
    myRoute.push_back(edge);
}


void
RORoute::setCosts(SUMOReal costs) throw()
{
    myCosts = costs;
}


void
RORoute::setProbability(SUMOReal prob) throw()
{
    myProbability = prob;
}


void
RORoute::pruneFirst() throw()
{
    myRoute.erase(myRoute.begin());
}


void
RORoute::recheckForLoops() throw()
{
    ROHelper::recheckForLoops(myRoute);
}


OutputDevice &
RORoute::writeXMLDefinition(OutputDevice &dev, bool asAlternatives) const
{
    // (optional) alternatives header
    if (asAlternatives) {
        dev << "<routeDistribution last=\"0\">\n         ";
    }
    // the route
    dev << "<route";
    if (asAlternatives) {
        dev << " cost=\"" << myCosts;
        dev << "\" probability=\"" << myProbability << "\"";
    }
    if (myColor!=0) {
        dev << " color=\"" << *myColor << "\"";
    }
    dev << " edges=\"" << myRoute << "\"/>\n";
    // (optional) alternatives end
    if (asAlternatives) {
        dev << "      </routeDistribution>\n";
    }
    return dev;
}




/****************************************************************************/

