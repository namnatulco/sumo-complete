/****************************************************************************/
/// @file    NIXMLEdgesHandler.cpp
/// @author  Daniel Krajzewicz
/// @date    Tue, 20 Nov 2001
/// @version $Id$
///
// Realises the loading of the edges given in a
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
#include <map>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/sax/SAXException.hpp>
#include "NIXMLEdgesHandler.h"
#include <cmath>
#include <utils/xml/SUMOSAXHandler.h>
#include <netbuild/nodes/NBNodeCont.h>
#include <netbuild/NBTypeCont.h>
#include <utils/xml/SUMOXMLDefinitions.h>
#include <utils/common/MsgHandler.h>
#include <utils/common/TplConvert.h>
#include <utils/common/StringTokenizer.h>
#include <utils/geom/GeomConvHelper.h>
#include <utils/common/ToString.h>
#include <utils/options/OptionsCont.h>
#include <utils/options/OptionsSubSys.h>
#include <utils/geom/GeoConvHelper.h>

#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif // CHECK_MEMORY_LEAKS


// ===========================================================================
// used namespaces
// ===========================================================================
using namespace std;

const SUMOReal SUMOXML_INVALID_POSITION = -999999.;

// ===========================================================================
// method definitions
// ===========================================================================
NIXMLEdgesHandler::NIXMLEdgesHandler(NBNodeCont &nc,
                                     NBEdgeCont &ec,
                                     NBTypeCont &tc,
                                     NBDistrictCont &dc,
                                     OptionsCont &options)
        : SUMOSAXHandler("xml-edges - file"),
        _options(options),
        myNodeCont(nc), myEdgeCont(ec), myTypeCont(tc), myDistrictCont(dc)
{}


NIXMLEdgesHandler::~NIXMLEdgesHandler() throw()
{}


void
NIXMLEdgesHandler::myStartElement(SumoXMLTag element,
                                  const Attributes &attrs) throw(ProcessError)
{
    if (element==SUMO_TAG_EDGE) {
        myExpansions.clear();
        // retrieve the id of the edge
        setID(attrs);
        // retrieve the name of the edge
        setName(attrs);
        // use default values, first
        myCurrentSpeed = myTypeCont.getDefaultSpeed();
        myCurrentPriority = myTypeCont.getDefaultPriority();
        myCurrentLaneNo = myTypeCont.getDefaultNoLanes();
        myCurrentEdgeFunction = NBEdge::EDGEFUNCTION_NORMAL;
        // check whether a type's values shall be used
        checkType(attrs);
        // speed, priority and the number of lanes have now default values;
        // try to read the real values from the file
        setGivenSpeed(attrs);
        setGivenLanes(attrs);
        setGivenPriority(attrs);
        setGivenType(attrs);
        // try to get the shape
        myShape = tryGetShape(attrs);
        // and how to spread the lanes
        myLanesSpread = getSpreadFunction(attrs);
        // try to set the nodes
        if (!setNodes(attrs)) {
            // return if this failed
            return;
        }
        // compute the edge's length
        setLength(attrs);
        /// insert the parsed edge into the edges map
        try {
            NBEdge *edge = 0;
            // the edge must be allocated in dependence to whether a shape
            //  is given
            if (myShape.size()==0) {
                edge = new NBEdge(
                           myCurrentID, myCurrentName,
                           myFromNode, myToNode,
                           myCurrentType, myCurrentSpeed,
                           myCurrentLaneNo, myCurrentPriority, myLanesSpread,
                           myCurrentEdgeFunction);
                edge->setLoadedLength(myLength);
            } else {
                edge = new NBEdge(
                           myCurrentID, myCurrentName,
                           myFromNode, myToNode,
                           myCurrentType, myCurrentSpeed,
                           myCurrentLaneNo, myCurrentPriority,
                           myShape, myLanesSpread,
                           myCurrentEdgeFunction);
                edge->setLoadedLength(myLength);
            }
            // insert the edge
            if (!myEdgeCont.insert(edge)) {
                MsgHandler::getErrorInstance()->inform("Duplicate edge occured. ID='" + myCurrentID + "'");
                delete edge;
            }
        } catch (...) {
            MsgHandler::getErrorInstance()->inform(
                "Important information (probably the source or the destination node) missing in edge '"
                + myCurrentID + "'.");
        }
    }
    if (element==SUMO_TAG_LANE) {
        NBEdge *edge = myEdgeCont.retrieve(myCurrentID);
        if (edge==0) {
            if (!OptionsSubSys::helper_CSVOptionMatches("remove-edges", myCurrentID)) {
                MsgHandler::getErrorInstance()->inform("Additional lane information could not been set - the edge with id '" + myCurrentID + "' is not known.");
            }
            return;
        }
        int lane = getIntSecure(attrs, SUMO_ATTR_ID, -1);
        if (lane<0) {
            MsgHandler::getErrorInstance()->inform("Missing lane-id in lane definition (edge '" + myCurrentID + "').");
            return;
        }
        // check whether this lane exists
        // set information about allwed / disallowed vehicle classes
        string disallowed = getStringSecure(attrs, SUMO_ATTR_DISALLOW, "");
        string allowed = getStringSecure(attrs, SUMO_ATTR_ALLOW, "");
        if (disallowed!="") {
            StringTokenizer st(disallowed, ";");
            while (st.hasNext()) {
                edge->disallowVehicleClass(lane, getVehicleClassID(st.next()));
            }
        }
        if (allowed!="") {
            StringTokenizer st(allowed, ";");
            while (st.hasNext()) {
                string next = st.next();
                if (next[0]=='-') {
                    edge->disallowVehicleClass(lane, getVehicleClassID(next.substr(1)));
                } else {
                    edge->allowVehicleClass(lane, getVehicleClassID(next));
                }
            }
        }

        // set information about later beginning lanes
        if (hasAttribute(attrs, SUMO_ATTR_FORCE_LENGTH)) {
            try {
                int forcedLength = getInt(attrs, SUMO_ATTR_FORCE_LENGTH); // !!! describe
                int nameid = forcedLength;
                forcedLength = (int)(edge->getGeometry().length() - forcedLength);
                std::vector<Expansion>::iterator i;
                i = find_if(myExpansions.begin(), myExpansions.end(), expansion_by_pos_finder((SUMOReal) forcedLength));
                if (i==myExpansions.end()) {
                    Expansion e;
                    e.pos = (SUMOReal) forcedLength;
                    e.nameid = nameid;
                    for (size_t j=0; j<edge->getNoLanes(); j++) {
                        e.lanes.push_back(j);
                    }
                    myExpansions.push_back(e);
                }
                i = find_if(myExpansions.begin(), myExpansions.end(), expansion_by_pos_finder((SUMOReal) forcedLength));
                std::vector<int>::iterator k = find((*i).lanes.begin(), (*i).lanes.end(), lane);
                if (k!=(*i).lanes.end()) {
                    (*i).lanes.erase(k);
                }
            } catch (NumberFormatException &) {}
            catch (EmptyData &) {}
        }
        /*
        // to split?
        if (forcedLength!=0) {
            // maybe the edge has already been split at this position
            string nid = myCurrentID + "/" +  toString(forcedLength);
            string pid;
            if (myNodeCont.retrieve(nid)==0) {
                SUMOReal splitLength = forcedLength;
                // split only if not
                //  get the proper edge - it may be a previously splitted edge
                bool toNext = true;
                do {
                    toNext = true;
                    if (edge->getToNode()->getOutgoingEdges().size()!=1) {
                        toNext = false;
                        break;
                    }
                    string nextID = edge->getToNode()->getOutgoingEdges()[0]->getID();
                    if (nextID.substr(0, myCurrentID.length()+1)!=myCurrentID+"/") {
                        toNext = false;
                        break;
                    }
                    int dist = TplConvert<char>::_2int(nextID.substr(myCurrentID.length()+1).c_str());
                    if (forcedLength>dist) {
                        toNext = false;
                        splitLength -= dist;
                        break;
                    } else {}
                    if (toNext) {
                        edge = edge->getToNode()->getOutgoingEdges()[0];
                    }
                } while (toNext);

                //  build the node
                if (edge->getGeometry().length()-splitLength>0) {
                    Position2D p = edge->getGeometry().positionAtLengthPosition(edge->getGeometry().length() - splitLength);
                    pid = edge->getID();
                    NBNode *rn = new NBNode(nid, p);
                    if (myNodeCont.insert(rn)) {
                        //  split the edge
                        myEdgeCont.splitAt(myDistrictCont, edge, edge->getGeometry().length()-splitLength, rn,
                                           pid, nid, edge->getNoLanes(), edge->getNoLanes());
                    } else {
                        // hmm, the node could not be build!?
                        delete rn;
                        addError("Could not insert node '" + nid + "' for edge splitting.");
                        return;
                    }
                    NBEdge *e = myEdgeCont.retrieve(nid);
                    bool cont = true;
                    do {
                        cont = false;
                        const EdgeVector &ev = e->getFromNode()->getIncomingEdges();
                        if (ev.size()==1) {
                            NBEdge *prev = ev[0];
                            string idp = prev->getID();
                            string idp2 = idp.substr(0, idp.find('/'));
                            string idc = e->getID();
                            string idc2 = idc.substr(0, idc.find('/'));
                            if (idp2==idc2) {
                                e = prev;
                                if (prev->getNoLanes()>1) {
                                    prev->decLaneNo(1);
                                } else {
                                    MsgHandler::getWarningInstance()->inform("Could not split edge '" + prev->getID() + "'.");
                                    return;
                                }
                                cont = true;
                            }
                        }
                    } while (cont);
                } else {
                    MsgHandler::getWarningInstance()->inform("Could not split edge '" + edge->getID() + "'.");
                    return;
                }
                // set proper connections
                NBEdge *ce = myEdgeCont.retrieve(nid);
                NBEdge *pe = myEdgeCont.retrieve(pid);
                if((SUMOReal) lane<(SUMOReal) priorLaneNo/(SUMOReal) 2) {
                    // new lane on the right side
                    pe->addLane2LaneConnections(0, ce, 1, pe->getNoLanes(), false, true);
                    pe->addLane2LaneConnection(0, ce, 0, false, false);
                } else {
                    // new lane on the left side
                    pe->addLane2LaneConnections(0, ce, 0, pe->getNoLanes(), false, true);
                    pe->addLane2LaneConnection(pe->getNoLanes()-1, ce, ce->getNoLanes()-1, false, false);
                }
            } else {
                pid = myNodeCont.retrieve(nid)->getIncomingEdges()[0]->getID();
                // set proper connections
                NBEdge *ce = myEdgeCont.retrieve(nid);
                NBEdge *pe = myEdgeCont.retrieve(pid);
                if (pe->getNoLanes()>1) {
                    if((SUMOReal) lane<(SUMOReal) priorLaneNo/(SUMOReal) 2) {
                        pe->decLaneNo(1, 1);
                        int to = MAX2(pe->getMinConnectedLane(ce)-1, 0);
                        //pe->addLane2LaneConnection(0, ce, to, false, false);
                    } else {
                        pe->decLaneNo(1, -1);
                        int to = MIN2(pe->getMaxConnectedLane(ce)+1, (int) ce->getNoLanes()-1);
                        //pe->addLane2LaneConnection(pe->getNoLanes()-1, ce, to, false, false);
                    }
                    pe->invalidateConnections(true);
                } else {
                    MsgHandler::getWarningInstance()->inform("Could not split edge '" + pe->getID() + "'.");
                    return;
                }
            }
        }
        */
    }
    if (element==SUMO_TAG_EXPANSION) {
        try {
            Expansion e;
            e.pos = getFloat(attrs, SUMO_ATTR_POS);
            NBEdge *edge = myEdgeCont.retrieve(myCurrentID);
            if (edge==0) {
                if (!OptionsSubSys::helper_CSVOptionMatches("remove-edges", myCurrentID)) {
                    MsgHandler::getErrorInstance()->inform("Additional lane information could not been set - the edge with id '" + myCurrentID + "' is not known.");
                }
                return;
            }
            if (e.pos<0) {
                e.pos = edge->getGeometry().length() + e.pos;
            }
            myExpansions.push_back(e);
        } catch (EmptyData&) {
            MsgHandler::getErrorInstance()->inform("The position of an expansion is missing (edge '" + myCurrentID + "').");
        } catch (NumberFormatException&) {
            MsgHandler::getErrorInstance()->inform("The position of an expansion is not numeric (edge '" + myCurrentID + "').");
        }
    }
}


void
NIXMLEdgesHandler::setID(const Attributes &attrs)
{
    myCurrentID = "";
    try {
        myCurrentID = getString(attrs, SUMO_ATTR_ID);
        if(myCurrentID=="") {
            throw EmptyData();
        }
    } catch (EmptyData &) {
        if(_options.getBool("omit-corrupt-edges")) {
            MsgHandler::getWarningInstance()->inform("Missing edge id.");
        } else {
            MsgHandler::getErrorInstance()->inform("Missing edge id.");
        }
    }
}


void
NIXMLEdgesHandler::setName(const Attributes &attrs)
{
    if (hasAttribute(attrs, SUMO_ATTR_NAME)) {
        myCurrentName = getString(attrs, SUMO_ATTR_NAME);
    } else {
        myCurrentName = myCurrentID;
    }
}


void
NIXMLEdgesHandler::checkType(const Attributes &attrs)
{
    // try to get the type and maybe to overwrite default values for speed, priority and th enumber of lanes
    myCurrentEdgeFunction = NBEdge::EDGEFUNCTION_NORMAL;
    myCurrentType = "";
    if (hasAttribute(attrs, SUMO_ATTR_TYPE)) {
        myCurrentType = getString(attrs, SUMO_ATTR_TYPE);
        if(myCurrentType=="") {
            MsgHandler::getErrorInstance()->inform("Edge '" + myCurrentID + "' has an empty type.");
            return;
        }
        if(!myTypeCont.knows(myCurrentType)) {
            MsgHandler::getErrorInstance()->inform("Type '" + myCurrentType + "' used by edge '" + myCurrentID + "' was not defined.");
            return;
        }
        myCurrentSpeed = myTypeCont.getSpeed(myCurrentType);
        myCurrentPriority = myTypeCont.getPriority(myCurrentType);
        myCurrentLaneNo = myTypeCont.getNoLanes(myCurrentType);
        myCurrentEdgeFunction = myTypeCont.getFunction(myCurrentType);
    }
}


void
NIXMLEdgesHandler::setGivenSpeed(const Attributes &attrs)
{
    if (!hasAttribute(attrs, SUMO_ATTR_SPEED)) {
        if (_options.getBool("speed-in-kmh")) {
            myCurrentSpeed = myCurrentSpeed / (SUMOReal) 3.6;
        }
        return;
    }
    try {
        myCurrentSpeed = getFloat(attrs, SUMO_ATTR_SPEED);
        if (_options.getBool("speed-in-kmh")) {
            myCurrentSpeed = myCurrentSpeed / (SUMOReal) 3.6;
        }
    } catch (NumberFormatException &) {
        MsgHandler::getErrorInstance()->inform("Not numeric value for speed in edge '" + myCurrentID + "'.");
    } catch (EmptyData &) {
        MsgHandler::getErrorInstance()->inform("Empty speed definition in edge '" + myCurrentID + "'.");
    }
}


void
NIXMLEdgesHandler::setGivenLanes(const Attributes &attrs)
{
    if (!hasAttribute(attrs, SUMO_ATTR_NOLANES)) {
        return;
    }
    // try to get the number of lanes
    try {
        myCurrentLaneNo = getInt(attrs, SUMO_ATTR_NOLANES);
    } catch (NumberFormatException &) {
        MsgHandler::getErrorInstance()->inform("The lane number is not numeric in edge '" + myCurrentID + "'.");
    } catch (EmptyData &) {
        MsgHandler::getErrorInstance()->inform("Empty lane number definition in edge '" + myCurrentID + "'.");
    }
}


void
NIXMLEdgesHandler::setGivenPriority(const Attributes &attrs)
{
    if (!hasAttribute(attrs, SUMO_ATTR_PRIORITY)) {
        return;
    }
    // try to retrieve given priority
    try {
        myCurrentPriority = getInt(attrs, SUMO_ATTR_PRIORITY);
    } catch (NumberFormatException &) {
        MsgHandler::getErrorInstance()->inform("Not numeric value for priority in edge '" + myCurrentID + "'.");
    } catch (EmptyData &) {
        MsgHandler::getErrorInstance()->inform("Empty priority definition in edge '" + myCurrentID + "'.");
    }
}


void
NIXMLEdgesHandler::setGivenType(const Attributes &attrs)
{
    if (!hasAttribute(attrs, SUMO_ATTR_FUNC)) {
        return;
    }
    // try to get the tpe
    string func = getStringSecure(attrs, SUMO_ATTR_FUNC, "");
    if (func=="") {
        MsgHandler::getErrorInstance()->inform("Empty edge function in edge '" + myCurrentID + "'.");
        return;
    }
    if (func=="source") {
        myCurrentEdgeFunction = NBEdge::EDGEFUNCTION_SOURCE;
    } else if (func=="sink") {
        myCurrentEdgeFunction = NBEdge::EDGEFUNCTION_SINK;
    } else if (func!="normal") {
        MsgHandler::getErrorInstance()->inform("Unknown edge function '" + func + "' in edge '" + myCurrentID + "'.");
    }
}


bool
NIXMLEdgesHandler::setNodes(const Attributes &attrs)
{
    // the names and the coordinates of the beginning and the end node
    // may be found, try
    string begNodeID = getStringSecure(attrs, SUMO_ATTR_FROMNODE, "");
    string endNodeID = getStringSecure(attrs, SUMO_ATTR_TONODE, "");
    // or their positions
    SUMOReal begNodeXPos = tryGetPosition(attrs, SUMO_ATTR_XFROM, "XFrom");
    SUMOReal begNodeYPos = tryGetPosition(attrs, SUMO_ATTR_YFROM, "YFrom");
    SUMOReal endNodeXPos = tryGetPosition(attrs, SUMO_ATTR_XTO, "XTo");
    SUMOReal endNodeYPos = tryGetPosition(attrs, SUMO_ATTR_YTO, "YTo");
    if (begNodeXPos!=SUMOXML_INVALID_POSITION&&begNodeYPos!=SUMOXML_INVALID_POSITION) {
        Position2D pos(begNodeXPos, begNodeYPos);
        GeoConvHelper::x2cartesian(pos);
        begNodeXPos = pos.x();
        begNodeYPos = pos.y();
    }
    if (endNodeXPos!=SUMOXML_INVALID_POSITION&&endNodeYPos!=SUMOXML_INVALID_POSITION) {
        Position2D pos(endNodeXPos, endNodeYPos);
        GeoConvHelper::x2cartesian(pos);
        endNodeXPos = pos.x();
        endNodeYPos = pos.y();
    }
    // check the obtained values for nodes
    MsgHandler *msgh = _options.getBool("omit-corrupt-edges")
                       ? MsgHandler::getWarningInstance()
                       : MsgHandler::getErrorInstance();
    myFromNode = insertNodeChecking(*msgh, Position2D(begNodeXPos, begNodeYPos), begNodeID, "from");
    myToNode = insertNodeChecking(*msgh, Position2D(endNodeXPos, endNodeYPos), endNodeID, "to");
    return myFromNode!=0&&myToNode!=0;
}


SUMOReal
NIXMLEdgesHandler::tryGetPosition(const Attributes &attrs, SumoXMLAttr attrID,
                                  const std::string &attrName)
{
    try {
        return getFloatSecure(attrs, attrID, SUMOXML_INVALID_POSITION);
    } catch (NumberFormatException &) {
        MsgHandler::getErrorInstance()->inform("Not numeric value for " + attrName + " (at tag ID='" + myCurrentID + "').");
        return SUMOXML_INVALID_POSITION;
    }
}


NBNode *
NIXMLEdgesHandler::insertNodeChecking(MsgHandler &msgh, const Position2D &pos, 
                                      const std::string &name, const std::string &dir)
{
    NBNode *ret = 0;
    if(name=="" && (pos.x()==SUMOXML_INVALID_POSITION || pos.y()==SUMOXML_INVALID_POSITION)) {
        msgh.inform("Neither the name nor the position of the " + dir + "-node is given for edge '" + myCurrentID + "'.");
        return ret;
    }
    if(name!="") {
        if (pos.x()!=SUMOXML_INVALID_POSITION && pos.y()!=SUMOXML_INVALID_POSITION) {
            // the node is named and it has a position given
            if(!myNodeCont.insert(name, pos)) {
                msgh.inform("Position of " + dir + "-node '" + name + "' mismatches previous positions.");
                return 0;
            }
        }
        // the node is given by its name
        ret = myNodeCont.retrieve(name);
        if(ret==0) {
            msgh.inform("Edge's '" + myCurrentID + "' " + dir + "-node '" + name + "' is not known.");
        }
    } else {
        ret = myNodeCont.retrieve(pos);
        if (ret==0) {
            ret = new NBNode(myNodeCont.getFreeID(), pos);
            if (!myNodeCont.insert(ret)) {
                msgh.inform("Could not insert " + dir + "-node at position " + toString(pos) + ".");
                delete ret;
                return 0;
            }
        }
    }
    return ret;
}


void
NIXMLEdgesHandler::setLength(const Attributes &attrs)
{
    // get the length or compute it
    if (hasAttribute(attrs, SUMO_ATTR_LENGTH)) {
        try {
            myLength = getFloat(attrs, SUMO_ATTR_LENGTH);
        } catch (NumberFormatException &) {
            MsgHandler::getErrorInstance()->inform("Not numeric value for length in edge '" + myCurrentID + "'.");
        } catch (EmptyData &) {
            MsgHandler::getErrorInstance()->inform("Empty length definition of edge '" + myCurrentID + "'.");
        }
    } else {
        myLength = 0;
    }
}



Position2DVector
NIXMLEdgesHandler::tryGetShape(const Attributes &attrs)
{
    if (!hasAttribute(attrs, SUMO_ATTR_SHAPE)) {
        return Position2DVector();
    }
    // try to build shape
    try {
        string shpdef = getStringSecure(attrs, SUMO_ATTR_SHAPE, "");
        Position2DVector shape1 = GeomConvHelper::parseShape(shpdef);
        Position2DVector shape;
        for (size_t i=0; i<shape1.size(); ++i) {
            Position2D pos(shape1[i]);
            GeoConvHelper::x2cartesian(pos);
            shape.push_back(pos);
        }

        if (shape.size()==1) {
            MsgHandler::getErrorInstance()->inform("The shape of edge '" + myCurrentID + "' has only one entry.");
        }
        return shape;
    } catch (EmptyData &) {
        MsgHandler::getErrorInstance()->inform("At least one number is missing in shape definition for edge '" + myCurrentID + "'.");
    } catch (NumberFormatException &) {
        MsgHandler::getErrorInstance()->inform("A non-numeric value occured in shape definition for edge '" + myCurrentID + "'.");
    }
    return Position2DVector();
}


NBEdge::LaneSpreadFunction
NIXMLEdgesHandler::getSpreadFunction(const Attributes &attrs)
{
    if (getStringSecure(attrs, SUMO_ATTR_SPREADFUNC, "")=="center") {
        return NBEdge::LANESPREAD_CENTER;
    }
    return NBEdge::LANESPREAD_RIGHT;
}


void
NIXMLEdgesHandler::myCharacters(SumoXMLTag element,
                                const std::string &chars) throw(ProcessError)
{
    if (element==SUMO_TAG_EXPANSION) {
        if (myExpansions.size()!=0) {
            Expansion &e = myExpansions.back();
            StringTokenizer st(chars, ";");
            while (st.hasNext()) {
                try {
                    int lane = TplConvert<char>::_2int(st.next().c_str());
                    e.lanes.push_back(lane);
                } catch (NumberFormatException &) {
                    MsgHandler::getErrorInstance()->inform("Error on parsing an expansion (edge '" + myCurrentID + "').");
                } catch (EmptyData &) {
                    MsgHandler::getErrorInstance()->inform("Error on parsing an expansion (edge '" + myCurrentID + "').");
                }
            }
        } else {
            MsgHandler::getErrorInstance()->inform("Error on parsing an expansion (edge '" + myCurrentID + "').");
        }
    }
}


void
NIXMLEdgesHandler::myEndElement(SumoXMLTag element) throw(ProcessError)
{
    if (element==SUMO_TAG_EDGE) {
        if (myExpansions.size()!=0) {
            std::vector<Expansion>::iterator i, i2;
            sort(myExpansions.begin(), myExpansions.end(), expansions_sorter());
            NBEdge *e = myEdgeCont.retrieve(myCurrentID);
            // compute the node positions and sort the lanes
            for (i=myExpansions.begin(); i!=myExpansions.end(); ++i) {
                (*i).gpos = e->getGeometry().positionAtLengthPosition((*i).pos);
                sort((*i).lanes.begin(), (*i).lanes.end());
            }
            // patch lane information
            //  !!! hack: normally, all lanes to remove should always be supplied
            for (i=myExpansions.begin(); i!=myExpansions.end(); ++i) {
                vector<int>::iterator k;
                vector<int> lanes;
                for (size_t l=0; l<e->getNoLanes(); ++l) {
                    if (find((*i).lanes.begin(), (*i).lanes.end(), l)==(*i).lanes.end()) {
                        lanes.push_back(l);
                    }
                }
                for (i2=i+1; i2!=myExpansions.end(); ++i2) {
                    for (vector<int>::iterator k=lanes.begin(); k!=lanes.end(); ++k) {
                        if (find((*i2).lanes.begin(), (*i2).lanes.end(), *k)!=(*i2).lanes.end()) {
                            (*i2).lanes.erase(find((*i2).lanes.begin(), (*i2).lanes.end(), *k));
                        }
                    }
                }
            }
            // split the edge
            size_t lastRightLane = 0;
            for (i=myExpansions.begin(); i!=myExpansions.end(); ++i) {
                const Expansion &exp = *i;
                assert(exp.lanes.size()!=0);
                if (exp.pos>0 && e->getGeometry().length()-exp.pos>0) {
                    string nid = myCurrentID + "/" +  toString(exp.nameid);
                    NBNode *rn = new NBNode(nid, exp.gpos);
                    if (myNodeCont.insert(rn)) {
                        //  split the edge
                        string nid = myCurrentID + "/" +  toString(exp.nameid);
                        string pid = myCurrentID;
                        myEdgeCont.splitAt(myDistrictCont, e, exp.pos, rn,
                                           pid, nid, e->getNoLanes(), e->getNoLanes());
                        NBEdge *pe = myEdgeCont.retrieve(pid);
                        NBEdge *ne = myEdgeCont.retrieve(nid);
                        pe->decLaneNo(ne->getNoLanes()-exp.lanes.size());
                        // reconnect lanes
                        // how many lanes on the right are missing?
                        int off = exp.lanes[0] - lastRightLane;
                        lastRightLane = exp.lanes[0];
                        pe->addLane2LaneConnections(0, ne, off, pe->getNoLanes(), false, true);
                        // add to the right?
                        if (off>0) {
                            pe->addLane2LaneConnection(0, ne, off-1, false, false);
                        }
                        // add to the left?
                        if (off+exp.lanes.size()<ne->getNoLanes()) {
                            pe->addLane2LaneConnection(pe->getNoLanes()-1, ne, off+exp.lanes.size(), false, false);
                        }
                        // move to next
                        e = pe;
                    } else {
                        MsgHandler::getWarningInstance()->inform("Error on parsing an expansion (edge '" + myCurrentID + "').");
                    }
                } else {
                    MsgHandler::getWarningInstance()->inform("Expansion at '" + toString(exp.pos) + "' lies beyond the edge's length (edge '" + myCurrentID + "').");
                }
            }
        }
    }
}



/****************************************************************************/

