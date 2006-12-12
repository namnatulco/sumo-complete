/***************************************************************************
                          GUIRouteHandler.cpp
              Parser and container for routes during their loading
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
    const char rcsid[] =
        "$Id$";
}
// $Log$
// Revision 1.13  2006/12/12 12:11:01  dkrajzew
// removed simple/full geometry options; everything is now drawn using full geometry
//
// Revision 1.12  2006/11/30 07:43:35  dkrajzew
// added the inc-dua option in order to increase dua-computation
//
// Revision 1.11  2006/09/18 10:01:39  dkrajzew
// added vehicle class support to microsim
//
// Revision 1.10  2006/07/06 06:40:38  dkrajzew
// applied current microsim-APIs
//
// Revision 1.9  2006/06/22 07:14:56  dkrajzew
// debugged handling of previously loaded vehicles when loading states
//
// Revision 1.8  2006/04/18 08:12:04  dkrajzew
// consolidation of interaction with gl-objects
//
// Revision 1.7  2006/01/09 11:51:30  dkrajzew
// usage of vehicle and route coloring debugged
//
// Revision 1.6  2005/12/01 07:33:44  dkrajzew
// introducing bus stops: eased building vehicles; vehicles may now have nested elements
//
// Revision 1.5  2005/10/07 11:37:17  dkrajzew
// THIRD LARGE CODE RECHECK: patched problems on Linux/Windows configs
//
// Revision 1.4  2005/09/22 13:39:35  dkrajzew
// SECOND LARGE CODE RECHECK: converted doubles and floats to SUMOReal
//
// Revision 1.3  2005/09/15 11:06:37  dkrajzew
// LARGE CODE RECHECK
//
// Revision 1.2  2005/05/04 08:02:55  dkrajzew
// level 3 warnings removed; a certain SUMOTime time description added
//
// Revision 1.1  2004/07/02 08:56:12  dkrajzew
// coloring of routes and vehicle types added
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
#include <map>
#include <vector>
#include <microsim/MSRoute.h>
#include <microsim/MSEdge.h>
#include <guisim/GUIVehicleType.h>
#include <guisim/GUIRoute.h>
#include <microsim/MSVehicle.h>
#include <microsim/MSEdge.h>
#include <microsim/MSEmitControl.h>
#include <microsim/MSVehicleControl.h>
#include "GUIRouteHandler.h"
#include <utils/xml/XMLBuildingExceptions.h>
#include <utils/sumoxml/SUMOSAXHandler.h>
#include <utils/sumoxml/SUMOXMLDefinitions.h>
#include <utils/common/MsgHandler.h>
#include <utils/common/StringTokenizer.h>
#include <utils/common/UtilExceptions.h>
#include <utils/gfx/RGBColor.h>
#include <utils/gfx/GfxConvHelper.h>
#include <utils/options/OptionsSubSys.h>
#include <utils/options/OptionsCont.h>
#include <microsim/MSGlobals.h>

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
GUIRouteHandler::GUIRouteHandler(const std::string &file,
                                 MSVehicleControl &vc,
                                 bool addVehiclesDirectly,
                                 int incDUABase,
                                 int incDUAStage)
    : MSRouteHandler(file, vc, addVehiclesDirectly, true, incDUABase, incDUAStage)
{
}


GUIRouteHandler::~GUIRouteHandler()
{
}


void
GUIRouteHandler::addVehicleType(const Attributes &attrs)
{
    RGBColor col =
        GfxConvHelper::parseColor(
            getStringSecure(attrs, SUMO_ATTR_COLOR, "1,1,0"));
    // !!! unsecure
    try {
        string id = getString(attrs, SUMO_ATTR_ID);
        try {
            addParsedVehicleType(id,
                getFloat(attrs, SUMO_ATTR_LENGTH),
                getFloat(attrs, SUMO_ATTR_MAXSPEED),
                getFloat(attrs, SUMO_ATTR_ACCEL),
                getFloat(attrs, SUMO_ATTR_DECEL),
                getFloat(attrs, SUMO_ATTR_SIGMA),
                parseVehicleClass(*this, attrs, "vehicle", id),
                col);
        } catch (XMLIdAlreadyUsedException &e) {
            MsgHandler::getErrorInstance()->inform(e.getMessage("vehicletype", id));
        } catch (EmptyData) {
            MsgHandler::getErrorInstance()->inform("Error in description: missing attribute in a vehicletype-object.");
        } catch (NumberFormatException) {
            MsgHandler::getErrorInstance()->inform("Error in description: one of an vehtype's attributes must be numeric but is not.");
        }
    } catch (EmptyData) {
        MsgHandler::getErrorInstance()->inform("Error in description: missing id of a vehicle-object.");
    }
}


void
GUIRouteHandler::addParsedVehicleType(const string &id, const SUMOReal length,
                                      const SUMOReal maxspeed, const SUMOReal bmax,
                                      const SUMOReal dmax, const SUMOReal sigma,
                                      SUMOVehicleClass vclass,
                                      const RGBColor &c)
{
    GUIVehicleType *vtype =
        new GUIVehicleType(c, id, length, maxspeed, bmax, dmax, sigma, vclass);
    if(!MSVehicleType::dictionary(id, vtype)) {
        if(!MSGlobals::gStateLoaded) {
            throw XMLIdAlreadyUsedException("VehicleType", id);
        }
    }
}


void
GUIRouteHandler::closeRoute()
{
    int size = myActiveRoute.size();
    if(size==0) {
        throw XMLListEmptyException();
    }
    GUIRoute *route =
        new GUIRoute(myColor, myActiveRouteID, myActiveRoute, m_IsMultiReferenced);
    myActiveRoute.clear();
    if(!MSRoute::dictionary(myActiveRouteID, route)) {

        delete route;
        if(!MSGlobals::gStateLoaded) {
            throw XMLIdAlreadyUsedException("route", myActiveRouteID);
        } else {
            route = static_cast<GUIRoute*>(MSRoute::dictionary(myActiveRouteID));
        }
    }
    if(myAmInEmbeddedMode) {
        myCurrentEmbeddedRoute = route;
    }
}


void
GUIRouteHandler::openRoute(const Attributes &attrs)
{
    myColor =
        GfxConvHelper::parseColor(
            getStringSecure(attrs, SUMO_ATTR_COLOR, "1,1,0"));
    MSRouteHandler::openRoute(attrs);
}



/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

// Local Variables:
// mode:C++
// End:
