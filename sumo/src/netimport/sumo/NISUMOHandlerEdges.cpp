/****************************************************************************/
/// @file    NISUMOHandlerEdges.cpp
/// @author  Daniel Krajzewicz
/// @date    Sept 2002
/// @version $Id$
///
// A handler for SUMO edges
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
// compiler pragmas
// ===========================================================================
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif


// ===========================================================================
// included modules
// ===========================================================================
#ifdef WIN32
#include <windows_config.h>
#else
#include <config.h>
#endif
#include <string>
#include <utils/xml/SUMOSAXHandler.h>
#include <utils/common/UtilExceptions.h>
#include <netbuild/NLLoadFilter.h>
#include <netbuild/NBEdge.h>
#include <netbuild/NBEdgeCont.h>
#include <netbuild/nodes/NBNode.h>
#include <netbuild/nodes/NBNodeCont.h>
#include <utils/xml/SUMOXMLDefinitions.h>
#include "NISUMOHandlerEdges.h"

#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif // CHECK_MEMORY_LEAKS
// ===========================================================================
// used namespaces
// ===========================================================================

using namespace std;

NISUMOHandlerEdges::NISUMOHandlerEdges(NBEdgeCont &ec, NBNodeCont &nc,
                                       LoadFilter what)
        : SUMOSAXHandler("sumo-network"),
        _loading(what), myEdgeCont(ec), myNodeCont(nc)
{}


NISUMOHandlerEdges::~NISUMOHandlerEdges()
{}


void
NISUMOHandlerEdges::myStartElement(SumoXMLTag element, const std::string &/*name*/,
                                   const Attributes &attrs)
{
    if (element==SUMO_TAG_EDGE&&_loading==LOADFILTER_ALL) {
        addEdge(attrs);
    }
}


void
NISUMOHandlerEdges::addEdge(const Attributes &attrs)
{
    string id;
    try {
        // get the id
        id = getString(attrs, SUMO_ATTR_ID);
        // get the name
        string name;
        try {
            name = getString(attrs, SUMO_ATTR_NAME);
        } catch (EmptyData) {}
        // get the type
        string type;
        try {
            type = getString(attrs, SUMO_ATTR_TYPE);
        } catch (EmptyData) {}
        // get the origin and the destination node
        NBNode *from = getNode(attrs, SUMO_ATTR_FROMNODE, "from", id);
        NBNode *to = getNode(attrs, SUMO_ATTR_TONODE, "to", id);
        if (from==0||to==0) {
            return;
        }
        // get some other parameter
        SUMOReal speed = getFloat(attrs, SUMO_ATTR_SPEED);
        SUMOReal length = getFloat(attrs, SUMO_ATTR_LENGTH);
        int nolanes = getInt(attrs, SUMO_ATTR_NOLANES);
        int priority = getInt(attrs, SUMO_ATTR_PRIORITY);
        if (speed>0&&length>0&&nolanes>0&&priority>0) {
            myEdgeCont.insert(new NBEdge(id, name, from, to, type, speed,
                                         nolanes, length, priority));
        }
    } catch (EmptyData) {
        addError("An edge with an unknown id occured.");
    }
}

NBNode *
NISUMOHandlerEdges::getNode(const Attributes &attrs, SumoXMLAttr id,
                            const std::string &dir, const std::string &name)
{
    try {
        string nodename = getString(attrs, id);
        NBNode *node = myNodeCont.retrieve(nodename);
        if (node==0) {
            addError("The " + dir + "-node '" + nodename + "' used within edge '" + name + "' is not known.");
        }
        return node;
    } catch (EmptyData) {
        addError("Missing " + dir + "-node name for edge with id '" + name + "'");
    }
    return 0;
}

void NISUMOHandlerEdges::myCharacters(SumoXMLTag , const std::string &,
                                      const std::string &)
{
}


void NISUMOHandlerEdges::myEndElement(SumoXMLTag , const std::string &)
{
}



/****************************************************************************/

