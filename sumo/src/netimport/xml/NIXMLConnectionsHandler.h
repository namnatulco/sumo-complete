/****************************************************************************/
/// @file    NIXMLConnectionsHandler.h
/// @author  Daniel Krajzewicz
/// @date    Tue, 20 Nov 2001
/// @version $Id$
///
// Used to parse the XML-descriptions of types given in a XML-format
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
#ifndef NIXMLConnectionsHandler_h
#define NIXMLConnectionsHandler_h
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

#include <utils/xml/SUMOSAXHandler.h>
#include <netbuild/NBConnection.h>


// ===========================================================================
// class declarations
// ===========================================================================
class NBEdge;
class NBEdgeCont;


// ===========================================================================
// class definitions
// ===========================================================================
class NIXMLConnectionsHandler : public SUMOSAXHandler
{
public:
    /// standard constructor
    NIXMLConnectionsHandler(NBEdgeCont &ec);

    /// destructor
    ~NIXMLConnectionsHandler();

protected:
    /// The method called by the SAX-handler to parse start tags
    void myStartElement(int element, const std::string &name,
                        const Attributes &attrs);

    /// The method called by the SAX-handler to parse intermediate characters
    void myCharacters(int element, const std::string &name,
                      const std::string &chars);

    /// The method called by the SAX-handler to parse closing tags
    void myEndElement(int element, const std::string &name);

private:
    // parses a connection when it describes a edge-2-edge relationship
    void parseEdgeBound(const Attributes &attrs, NBEdge *from,
                        NBEdge *to);

    // parses a connection when it describes a lane-2-lane relationship
    void parseLaneBound(const Attributes &attrs,NBEdge *from,
                        NBEdge *to);

    /** @brief returns the connection described by def
     *
     * def should have the following format <FROM_EDGE_ID>[_<FROM_LANE_NO>]-><TO_EDGE_ID>[_<TO_LANE_NO>]
     */
    NBConnection parseConnection(const std::string &defRole, const std::string &def);

    /** @brief Returns the node over which the connection defined by def goes
     *
     * def should have the following format <FROM_EDGE_ID>[_<FROM_LANE_NO>]-><TO_EDGE_ID>[_<TO_LANE_NO>]
     * As node the node the from-edge is incoming to is chosen
     */
    NBNode *getNode(const std::string &def);

private:
    NBEdgeCont &myEdgeCont;

private:
    /** invalid copy constructor */
    NIXMLConnectionsHandler(const NIXMLConnectionsHandler &s);

    /** invalid assignment operator */
    NIXMLConnectionsHandler &operator=(const NIXMLConnectionsHandler &s);

};


#endif

/****************************************************************************/

