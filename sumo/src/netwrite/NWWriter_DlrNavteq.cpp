/****************************************************************************/
/// @file    NWWriter_DlrNavteq.h
/// @author  Jakob Erdmann
/// @date    26.10.2012
/// @version $Id: NWWriter_DlrNavteq.h 12012 2012-03-05 09:41:53Z namdre $
///
// Exporter writing networks using DlrNavteq (Elmar) format
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
#include <algorithm>
#include <ctime>
#include <cmath>
#include <utils/common/MsgHandler.h>
#include <netbuild/NBEdge.h>
#include <netbuild/NBEdgeCont.h>
#include <netbuild/NBNode.h>
#include <netbuild/NBNodeCont.h>
#include <netbuild/NBNetBuilder.h>
#include <utils/common/ToString.h>
#include <utils/common/StringUtils.h>
#include <utils/options/OptionsCont.h>
#include <utils/iodevices/OutputDevice.h>
#include <utils/geom/GeoConvHelper.h>
#include "NWFrame.h"
#include "NWWriter_DlrNavteq.h"

#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif // CHECK_MEMORY_LEAKS


// ---------------------------------------------------------------------------
// static members
// ---------------------------------------------------------------------------
const std::string NWWriter_DlrNavteq::UNDEFINED("-1");

// ---------------------------------------------------------------------------
// static methods
// ---------------------------------------------------------------------------
void
NWWriter_DlrNavteq::writeNetwork(const OptionsCont& oc, NBNetBuilder& nb) {
    // check whether a matsim-file shall be generated
    if (!oc.isSet("dlr-navteq-output")) {
        return;
    }
    writeNodesUnsplitted(oc, nb.getNodeCont(), nb.getEdgeCont());
    writeLinksUnsplitted(oc, nb.getNodeCont(), nb.getEdgeCont());
}


void NWWriter_DlrNavteq::writeHeader(OutputDevice& device, const OptionsCont& oc) {
    time_t rawtime;
    time(&rawtime);
    char buffer [80];
    strftime(buffer, 80, "on %c", localtime(&rawtime));
    device << "# Generated " << buffer << " by " << oc.getFullName() << "\n";
    std::stringstream tmp;
    oc.writeConfiguration(tmp, true, false, false);
    tmp.seekg(std::ios_base::beg);
    std::string line;
    while (!tmp.eof()) {
        std::getline(tmp, line);
        device << "# " << line << "\n";
    }
    device << "#\n";
}

void
NWWriter_DlrNavteq::writeNodesUnsplitted(const OptionsCont& oc, NBNodeCont& nc, NBEdgeCont& ec) {
    // For "real" nodes we simply use the node id.
    // For internal nodes (geometry vectors describing edge geometry in the parlance of this format) 
    // we use the id of the edge and do not bother with
    // compression (each direction gets its own internal node).
    // XXX add option for generating numerical ids in case the input network has string ids and the target process needs integers
    OutputDevice& device = OutputDevice::getDevice(oc.getString("dlr-navteq-output") + "_nodes_unsplitted.txt");
    writeHeader(device, oc);
    const GeoConvHelper& gch = GeoConvHelper::getFinal();
    const bool haveGeo = gch.usingGeoProjection();
    const SUMOReal geoScale = pow(10.0f, haveGeo ? 5 : 2); // see NIImporter_DlrNavteq::GEO_SCALE
    device.setPrecision(0);
    if (!haveGeo) {
        WRITE_WARNING("DlrNavteq node data will be written in (floating point) cartesian coordinates");
    }
    // write format specifier
    device << "# NODE_ID\tIS_BETWEEN_NODE\tamount_of_geocoordinates\tx1\ty1\t[x2 y2  ... xn  yn]\n";
    // write normal nodes
    for (std::map<std::string, NBNode*>::const_iterator i = nc.begin(); i != nc.end(); ++i) {
        NBNode* n = (*i).second;
        Position pos = n->getPosition();
        gch.cartesian2geo(pos);
        pos.mul(geoScale);
        device << n->getID() << "\t0\t1\t" << pos.x() << "\t" << pos.y() << "\n";
    }
    // write "internal" nodes
    for (std::map<std::string, NBEdge*>::const_iterator i = ec.begin(); i != ec.end(); ++i) {
        NBEdge* e = (*i).second;
        const PositionVector& geom = e->getGeometry();
        if (geom.size() > 2) {
            if (e->getID() == UNDEFINED) {
                WRITE_WARNING("Edge id '" + UNDEFINED + 
                        "' clashes with the magic value for NO_BETWEEN_NODE. Internal geometry for this edge will be lost.");
            }
            device << e->getID() << "\t1\t" << geom.size() - 2;  
            for (size_t ii = 1; ii < geom.size() - 1; ++ii) {
                Position pos = geom[ii];
                gch.cartesian2geo(pos);
                pos.mul(geoScale);
                device << "\t" << pos.x() << "\t" << pos.y();
            }
            device << "\n";
        }
    }
    device.close();
}


void
NWWriter_DlrNavteq::writeLinksUnsplitted(const OptionsCont& oc, NBNodeCont& nc, NBEdgeCont& ec) {
    OutputDevice& device = OutputDevice::getDevice(oc.getString("dlr-navteq-output") + "_links_unsplitted.txt");
    writeHeader(device, oc);
    // write format specifier
    device << "# LINK_ID\tNODE_ID_FROM\tNODE_ID_TO\tBETWEEN_NODE_ID\tlength\tvehicle_type\tform_of_way\tbrunnel_type\tfunctional_road_class\tspeed_category\tnumber_of_lanes\tspeed_limit\tNAME_ID1_REGIONAL\tNAME_ID2_LOCAL\thousenumbers_right\thousenumbers_left\tZIP_CODE\tAREA_ID\tSUBAREA_ID\tthrough_traffic\tspecial_restrictions\textended_number_of_lanes\tisRamp\tconnection\n";
    // write edges
    for (std::map<std::string, NBEdge*>::const_iterator i = ec.begin(); i != ec.end(); ++i) {
        NBEdge* e = (*i).second;
        const std::string& betweenNodeID = (e->getGeometry().size() > 2) ? e->getID() : UNDEFINED;
        device << e->getID() << "\t"
            << e->getFromNode()->getID() << "\t"
            << e->getToNode()->getID() << "\t"
            << betweenNodeID << "\t"
            << e->getLength() << "\t"
            << getAllowedTypes(e->getPermissions()) << "\t"
            << "3\t" // Speed Category 1-8 XXX refine this
            << UNDEFINED << "\t" // no special brunnel type (we don't know yet)
            << getRoadClass(e) << "\t"
            << getSpeedCategory(e->getSpeed()) << "\t"
            << getNavteqLaneCode(e->getNumLanes()) << "\t"
            << e->getSpeed() << "\t"
            << UNDEFINED << "\t" // NAME_ID1_REGIONAL XXX
            << UNDEFINED << "\t" // NAME_ID2_LOCAL XXX
            << UNDEFINED << "\t" // housenumbers_right 
            << UNDEFINED << "\t" // housenumbers_left 
            << UNDEFINED << "\t" // ZIP_CODE 
            << UNDEFINED << "\t" // AREA_ID 
            << UNDEFINED << "\t" // SUBAREA_ID 
            << "1\t" // through_traffic (allowed)
            << UNDEFINED << "\t" // special_restrictions 
            << UNDEFINED << "\t" // extended_number_of_lanes 
            << UNDEFINED << "\t" // isRamp 
            << "0\t" // connection (between nodes always in order)
            << "\n"; 
    }
}


std::string 
NWWriter_DlrNavteq::getAllowedTypes(SVCPermissions permissions) {
    if (permissions == SVCFreeForAll) {
        return "1000000000";
    }
    std::ostringstream oss;
    oss << "0";
    oss << (permissions & SVC_PASSENGER)                    > 0 ? 1 : 0;
    oss << (permissions & SVC_HOV)                          > 0 ? 1 : 0;
    oss << (permissions & SVC_PUBLIC_EMERGENCY)             > 0 ? 1 : 0;
    oss << (permissions & SVC_TAXI)                         > 0 ? 1 : 0;
    oss << (permissions & (SVC_PUBLIC_TRANSPORT | SVC_BUS)) > 0 ? 1 : 0;
    oss << (permissions & SVC_DELIVERY)                     > 0 ? 1 : 0;
    oss << (permissions & SVC_TRANSPORT)                    > 0 ? 1 : 0;
    oss << (permissions & SVC_BICYCLE)                      > 0 ? 1 : 0;
    oss << (permissions & SVC_PEDESTRIAN)                   > 0 ? 1 : 0;
    return oss.str();
}


int 
NWWriter_DlrNavteq::getRoadClass(NBEdge* edge) {
    // quoting the navteq manual: 
    // As a general rule, Functional Road Class assignments have no direct
    // correlation with other road attributes like speed, controlled access, route type, etc. 
    //
    // we do a simple speed / lane-count mapping anyway 
    // XXX the resulting functional road class layers probably won't be connected as required
    const int kph = (int)std::floor(edge->getSpeed() * 3.6 + 0.5);
    if ((kph) > 100) return 0;
    if ((kph) > 70) return 1;
    if ((kph) > 50) return (edge->getNumLanes() > 1 ? 2 : 3);
    if ((kph) > 30) return 3;
    return 4;
}


int 
NWWriter_DlrNavteq::getSpeedCategory(SUMOReal speed) {
    const int kph = (int)std::floor(speed * 3.6 + 0.5);
    if ((kph) > 130) return 1;
    if ((kph) > 100) return 2;
    if ((kph) > 90) return 3;
    if ((kph) > 70) return 4;
    if ((kph) > 50) return 5;
    if ((kph) > 30) return 6;
    if ((kph) > 10) return 7;
    return 8;
}


int 
NWWriter_DlrNavteq::getNavteqLaneCode(unsigned int numLanes) {
    const int code = numLanes == (1 ? 1 :
        (numLanes < 4 ?  2 : 3));
    return numLanes * 10 + code;
}
/****************************************************************************/

