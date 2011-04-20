/****************************************************************************/
/// @file    TraCIServerAPI_Route.cpp
/// @author  Daniel Krajzewicz
/// @date    07.05.2009
/// @version $Id$
///
// APIs for getting/setting route values via TraCI
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

#include <microsim/MSNet.h>
#include <microsim/MSRoute.h>
#include <microsim/MSEdge.h>
#include "TraCIConstants.h"
#include "TraCIServerAPI_Route.h"

#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif // CHECK_MEMORY_LEAKS


// ===========================================================================
// used namespaces
// ===========================================================================
using namespace traci;


// ===========================================================================
// method definitions
// ===========================================================================
bool
TraCIServerAPI_Route::processGet(TraCIServer &server, tcpip::Storage &inputStorage,
                                 tcpip::Storage &outputStorage) {
    std::string warning = ""; // additional description for response
    // variable & id
    int variable = inputStorage.readUnsignedByte();
    std::string id = inputStorage.readString();
    // check variable
    if (variable!=ID_LIST&&variable!=VAR_EDGES) {
        server.writeStatusCmd(CMD_GET_ROUTE_VARIABLE, RTYPE_ERR, "Get Route Variable: unsupported variable specified", outputStorage);
        return false;
    }
    // begin response building
    tcpip::Storage tempMsg;
    //  response-code, variableID, objectID
    tempMsg.writeUnsignedByte(RESPONSE_GET_ROUTE_VARIABLE);
    tempMsg.writeUnsignedByte(variable);
    tempMsg.writeString(id);
    // process request
    if (variable==ID_LIST) {
        std::vector<std::string> ids;
        MSRoute::insertIDs(ids);
        tempMsg.writeUnsignedByte(TYPE_STRINGLIST);
        tempMsg.writeStringList(ids);
    } else {
        const MSRoute *r = MSRoute::dictionary(id);
        if (r==0) {
            server.writeStatusCmd(CMD_GET_ROUTE_VARIABLE, RTYPE_ERR, "Route '" + id + "' is not known", outputStorage);
            return false;
        }
        switch (variable) {
        case VAR_EDGES:
            tempMsg.writeUnsignedByte(TYPE_STRINGLIST);
            tempMsg.writeInt(r->size());
            for (MSRouteIterator i=r->begin(); i!=r->end(); ++i) {
                tempMsg.writeString((*i)->getID());
            }
            break;
        default:
            break;
        }
    }
    server.writeStatusCmd(CMD_GET_ROUTE_VARIABLE, RTYPE_OK, warning, outputStorage);
    // send response
    outputStorage.writeUnsignedByte(0); // command length -> extended
    outputStorage.writeInt(1 + 4 + (int)tempMsg.size());
    outputStorage.writeStorage(tempMsg);
    return true;
}


bool
TraCIServerAPI_Route::processSet(TraCIServer& server, tcpip::Storage& inputStorage,
                                 tcpip::Storage& outputStorage) {
    std::string warning = ""; // additional description for response
    // variable
    int variable = inputStorage.readUnsignedByte();
    if (variable!=ADD) {
        server.writeStatusCmd(CMD_SET_ROUTE_VARIABLE, RTYPE_ERR, "Change Route State: unsupported variable specified", outputStorage);
        return false;
    }
    // id
    std::string id = inputStorage.readString();
    // process
    int valueDataType = inputStorage.readUnsignedByte();
    switch (variable) {
    case ADD: {
        if (valueDataType!=TYPE_STRINGLIST) {
            server.writeStatusCmd(CMD_SET_ROUTE_VARIABLE, RTYPE_ERR, "A string list is needed for adding a new route.", outputStorage);
            return false;
        }
        //read itemNo 
        int numEdges = inputStorage.readInt();
		MSEdgeVector edges(numEdges);
		while (numEdges--) {
			MSEdge* edge = MSEdge::dictionary(inputStorage.readString());
			if (edge==0) {
				server.writeStatusCmd(CMD_SET_ROUTE_VARIABLE, RTYPE_ERR, "Unknown edge in route.", outputStorage);
				return false;
			}
			edges.push_back(edge);
		}
		const std::vector<SUMOVehicleParameter::Stop> stops;
        if (!MSRoute::dictionary(id, new MSRoute(id, edges, 1, RGBColor::DEFAULT_COLOR, stops))) {
            server.writeStatusCmd(CMD_SET_ROUTE_VARIABLE, RTYPE_ERR, "Could not add route.", outputStorage);
            return false;
        }
    }
    break;
    default:
        break;
    }
    server.writeStatusCmd(CMD_SET_ROUTE_VARIABLE, RTYPE_OK, warning, outputStorage);
    return true;
}



/****************************************************************************/

