/***************************************************************************
                          NBNode.cpp
			  The representation of a single node
                             -------------------
    project              : SUMO
    subproject           : netbuilder / netconverter
    begin                : Tue, 20 Nov 2001
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
// Revision 1.33  2003/10/17 11:21:10  dkrajzew
// error on trying to build a geometry for a emoty node removed
//
// Revision 1.32  2003/10/15 11:48:13  dkrajzew
// geometry computation corrigued partially
//
// Revision 1.31  2003/10/14 11:35:28  roessel
// Declared variable i before the loop because it is accessed afterwards.
//
// Revision 1.30  2003/10/06 07:46:12  dkrajzew
// further work on vissim import (unsignalised vs. signalised streams modality cleared & lane2lane instead of edge2edge-prohibitions implemented
//
// Revision 1.28  2003/09/22 12:40:12  dkrajzew
// further work on vissim-import
//
// Revision 1.27  2003/09/05 15:16:57  dkrajzew
// umlaute conversion; node geometry computation; internal links computation
//
// Revision 1.26  2003/08/21 12:55:20  dkrajzew
// directional information patched
//
// Revision 1.25  2003/08/20 11:53:18  dkrajzew
// further work on node geometry
//
// Revision 1.24  2003/08/14 13:50:15  dkrajzew
// new junction shape computation implemented
//
// Revision 1.23  2003/07/30 09:21:11  dkrajzew
// added the generation about link directions and priority
//
// Revision 1.22  2003/07/16 15:32:02  dkrajzew
// some work on the geometry of nodes
//
// Revision 1.21  2003/07/07 08:22:42  dkrajzew
// some further refinements due to the new 1:N traffic lights and usage of geometry information
//
// Revision 1.20  2003/06/24 08:21:01  dkrajzew
// some further work on importing traffic lights
//
// Revision 1.19  2003/06/18 11:13:13  dkrajzew
// new message and error processing: output to user may be a message, warning or an error now; it is reported to a Singleton (MsgHandler); this handler puts it further to output instances. changes: no verbose-parameter needed; messages are exported to singleton
//
// Revision 1.18  2003/06/16 08:02:44  dkrajzew
// further work on Vissim-import
//
// Revision 1.17  2003/06/05 11:43:35  dkrajzew
// class templates applied; documentation added
//
// Revision 1.16  2003/05/20 09:33:47  dkrajzew
// false computation of yielding on lane ends debugged; some debugging on tl-import; further work on vissim-import
//
// Revision 1.15  2003/04/16 10:03:47  dkrajzew
// further work on Vissim-import
//
// Revision 1.14  2003/04/10 15:45:18  dkrajzew
// some lost changes reapplied
//
// Revision 1.13  2003/04/07 12:15:40  dkrajzew
// first steps towards a junctions geometry; tyellow removed again, traffic lights have yellow times given explicitely, now
//
// Revision 1.12  2003/04/04 07:43:03  dkrajzew
// Yellow phases must be now explicetely given; comments added; order of edge sorting (false lane connections) debugged
//
// Revision 1.11  2003/04/01 15:15:52  dkrajzew
// further work on vissim-import
//
// Revision 1.10  2003/03/17 14:22:33  dkrajzew
// further debug and windows eol removed
//
// Revision 1.9  2003/03/12 16:47:54  dkrajzew
// extension for artemis-import
//
// Revision 1.8  2003/03/06 17:18:41  dkrajzew
// debugging during vissim implementation
//
// Revision 1.7  2003/03/03 14:59:06  dkrajzew
// debugging; handling of imported traffic light definitions
//
// Revision 1.6  2003/02/13 15:51:54  dkrajzew
// functions for merging edges with the same origin and destination added
//
// Revision 1.5  2003/02/07 10:43:44  dkrajzew
// updated
//
// Revision 1.4  2002/10/22 10:08:28  dkrajzew
// unsupported return value patched
//
// Revision 1.3  2002/10/18 11:47:55  dkrajzew
// edges must only be sorted, when at least 2 are there...
//
// Revision 1.2  2002/10/17 13:40:10  dkrajzew
// typing of nodes during loading allwoed
//
// Revision 1.1  2002/10/16 15:48:13  dkrajzew
// initial commit for net building classes
//
// Revision 1.11  2002/07/25 08:38:27  dkrajzew
// problems on determination of a nodes type patched
//
// Revision 1.10  2002/07/02 09:01:41  dkrajzew
// Output of outgoing edges needed for the router added
//
// Revision 1.9  2002/06/21 13:17:51  dkrajzew
// Lane division bug (forgetting the turning direction; cause of segmentation violations under Linux) fixed
//
// Revision 1.8  2002/06/11 16:00:41  dkrajzew
// windows eol removed; template class definition inclusion depends now on the EXTERNAL_TEMPLATE_DEFINITION-definition
//
// Revision 1.7  2002/06/07 14:58:45  dkrajzew
// Bugs on dead ends and junctions with too few outgoing roads fixed; Comments improved
//
// Revision 1.6  2002/05/14 04:42:55  dkrajzew
// new computation flow
//
// Revision 1.5  2002/04/26 10:07:11  dkrajzew
// Windows eol removed; minor double to int conversions removed;
//
// Revision 1.4  2002/04/25 14:16:57  dkrajzew
// The unneeded iterator definition was again removed
//
// Revision 1.3  2002/04/25 14:15:07  dkrajzew
// The assignement of priorities of incoming edges improved; now, if having equal priorities, the opposite edges are chosen as higher priorised
//
// Revision 1.1.1.1  2002/04/09 14:18:27  dkrajzew
// new version-free project name (try2)
//
// Revision 1.3  2002/03/22 10:50:03  dkrajzew
// Memory leaks debugging added (MSVC++)
//
// Revision 1.2  2002/03/15 12:39:40  dkrajzew
// Handling of map logics removed
//
// Revision 1.1.1.1  2002/02/19 15:33:04  traffic
// Initial import as a separate application.
//
// Revision 1.1  2001/12/06 13:37:59  traffic
// files for the netbuilder
//
//
/* =========================================================================
 * debugging definitions (MSVC++ only)
 * ======================================================================= */
#ifdef _DEBUG
   #define _CRTDBG_MAP_ALLOC // include Microsoft memory leak detection
   #define _INC_MALLOC	     // exclude standard memory alloc procedures
#endif


/* =========================================================================
 * included modules
 * ======================================================================= */
#include <string>
#include <map>
#include <cassert>
#include <algorithm>
#include <vector>
#include <deque>
#include <iostream>
#include <set>
#include <utils/common/UtilExceptions.h>
#include <utils/common/StringUtils.h>
#include <utils/options/OptionsCont.h>
#include <utils/options/OptionsSubSys.h>
#include <utils/geom/Line2D.h>
#include <utils/geom/GeomHelper.h>
#include <utils/common/MsgHandler.h>
#include <utils/common/StdDefs.h>
#include <utils/convert/ToString.h>
#include <iomanip>
#include "NBNode.h"
#include "NBNodeCont.h"
#include "NBEdgeCont.h"
#include "NBTypeCont.h"
#include "NBJunctionLogicCont.h"
#include "NBHelpers.h"
#include "NBDistrict.h"
#include "NBContHelper.h"
#include "NBLogicKeyBuilder.h"
#include "NBRequest.h"
#include "NBOwnTLDef.h"
#include "NBTrafficLightLogicCont.h"


/* =========================================================================
 * used namespaces
 * ======================================================================= */
using namespace std;


/* =========================================================================
 * static variable definitions
 * ======================================================================= */
int NBNode::_noDistricts = 0;
int NBNode::_noNoJunctions = 0;
int NBNode::_noPriorityJunctions = 0;
int NBNode::_noRightBeforeLeftJunctions = 0;


/* =========================================================================
 * method definitions
 * ======================================================================= */
/* -------------------------------------------------------------------------
 * NBNode::ApproachingDivider-methods
 * ----------------------------------------------------------------------- */
NBNode::ApproachingDivider::ApproachingDivider(
    std::vector<NBEdge*> *approaching, NBEdge *currentOutgoing)
    : _approaching(approaching), _currentOutgoing(currentOutgoing)
{
    // check whether origin lanes have been given
    assert(_approaching!=0);
}


NBNode::ApproachingDivider::~ApproachingDivider()
{
}


void
NBNode::ApproachingDivider::execute(double src, double dest)
{
    assert(_approaching->size()>src);
    // get the origin edge
    NBEdge *incomingEdge = (*_approaching)[(int) src];
    if(incomingEdge->getStep()==NBEdge::LANES2LANES) {
        return;
    }
    vector<size_t> approachingLanes =
        incomingEdge->getConnectionLanes(_currentOutgoing);
    assert(approachingLanes.size()!=0);
    deque<size_t> *approachedLanes = spread(approachingLanes, dest);
    assert(approachedLanes->size()<=_currentOutgoing->getNoLanes());
    // set lanes
    for(size_t i=0; i<approachedLanes->size(); i++) {
        size_t approached = (*approachedLanes)[i];
	    assert(approachedLanes->size()>i);
	    assert(approachingLanes.size()>i);
        incomingEdge->setConnection(approachingLanes[i], _currentOutgoing,
            approached);
    }
    delete approachedLanes;
}


deque<size_t> *
NBNode::ApproachingDivider::spread(const vector<size_t> &approachingLanes,
                                   double dest) const
{
    deque<size_t> *ret = new deque<size_t>();
    size_t noLanes = approachingLanes.size();
    // when only one lane is approached, we check, whether the double-value
    //  is assigned more to the left or right lane
    if(noLanes==1) {
        if((size_t) (dest+0.5)>(size_t) dest) {
            ret->push_back((size_t) dest+1);
        } else {
            ret->push_back((size_t) dest);
        }
        return ret;
    }

    size_t noOutgoingLanes = _currentOutgoing->getNoLanes();
    //
    ret->push_back((size_t) dest);
    size_t noSet = 1;
    double roffset = 1;
    double loffset = 1;
    while(noSet<noLanes) {
        // It may be possible, that there are not enough lanes the source
        //  lanes may be divided on
        //  In this case, they remain unset
        //  !!! this is only a hack. It is possible, that this yields in
        //   uncommon divisions
        if(noOutgoingLanes==noSet)
            return ret;

        // as due to the conversion of double->uint the numbers will be lower
        //  than they should be, we try to append to the left side first
        //
        // check whether the left boundary of the approached street has
        //  been overridden; if so, move all lanes to the right
        if(((size_t) dest+loffset)>=noOutgoingLanes) {
            loffset -= 1;
            roffset += 1;
            for(size_t i=0; i<ret->size(); i++) {
                (*ret)[i] = (*ret)[i] - 1;
            }
        }
        // append the next lane to the left of all edges
        //  increase the position (destination edge)
        ret->push_back((size_t) (dest+loffset));
        noSet++;
        loffset += 1;

        // as above
        if(noOutgoingLanes==noSet)
            return ret;

        // now we try to append the next lane to the right side, when needed
        if(noSet<noLanes) {
            // check whether the right boundary of the approached street has
            //  been overridden; if so, move all lanes to the right
            if(((size_t) dest-roffset)<0) {
                loffset += 1;
                roffset -= 1;
                for(size_t i=0; i<ret->size(); i++) {
                    (*ret)[i] = (*ret)[i] + 1;
                }
            }
            ret->push_front((size_t) (dest-roffset));
            noSet++;
            roffset += 1;
        }
    }
    return ret;
}




/* -------------------------------------------------------------------------
 * NBNode-methods
 * ----------------------------------------------------------------------- */
NBNode::NBNode(const string &id, double x, double y)
    : _id(StringUtils::convertUmlaute(id)), _x(x), _y(y),
    _type(NODETYPE_UNKNOWN), myDistrict(0), _request(0)
{
    _incomingEdges = new EdgeVector();
    _outgoingEdges = new EdgeVector();
}

NBNode::NBNode(const string &id, double x, double y,
               BasicNodeType type)
    : _id(StringUtils::convertUmlaute(id)), _x(x), _y(y),
    _type(type), myDistrict(0), _request(0)
{
    _incomingEdges = new EdgeVector();
    _outgoingEdges = new EdgeVector();
}

NBNode::NBNode(const string &id, double x, double y, NBDistrict *district)
    : _id(StringUtils::convertUmlaute(id)), _x(x), _y(y),
    _type(NODETYPE_DISTRICT), myDistrict(district), _request(0)
{
    _incomingEdges = new EdgeVector();
    _outgoingEdges = new EdgeVector();
}


NBNode::~NBNode()
{
    delete _incomingEdges;
    delete _outgoingEdges;
    delete _request;
}


double
NBNode::getXCoordinate() {
    return _x;
}


double
NBNode::getYCoordinate() {
    return _y;
}


Position2D
NBNode::geomPosition() const
{
    return Position2D(_x, _y);
}




void
NBNode::addIncomingEdge(NBEdge *edge)
{
    assert(edge!=0);
    if( find(_incomingEdges->begin(), _incomingEdges->end(), edge)
        ==_incomingEdges->end()) {

        _incomingEdges->push_back(edge);
    }
}


void
NBNode::addOutgoingEdge(NBEdge *edge)
{
    assert(edge!=0);
    if( find(_outgoingEdges->begin(), _outgoingEdges->end(), edge)
        ==_outgoingEdges->end()) {

        _outgoingEdges->push_back(edge);
    }
}


string
NBNode::getID() const
{
    return _id;
}


const EdgeVector &
NBNode::getIncomingEdges() const
{
    return *_incomingEdges;
}


const EdgeVector &
NBNode::getOutgoingEdges() const
{
    return *_outgoingEdges;
}


const EdgeVector *
NBNode::getEdges()
{
    return &_allEdges;
}


void
NBNode::buildList()
{
    copy(_incomingEdges->begin(), _incomingEdges->end(),
        back_inserter(_allEdges));
    copy(_outgoingEdges->begin(), _outgoingEdges->end(),
        back_inserter(_allEdges));
}


void
NBNode::sortSmall()
{
    if(_allEdges.size()==0) {
        return;
    }
    vector<NBEdge*>::iterator i;
    for( i=_allEdges.begin();
         i!=_allEdges.end()-1&&i!=_allEdges.end(); i++) {
        swapWhenReversed(i, i+1);
    }
    if(_allEdges.size()>1 && i!=_allEdges.end()) {
        swapWhenReversed(_allEdges.end()-1, _allEdges.begin());
    }
}


bool
NBNode::swapWhenReversed(const vector<NBEdge*>::iterator &i1,
                         const vector<NBEdge*>::iterator &i2)
{
    NBEdge *e1 = *i1;
    NBEdge *e2 = *i2;
    if(e2->isTurningDirection(e1) && e2->getToNode()==this) {
        swap(*i1, *i2);
        return true;
    }
    return false;
}

void
NBNode::setPriorities()
{
    // reset all priorities
    vector<NBEdge*>::iterator i;
    // check if the junction is not a real junction
    if(_incomingEdges->size()==1&&_outgoingEdges->size()==1) {
        for(i=_allEdges.begin(); i!=_allEdges.end(); i++) {
            (*i)->setJunctionPriority(this, 1);
        }
        return;
    }
    // preset all junction's edge priorities to zero
    for(i=_allEdges.begin(); i!=_allEdges.end(); i++) {
        (*i)->setJunctionPriority(this, 0);
    }
    // compute the priorities on junction when needed
    if(_type!=NODETYPE_RIGHT_BEFORE_LEFT) {
        setPriorityJunctionPriorities();
    }
}


NBNode::BasicNodeType
NBNode::computeType() const
{
    // the type may already be set from the data
    if(_type!=NODETYPE_UNKNOWN) {
        return _type;
    }
    // check whether the junction is not a real junction
    if(_incomingEdges->size()==1/*&&_outgoingEdges->size()==1*/) {
        return NODETYPE_PRIORITY_JUNCTION; // !!! no junction?
    }
    // check whether the junction is a district and has no
    //  special meaning
    if(isDistrictCenter()) {
        return NODETYPE_NOJUNCTION;
    }
    // choose the uppermost type as default
    BasicNodeType type = NODETYPE_RIGHT_BEFORE_LEFT;
    // determine the type
    for( vector<NBEdge*>::const_iterator i=_allEdges.begin();
         i!=_allEdges.end(); i++) {
        for(vector<NBEdge*>::const_iterator j=i+1; j!=_allEdges.end(); j++) {
            // !!!
            // This usage of defaults is not very well, still we do not have any
            //  methods for the conversion of foreign, sometimes not supplied
            //  road types ino an own format
            // As default, TYPE_TRAFFIC_LIGHT is used, this should be valid for
            //  most coarse networks
            // !!!
            BasicNodeType tmptype = NODETYPE_PRIORITY_JUNCTION;
            try {
                tmptype = NBTypeCont::getJunctionType(
                    (*i)->getPriority(),
                    (*j)->getPriority());
            } catch (OutOfBoundsException) {
            }
            if(tmptype<type&&tmptype!=NODETYPE_UNKNOWN&&tmptype!=NODETYPE_NOJUNCTION) {
                type = tmptype;
            }
        }
    }
    return type;
}


bool
NBNode::isDistrictCenter() const
{
    return _id.substr(0, 14)=="DistrictCenter";
}


void
NBNode::setPriorityJunctionPriorities()
{
    if(_incomingEdges->size()==0) {
        return; // !!! what happens with outgoing edges
                //  (which priority should be assigned here)?
    }
    NBEdge *best1, *best2;
    best1 = best2 = 0;
    best1 = 0;
    vector<NBEdge*> incoming(*_incomingEdges);
    int noIncomingPrios = NBContHelper::countPriorities(incoming);
    if(noIncomingPrios==1) {
        return;
    }
    // !!! Attention!
    // there is no case that fits into junctions with no incoming edges
        // extract the edge with the highest priority
    if(incoming.size()>0) {
        sort(incoming.begin(), incoming.end(),
            NBContHelper::edge_by_priority_sorter());
        best1 = extractAndMarkFirst(incoming);
    }
        // check whether a second main road exists
    if(incoming.size()>0) {
        noIncomingPrios = NBContHelper::countPriorities(incoming);
        if(noIncomingPrios>1) {
            sort(incoming.begin(), incoming.end(),
                NBContHelper::edge_by_priority_sorter());
            best2 = extractAndMarkFirst(incoming);
        } /*else {
            sort(incoming.begin(), incoming.end(),
                NBContHelper::edge_opposite_direction_sorter(best1));
            best2 = extractAndMarkFirst(incoming);
        }*/
    }
    // get the best continuations (outgoing edges)
    NBEdge *bestBack1, *bestBack2;
    bestBack1 = bestBack2 = 0;
    vector<NBEdge*> outgoing(*_outgoingEdges);
    // for best1
    if(outgoing.size()>0) {
        sort(outgoing.begin(), outgoing.end(),
            NBContHelper::edge_similar_direction_sorter(best1));
        bestBack1 = extractAndMarkFirst(outgoing);
    }
    // for best2
    if(best2!=0&&outgoing.size()>0) {
        sort(outgoing.begin(), outgoing.end(),
            NBContHelper::edge_similar_direction_sorter(best1));
        bestBack1 = extractAndMarkFirst(outgoing);
    }


    /*
    if(noIncomingPrios==1) {
        // in this case, all incoming edges are equal in their priority
        // --> choose any as the main
        best1 = extractAndMarkFirst(incoming);
        // try to get the incoming edge in the opposite geometrical direction
        if(incoming.size()>0) {
            sort(incoming.begin(), incoming.end(),
                NBContHelper::edge_opposite_direction_sorter(best1));
            best2 = extractAndMarkFirst(incoming);
        }
    } else {
        // in this case, there is at least one higher priorised edge
        // --> choose this as main
        sort(incoming.begin(), incoming.end(),
            NBContHelper::edge_by_priority_sorter());
        best1 = extractAndMarkFirst(incoming);
        // now check whether to use another high priorised edge or choose one
        // of the lower priorised edges; it is also possible that there are
        // more than one higher priorised edges
        noIncomingPrios = NBContHelper::countPriorities(incoming);
        if(noIncomingPrios==1) {
            // all other incoming edges have the same priority; choose the one
            // in the opposite direction
            if(incoming.size()>0) {
                sort(incoming.begin(), incoming.end(),
                    NBContHelper::edge_opposite_direction_sorter(best1));
                best2 = extractAndMarkFirst(incoming);
            }
        } else {
            // choose one of the highest priorised edges that is in the most
            // opposite direction to the first chosen
            if(incoming.size()>0) {
                vector<NBEdge*> best = getMostPriorised(incoming);
                sort(best.begin(), best.end(),
                    NBContHelper::edge_opposite_direction_sorter(best1));
                best2 = extractAndMarkFirst(best);
            }
        }
    }
    // now process the outgoing edges
    vector<NBEdge*> outgoing(*_outgoingEdges);
    NBEdge *bestBack1, *bestBack2;
    bestBack1 = bestBack2 = 0;
    // for both edges, try to find the edges in the other direction
    if(outgoing.size()>1) {
        sort(outgoing.begin(), outgoing.end(),
            NBContHelper::edge_opposite_direction_sorter(best1));
        bestBack1 = extractAndMarkFirst(outgoing);
    }
    if(outgoing.size()>0&&best2!=0) {
        sort(outgoing.begin(), outgoing.end(),
            NBContHelper::edge_opposite_direction_sorter(best2));
        bestBack2 = extractAndMarkFirst(outgoing);
    }
    */
}


int
NBNode::getHighestPriority(const vector<NBEdge*> &s) {
    if(s.size()==0) {
        return 0;
    }
    vector<int> knownPrios;
    for(vector<NBEdge*>::const_iterator i=s.begin(); i!=s.end(); i++) {
        knownPrios.push_back((*i)->getPriority());
    }
    sort(knownPrios.begin(), knownPrios.end());
    return knownPrios[0];
}


NBEdge*
NBNode::extractAndMarkFirst(vector<NBEdge*> &s) {
    if(s.size()==0) {
        return 0;
    }
    NBEdge *ret = s.front();
    s.erase(s.begin());
    ret->setJunctionPriority(this, 1);
    return ret;
}


vector<NBEdge*>
NBNode::getMostPriorised(vector<NBEdge*> &s)
{
    if(s.size()==0) {
        return vector<NBEdge*>();
    }
    sort(s.begin(), s.end(), NBContHelper::edge_by_priority_sorter());
    vector<NBEdge*> ret;
    int highestP = (*(s.begin()))->getPriority();
    for(vector<NBEdge*>::iterator i=s.begin(); i!=s.end(); i++) {
        if(highestP==(*i)->getPriority())
            ret.push_back(*i);
    }
    return ret;
}


void
NBNode::rotateIncomingEdges(int norot)
{
    if(_incomingEdges->size()==0) {
        return;
    }
    while(norot>0) {
        NBEdge *e = (*_incomingEdges)[0];
        unsigned int i;
        for(i=0; i<_incomingEdges->size()-1; i++) {
            assert(_incomingEdges!=0&&_incomingEdges->size()>i+1);
            (*_incomingEdges)[i] = (*_incomingEdges)[i+1];
        }
        assert(_incomingEdges!=0&&i<_incomingEdges->size());
        (*_incomingEdges)[i] = e;
        norot--;
    }
}

std::vector<std::string>
NBNode::getInternalNamesList()
{
    std::vector<std::string> ret;
    if(countInternalLanes()!=0) {
        ret.push_back(string(":") + _id);
    }
    return ret;
}

size_t
NBNode::countInternalLanes()
{
    size_t lno = 0;
    EdgeVector::iterator i;
    for(i=_incomingEdges->begin(); i!=_incomingEdges->end(); i++) {
        size_t noLanesEdge = (*i)->getNoLanes();
        for(size_t j=0; j<noLanesEdge; j++) {
            const EdgeLaneVector *elv = (*i)->getEdgeLanesFromLane(j);
            for(EdgeLaneVector::const_iterator k=elv->begin(); k!=elv->end(); k++) {
                if((*k).edge==0) {
                    continue;
                }
                lno++;
            }
        }
    }
    return lno;
}


void
NBNode::writeXMLInternalLinks(ostream &into)
{
    if(countInternalLanes()==0) {
        return;
    }
    string id = string(":") + _id;
    into << "   <edge id=\"" << id
        << "\" function=\"internal\">" << endl;
    into << "      <lanes>" << endl;
    size_t lno = 0;
    EdgeVector::iterator i;
    for(i=_incomingEdges->begin(); i!=_incomingEdges->end(); i++) {
        size_t noLanesEdge = (*i)->getNoLanes();
        for(size_t j=0; j<noLanesEdge; j++) {
            const EdgeLaneVector *elv = (*i)->getEdgeLanesFromLane(j);
            for(EdgeLaneVector::const_iterator k=elv->begin(); k!=elv->end(); k++) {
                if((*k).edge==0) {
                    continue;
                }
/*              string id = string(":") + (*i)->getID() + string("_")
                    + toString<size_t>(j) + "x2x" + (*k).edge->getID();*/
                Position2D end = (*k).edge->getLaneShape((*k).lane).getBegin();
                Position2D beg = (*i)->getLaneShape(j).getEnd();
                double length = GeomHelper::distance(beg, end);
                into << "         <lane id=\"" << id << "_"
                    << toString<size_t>(lno++) << "\" depart=\"0\" "
                    << "maxspeed=\"11.111000\" length=\""
                    << toString<double>(length) << "\" "
                    << "changeurge=\"0\">"
                    << beg << " " << end
                    << "</lane>" << endl;
            }
        }
    }
    into << "      </lanes>" << endl;
    lno = 0;
    for(i=_incomingEdges->begin(); i!=_incomingEdges->end(); i++) {
        size_t noLanesEdge = (*i)->getNoLanes();
        for(size_t j=0; j<noLanesEdge; j++) {
            const EdgeLaneVector *elv = (*i)->getEdgeLanesFromLane(j);
            for(EdgeLaneVector::const_iterator k=elv->begin(); k!=elv->end(); k++) {
                if((*k).edge==0) {
                    continue;
                }
/*              string id = string(":") + (*i)->getID() + string("_")
                    + toString<size_t>(j) + "x2x" + (*k).edge->getID();*/
                into << "      <cedge id=\"" << (*k).edge->getID()
                    << "\">" << id << "_" << toString<size_t>(lno++)
                    << "</cedge>" << endl;
            }
        }
    }

    into << "   </edge>" << endl << endl;
}


void
NBNode::writeXMLInternalEdgePos(ostream &into)
{
    if(countInternalLanes()==0) {
        return;
    }
    string id = string(":") + _id;
    size_t lno = 0;
    for(EdgeVector::iterator i=_incomingEdges->begin(); i!=_incomingEdges->end(); i++) {
        size_t noLanesEdge = (*i)->getNoLanes();
        for(size_t j=0; j<noLanesEdge; j++) {
            const EdgeLaneVector *elv = (*i)->getEdgeLanesFromLane(j);
            for(EdgeLaneVector::const_iterator k=elv->begin(); k!=elv->end(); k++) {
                if((*k).edge==0) {
                    continue;
                }
/*              string id = string(":") + (*i)->getID() + string("_")
                    + toString<size_t>(j) + "x2x" + (*k).edge->getID();*/
                into << "   <edgepos id=\"" << id
                    << "\" from=\"" << _id << "\" to=\"" << _id << "\" "
                    << "lane=\"" << lno++ << "\" function=\"internal\">"
                    << _x << "," << _y << " " << _x << "," << _y
                    << "</edgepos>"
                    << endl;
            }
        }
    }
}



void
NBNode::writeXMLInternalSuccInfos(ostream &into)
{
    if(countInternalLanes()==0) {
        return;
    }
    string id = string(":") + _id;
    size_t lno = 0;
    for(EdgeVector::iterator i=_incomingEdges->begin(); i!=_incomingEdges->end(); i++) {
        size_t noLanesEdge = (*i)->getNoLanes();
        for(size_t j=0; j<noLanesEdge; j++) {
            const EdgeLaneVector *elv = (*i)->getEdgeLanesFromLane(j);
            for(EdgeLaneVector::const_iterator k=elv->begin(); k!=elv->end(); k++) {
                if((*k).edge==0) {
                    continue;
                }
/*              string id = string(":") + (*i)->getID() + string("_")
                    + toString<size_t>(j) + "x2x" + (*k).edge->getID();*/
                into << "   <succ edge=\"" << id << "\" "
                    << "lane=\"" << id << "_"
                    << toString<size_t>(lno) << "\" junction=\"" << _id << "\">"
                    << endl;
                into << "      <succlane lane=\""
                    << (*k).edge->getID() << "_" << (*k).lane
                    << "\" tl=\"" << (*k).tlID << "\" linkno=\""
                    << (*k).tlLinkNo << "\" yield=\"0\" dir=\"s\" state=\"M\""
                    << " int_end=\"x\"/>"
                    << endl;
                lno++;
                into << "   </succ>" << endl;
            }
        }
    }
}


void
bla(EdgeVector *_incomingEdges, ostream &into, const std::string &id)
{
    size_t l = 0;
    for(EdgeVector::iterator i=_incomingEdges->begin(); i!=_incomingEdges->end(); i++) {
        size_t noLanesEdge = (*i)->getNoLanes();
        for(size_t j=0; j<noLanesEdge; j++) {
            const EdgeLaneVector *elv = (*i)->getEdgeLanesFromLane(j);
            for(EdgeLaneVector::const_iterator k=elv->begin(); k!=elv->end(); k++) {
                if((*k).edge==0) {
                    continue;
                }
                if(l!=0) {
                    into << ' ';
                }
                string lid = string(":") + id + string("_")
                    + toString<size_t>(l);
                l++;
                into << lid;
            }
        }
    }
}

void
NBNode::writeXML(ostream &into)
{
    // write the attributes
    into << "   <junction id=\"" << _id << '\"';
    if(_incomingEdges->size()!=0&&_outgoingEdges->size()!=0) {
        //into << " key=\"" << _key << '\"';
        switch(_type) {
        case NODETYPE_NOJUNCTION:
            into << " type=\"" << "none\"";
            break;
        case NODETYPE_PRIORITY_JUNCTION:
            into << " type=\"" << "priority\"";
            break;
        case NODETYPE_RIGHT_BEFORE_LEFT:
            into << " type=\"" << "right_before_left\"";
            break;
        case NODETYPE_DISTRICT:
            into << " type=\"" << "district\"";
            break;
        default:
            throw exception();
            break;
        }
    } else {
        into << " type=\"DEAD_END\"";
    }
    into << " x=\"" << setprecision( 2 ) << _x
        << "\" y=\"" << setprecision( 2 ) << _y << "\"";
    into <<  ">" << endl;
    // write the inlanes
    EdgeVector::iterator i;
    into << "      <inlanes>";
    for(i=_incomingEdges->begin(); i!=_incomingEdges->end(); i++) {
        size_t noLanes = (*i)->getNoLanes();
        string id = (*i)->getID();
        for(size_t j=0; j<noLanes; j++) {
            into << id << '_' << j;
            if(i!=_incomingEdges->end()-1 || j<noLanes-1) {
                into << ' ';
            }
        }
    }
    if(OptionsSubSys::getOptions().getBool("add-internal-links")) {
        if(countInternalLanes()!=0) {
            into << ' ';
            bla(_incomingEdges, into, _id);
        }
    }
    into << "</inlanes>" << endl;
    // write the shape
    into << "      <shape>" << myPoly << "</shape>" << endl;
    // close writing
    into << "   </junction>" << endl << endl;
}


void
NBNode::setKey(string key)
{
    _key = key;
}


void
NBNode::computeLogic(OptionsCont &oc)
{
    if(_incomingEdges->size()==0||_outgoingEdges->size()==0) {
        return;
    }
    // build the request
    _request = new NBRequest(this,
        static_cast<const EdgeVector * const>(&_allEdges),
        static_cast<const EdgeVector * const>(_incomingEdges),
        static_cast<const EdgeVector * const>(_outgoingEdges),
        _blockedConnections);

    // compute the logic if necessary or split the junction
    if(_type!=NODETYPE_NOJUNCTION&&_type!=NODETYPE_DISTRICT) {
		_request->buildBitfieldLogic(_id);
    }
}


void
NBNode::setType(BasicNodeType type)
{
    switch(type) {
    case NODETYPE_NOJUNCTION:
        _noNoJunctions++;
        break;
        break;
    case NODETYPE_PRIORITY_JUNCTION:
        _noPriorityJunctions++;
        break;
    case NODETYPE_RIGHT_BEFORE_LEFT:
        _noRightBeforeLeftJunctions++;
        break;
    case NODETYPE_DISTRICT:
        _noRightBeforeLeftJunctions++;
        break;
    default:
        throw exception();
    }
    _type = type;
}


void
NBNode::reportBuild()
{
    MsgHandler::getMessageInstance()->inform(
        string("No Junctions (converted)    : ")
        + toString<int>(_noNoJunctions));
    MsgHandler::getMessageInstance()->inform(
        string("Priority Junctions          : ")
        + toString<int>(_noPriorityJunctions));
    MsgHandler::getMessageInstance()->inform(
        string("Right Before Left Junctions : ")
        + toString<int>(_noRightBeforeLeftJunctions));
}


void
NBNode::sortNodesEdges()
{
    // sort the edges
    buildList();
    sort(_allEdges.begin(), _allEdges.end(),
        NBContHelper::edge_by_junction_angle_sorter(this));
    sort(_incomingEdges->begin(), _incomingEdges->end(),
        NBContHelper::edge_by_junction_angle_sorter(this));
    sort(_outgoingEdges->begin(), _outgoingEdges->end(),
        NBContHelper::edge_by_junction_angle_sorter(this));
    sortSmall();
    setType(computeType());
    setPriorities();
#ifdef _DEBUG
#ifdef CROSS_TEST
    if(_id=="0") {
        EdgeVector::iterator i=_allEdges.begin();
        assert((*i++)->getID()=="4si");
        assert((*i++)->getID()=="4o");
        assert((*i++)->getID()=="2si");
        assert((*i++)->getID()=="2o");
        assert((*i++)->getID()=="3si");
        assert((*i++)->getID()=="3o");
        assert((*i++)->getID()=="1si");
        assert((*i++)->getID()=="1o");
    }
#endif
#endif
}


void
NBNode::computeNodeShape()
{
    if(_incomingEdges->size()==0&&_outgoingEdges->size()==0) {
        return;
    }
    // compute the shape of the junction
    //  only junction have a shape, otherwise it's somekind
    //  of another connection between streets
/*    if( (_incomingEdges->size()<1||_outgoingEdges->size()<1)
        ||
        (_incomingEdges->size()<2&&_outgoingEdges->size()<2) ) {
        return;
    }*/
    // check whether this is a real junction between different edges
    //  or only an edge's split/join
    bool isRealJunction = false;
    if(_id=="381") {
        int bla = 0;
    }
    if(_id=="684"||_id=="685") {
        int bla = 0;
    }
    for(EdgeVector::const_iterator i=_allEdges.begin(); i!=_allEdges.end()&&!isRealJunction; i++) {
        Line2D incLine;
        if(hasIncoming(*i)) {
            const Position2DVector &geom = (*i)->getGeometry();
            incLine = Line2D(geom.at(geom.size()-2), geom.at(geom.size()-1));
        } else {
            const Position2DVector &geom = (*i)->getGeometry();
            incLine = Line2D(geom.at(1), geom.at(0));
        }
        NBEdge *e1 = (*i);
        double angle1 = incLine.atan2DegreeAngle();
        for(EdgeVector::const_iterator i2=i+1; i2!=_allEdges.end()&&!isRealJunction; i2++) {
            Line2D outLine;
            NBEdge *e2 = (*i2);
            if(hasIncoming(*i2)) {
                const Position2DVector &geom = (*i2)->getGeometry();
                outLine = Line2D(geom.at(geom.size()-2), geom.at(geom.size()-1));
            } else {
                const Position2DVector &geom = (*i2)->getGeometry();
                outLine = Line2D(geom.at(1), geom.at(0));
            }
            double angle2 = outLine.atan2DegreeAngle();
            //
            if(abs(angle1-angle2)>35&&abs(angle1-angle2)<145) {
                isRealJunction = true;
            }
        }
    }
    if(_id=="684"||_id=="685") {
        int bla = 0;
    }
    if(_id=="381") {
        int bla = 0;
    }
//    if(isRealJunction) {
        computeRealNodeShape();
/*    } else {
        computeJoinSplitNodeShape();
    }*/
    myPoly = myPoly.convexHull();
    myPoly.closePolygon();
}


void
NBNode::computeRealNodeShape()
{
    if(_id=="545") {
        int bla = 0;
    }
    std::vector<double> edgeOffsets;
    EdgeVector::const_iterator i;
    for(i=_allEdges.begin(); i!=_allEdges.end(); i++) {
        NBEdge *current = *i;
        // the clockwise and the counter clockwise border;
        Position2DVector cb, ccb;
        // the left and the right crossing line
        // the crossing edges
        EdgeVector::const_iterator ri = i;
        EdgeVector::const_iterator li = i;
        double ccw = current->width();
        double cw = current->width();
        // clockwise border
        cb = current->getCWBounderyLine(this, 2.5);
        // counterclockwise border
        ccb = current->getCCWBounderyLine(this, 2.5);
        Position2DVector cl, ccl;
        if(hasIncoming(current)) {
            // is an incoming edge
            //  the counter clockwise edge is surely not the opposite direction
            NBContHelper::nextCCW(&_allEdges, ri);
            //  check the clockwise edge
            NBContHelper::nextCW(&_allEdges, li);
            if(current->isTurningDirection(*li)/*&&_incomingEdges->size()>2*/) {
                EdgeVector::const_iterator tmpi = li;
                NBContHelper::nextCW(&_allEdges, tmpi);
                double angle =
                    getCWAngleDiff(
                        cb.lineAt(0).atan2DegreeAngle(),
                        (*tmpi)->getCCWBounderyLine(this, 2.5).lineAt(0).atan2DegreeAngle());
                if(angle<90) {
                    cb = (*li)->getCWBounderyLine(this, 2.5);
                    cw = (*li)->width();
                }
                NBContHelper::nextCW(&_allEdges, li);
            }
            ccl = (*ri)->getCWBounderyLine(this, 2.5);
            cl = (*li)->getCCWBounderyLine(this, 2.5);
        } else {
            NBContHelper::nextCCW(&_allEdges, ri);
            if((*ri)->isTurningDirection(current)) {
                EdgeVector::const_iterator tmpi = ri;
                NBContHelper::nextCCW(&_allEdges, tmpi);
                double angle =
                    getCCWAngleDiff(
                        ccb.lineAt(0).atan2DegreeAngle(),
                        (*tmpi)->getCWBounderyLine(this, 2.5).lineAt(0).atan2DegreeAngle());
                if(angle<90) {
                    ccb = (*ri)->getCCWBounderyLine(this, 2.5);
                    ccw = (*ri)->width();
                }
                NBContHelper::nextCCW(&_allEdges, ri);
            }
            NBContHelper::nextCW(&_allEdges, li);
            ccl = (*ri)->getCWBounderyLine(this, 2.5);
            cl = (*li)->getCCWBounderyLine(this, 2.5);
        }
        double offr, offl, cca, ca;
        if(ri==i) {
            offr = 4;
            cca = 360;
        } else {
            offr = getOffset(ccb, ccl);
            double a1 = ccb.lineAt(0).atan2DegreeAngle();
            double a2 = ccl.lineAt(0).atan2DegreeAngle();
            cca = getCCWAngleDiff(a1, a2);
            if(cca>180||cca<10) {
                offr = -1;
            }
/*            double lw1 = fabs(sin(cca*3.1415926535897932384626433832795/180.0)*((*ri)->width()*1.5));
            double lw2 = fabs(sin(cca*3.1415926535897932384626433832795/180.0)*(ccw*1.5));
            if(cca>90&&lw1<fabs(100-offr)&&lw2<fabs(100-offr)) {
                offr = -1;
            }*/

        }
        if(li==i) {
            offl = 4;
            ca = 360;
        } else {
            offl = getOffset(cb, cl);
            double a1 = cb.lineAt(0).atan2DegreeAngle();
            double a2 = cl.lineAt(0).atan2DegreeAngle();
            ca = getCWAngleDiff(a1, a2);
            if(ca>180||ca<10) {
                offl = -1;
            }
/*            double lw1 = fabs(sin(ca*3.1415926535897932384626433832795/180.0)*((*li)->width()*1.5));
            double lw2 = fabs(sin(ca*3.1415926535897932384626433832795/180.0)*(cw*1.5));
            if(ca>90&&lw1<fabs(100-offl)&&lw2<fabs(100-offl)) {
                offl = -1;
            }*/

        }
        if(offr==-1&&offl==-1) {
            edgeOffsets.push_back(4);
        } else if(offr==-1||offl==-1) {
            edgeOffsets.push_back(MAX(offr, offl));
        } /*else if(cca!=360-ca) {
            if(cca>ca) {
                edgeOffsets.push_back(offl);
            } else {
                edgeOffsets.push_back(offr);
            }
        } */else {
            edgeOffsets.push_back(MAX(offr, offl));
        }
    }
    std::vector<double>::iterator j;
    for(i=_allEdges.begin(), j=edgeOffsets.begin(); i!=_allEdges.end(); i++, j++) {
        double offset = *j;
        // do not process edges with no crossing
        if(offset<0) {
            continue;
        }
        // do not process outgoing which have opposite incoming for themselves
        EdgeVector::const_iterator li = i;
        NBContHelper::nextCW(&_allEdges, li);
        NBEdge *current = *i;
        if(hasIncoming(current)&&current->isTurningDirection(*li)) {
            std::vector<double>::iterator j2 = j+1;
            if(j2==edgeOffsets.end()) {
                j2 = edgeOffsets.begin();
            }
            offset = MAX(offset,(*j2));
            // ok, process both directions
            addCCWPoint(current, offset);
            addCWPoint((*li), offset);
            // and skip the next one
            if(i+1!=_allEdges.end()) {
                i++;
                j++;
            }
        } else if(hasOutgoing(current)) {
            EdgeVector::const_iterator ri = i;
            NBContHelper::nextCCW(&_allEdges, ri);
            // skip outgoing that have an incoming
            if((*ri)->isTurningDirection(current)) {
                continue;
            }
            addCCWPoint(current, offset);
            addCWPoint(current, offset);
        } else {
            // process this edge only
            addCCWPoint(current, offset);
            addCWPoint(current, offset);
        }
    }
}


void
NBNode::addCCWPoint(NBEdge *e, double offset)
{
    Position2DVector l = e->getCCWBounderyLine(this, 1.5);
    double len = l.length();
    if(len>=offset) {
        myPoly.push_back(l.positionAtLengthPosition(offset));
    } else {
        myPoly.push_back(l.positionAtLengthPosition(len));
    }
}

void
NBNode::addCWPoint(NBEdge *e, double offset)
{
    Position2DVector l = e->getCWBounderyLine(this, 1.5);
    double len = l.length();
    if(len>=offset) {
        myPoly.push_back(l.positionAtLengthPosition(offset));
    } else {
        myPoly.push_back(l.positionAtLengthPosition(len));
    }
}

void
NBNode::computeJoinSplitNodeShape()
{
    if(_id=="684") {
        int bla = 0;
    }
    std::vector<double> edgeOffsets;
    EdgeVector::const_iterator i;
    for(i=_allEdges.begin(); i!=_allEdges.end(); i++) {
        Position2DVector o1 = (*i)->getCCWBounderyLine(this, 1.5);
        Position2DVector o2 = (*i)->getCWBounderyLine(this, 1.5);
        Position2DVector geom = (*i)->getGeometry();
        geom.extrapolate(1000);
        // build normal
        Line2D incLine;
        if(hasIncoming(*i)) {
            incLine = Line2D(geom.at(geom.size()-2), geom.at(geom.size()-1));
        } else {
            incLine = Line2D(geom.at(1), geom.at(0));
        }
        double xcenter = (incLine.p1().x() + incLine.p2().x()) / 2.0;
        double ycenter = (incLine.p1().y() + incLine.p2().y()) / 2.0;
        incLine.sub(xcenter, ycenter);
        Line2D extrapolated(
            GeomHelper::extrapolate_first(incLine.p1(), incLine.p2(), 1000),
            GeomHelper::extrapolate_second(incLine.p1(), incLine.p2(), 1000));
        Line2D normal(
            Position2D(extrapolated.p1().y(), extrapolated.p1().x()),
            Position2D(extrapolated.p2().y(), extrapolated.p2().x()));
        normal.add(_x, _y);
        // get cross position
        double pos;
        if(hasIncoming(*i)) {
            pos = 0;//!!!(*i)->getGeometry().length();
        } else {
            pos = 0;
        }
        if(!geom.intersects(normal.p1(), normal.p2())) {
            int bla = 0;
//            continue;//!!! muss immer gelten, tut's nicht
//            DoubleVector posses = geom.intersectsAtLengths(normal);
            myPoly.push_back(geom.positionAtLengthPosition(pos));
/*            myPoly.push_back(
                (*i)->getCCWBounderyLine(this, 1.5).getPositionAtDistance(pos+100));
            myPoly.push_back(
                (*i)->getCWBounderyLine(this, 1.5).getPositionAtDistance(pos+100));*/
        }

        if(o1.intersects(normal.p1(), normal.p2())) {
            myPoly.push_back(o1.intersectsAtPoint(normal.p1(), normal.p2()));
        } else {
            int bla = 0;
        }
        if(o2.intersects(normal.p1(), normal.p2())) {
            myPoly.push_back(o2.intersectsAtPoint(normal.p1(), normal.p2()));
        } else {
            int bla = 0;
        }
//        if(geom.intersects(normal.p1(), normal.p2())) {
        if(!o1.intersects(normal.p1(), normal.p2())
            &&
            !o2.intersects(normal.p1(), normal.p2())
            &&
            geom.intersects(normal.p1(), normal.p2())
            ) {
            assert(geom.intersects(normal.p1(), normal.p2()));
            DoubleVector posses = geom.intersectsAtLengths(normal);
            if(posses.size()==0) {
                int bla = 0;
                posses = geom.intersectsAtLengths(normal);
            }
            if(hasIncoming(*i)) {
                pos = DoubleVectorHelper::minValue(posses);
            } else {
                pos = DoubleVectorHelper::maxValue(posses);
            }
            // add geometry point
            myPoly.push_back(
                (*i)->getCCWBounderyLine(this, 1.5).positionAtLengthPosition(pos-1000));
            myPoly.push_back(
                (*i)->getCWBounderyLine(this, 1.5).positionAtLengthPosition(pos-1000));
        }
//        myPoly.push_back(geom.positionAtLengthPosition(pos));
    }
}


double
NBNode::getCCWAngleDiff(double angle1, double angle2)
{
    if(angle1<0) {
        angle1 = 360 + angle1;
    }
    if(angle2<0) {
        angle2 = 360 + angle2;
    }
    if(angle1>angle2) {
        return angle1 - angle2;
    }
    return angle1 + 360 - angle2;
}

double
NBNode::getCWAngleDiff(double angle1, double angle2)
{
    if(angle1<0) {
        angle1 = 360 + angle1;
    }
    if(angle2<0) {
        angle2 = 360 + angle2;
    }
    if(angle1<angle2) {
        return angle2 - angle1;
    }
    return 360 - angle1 + angle2;
}


double
NBNode::chooseLaneOffset(DoubleVector &chk)
{
    return DoubleVectorHelper::minValue(chk);
    /*
    double max = DoubleVectorHelper::max(chk);
    if(max<100) {
        return 100-max+100;
    } else {
        double min = max;
        for(DoubleVector::iterator i=chk.begin(); i!=chk.end(); i++) {
            double bla =
                (*i)>100 ? (*i) : 100-(*i)+100;
            if(bla<min) {
                min = bla;
            }
        }
        return min;*/
/*        DoubleVectorHelper::remove_smaller_than(chk, 100);
        return DoubleVectorHelper::min(chk);*/
//    }
}


double
NBNode::chooseLaneOffset2(DoubleVector &chk)
{
    DoubleVectorHelper::remove_larger_than(chk, 100);
    double max = DoubleVectorHelper::maxValue(chk);
    return 100-max+100;
}

double
NBNode::getOffset(Position2DVector on, Position2DVector cross)
{
    if(!on.intersects(cross)) {
        return -1;
    }
//    on.extrapolateBy(100);
    DoubleVector posses = on.intersectsAtLengths(cross);
    assert(posses.size()>0);
    return DoubleVectorHelper::maxValue(posses);
//    return on.positionAtLengthPosition(pos);
}


void
NBNode::computeLanes2Lanes()
{
    // go through this node's outgoing edges
    //  for every outgoing edge, compute the distribution of the node's
    //  incoming edges on this edge when approaching this edge
    for( vector<NBEdge*>::reverse_iterator i=_outgoingEdges->rbegin();
         i!=_outgoingEdges->rend(); i++) {
        NBEdge *currentOutgoing = *i;
        // get the information about edges that do approach this edge
        vector<NBEdge*> *approaching = getApproaching(currentOutgoing);
        if(approaching->size()!=0) {
            ApproachingDivider divider(approaching, currentOutgoing);
            Bresenham::compute(&divider, approaching->size(),
                currentOutgoing->getNoLanes());
        }
        delete approaching;
    }
}


vector<NBEdge*> *
NBNode::getApproaching(NBEdge *currentOutgoing)
{
    // get the position of the node to get the approaching nodes of
    vector<NBEdge*>::const_iterator i = find(_allEdges.begin(),
        _allEdges.end(), currentOutgoing);
    // get the first possible approaching edge
    NBContHelper::nextCW(&_allEdges, i);
    // go through the list of edges clockwise and add the edges
    vector<NBEdge*> *approaching = new vector<NBEdge*>();
    for(; *i!=currentOutgoing; ) {
        // check only incoming edges
        if((*i)->getToNode()==this) {
            vector<size_t> connLanes =
                (*i)->getConnectionLanes(currentOutgoing);
            if(connLanes.size()!=0) {
                approaching->push_back(*i);
            }
        }
        NBContHelper::nextCW(&_allEdges, i);
    }
    return approaching;
}


std::string
NBNode::setTurningDefinition(NBNode *from, NBNode *to)
{
    EdgeVector::iterator i;
    // get the source edge
    NBEdge *src = 0;
    for(i=_incomingEdges->begin(); src==0 && i!=_incomingEdges->end(); i++) {
        if((*i)->getFromNode()==from) {
            src = (*i);
        }
    }
    if(src==0) {
        return string("There is no edge from node '") + from->getID()
            + string("' to node '") + getID() + string("'.");
    }
    // get the destination edge
    NBEdge *dest = 0;
    for(i=_outgoingEdges->begin(); dest==0 && i!=_outgoingEdges->end(); i++) {
        if((*i)->getToNode()==to) {
            dest = (*i);
        }
    }
    if(dest==0) {
        return string("There is no edge from node '") + getID()
            + string("' to node '") + to->getID() + string("'.");
    }
    // both edges found
    //  set them into the edge
    src->addEdge2EdgeConnection(dest);
    // no error occured
    return "";
}


void
NBNode::resetby(double xoffset, double yoffset)
{
    _x += xoffset;
    _y += yoffset;
    myPoly.resetBy(xoffset, yoffset);
}


void
NBNode::reshiftPosition(double xoff, double yoff, double rot)
{
    _x = _x * cos(rot) + _y * sin(rot) + xoff;
    _y = _y * cos(rot) - _x * sin(rot) + yoff;
    myPoly.reshiftRotate(xoff, yoff, rot);
}


void
NBNode::replaceOutgoing(NBEdge *which, NBEdge *by, size_t laneOff)
{
    size_t i;
    // replace the edge in the list of outgoing nodes
    for(i=0; i<_outgoingEdges->size(); i++) {
        if((*_outgoingEdges)[i]==which) {
            (*_outgoingEdges)[i] = by;
        }
    }
    // replace the edge in connections of incoming edges
    for(i=0; i<_incomingEdges->size(); i++) {
        (*_incomingEdges)[i]->replaceInConnections(which, by, laneOff);
    }
    // replace within the connetion prohibition dependencies
    replaceInConnectionProhibitions(which, by, 0, laneOff);
}


void
NBNode::replaceOutgoing(const EdgeVector &which, NBEdge *by)
{
    // replace edges
    size_t laneOff = 0;
    for(EdgeVector::const_iterator i=which.begin(); i!=which.end(); i++) {
        replaceOutgoing(*i, by, laneOff);
        laneOff += (*i)->getNoLanes();
    }
    // removed double occurences
    removeDoubleEdges();
    // check whether this node belongs to a district and the edges
    //  must here be also remapped
    if(myDistrict!=0) {
        myDistrict->replaceOutgoing(which, by);
    }
}


void
NBNode::replaceIncoming(NBEdge *which, NBEdge *by, size_t laneOff)
{
    // replace the edge in the list of incoming nodes
    for(size_t i=0; i<_incomingEdges->size(); i++) {
        if((*_incomingEdges)[i]==which) {
            (*_incomingEdges)[i] = by;
        }
    }
    // add connections of the old edge to the new one
    EdgeVector connected = which->getConnected();
    for(EdgeVector::const_iterator j=connected.begin(); j!=connected.end(); j++) {
        by->addEdge2EdgeConnection(*j);
    }
    // replace within the connetion prohibition dependencies
    replaceInConnectionProhibitions(which, by, laneOff, 0);
}


void
NBNode::replaceIncoming(const EdgeVector &which, NBEdge *by)
{
    // replace edges
    size_t laneOff = 0;
    for(EdgeVector::const_iterator i=which.begin(); i!=which.end(); i++) {
        replaceIncoming(*i, by, laneOff);
        laneOff += (*i)->getNoLanes();
    }
    // removed double occurences
    removeDoubleEdges();
    // check whether this node belongs to a district and the edges
    //  must here be also remapped
    if(myDistrict!=0) {
        myDistrict->replaceIncoming(which, by);
    }
}



void
NBNode::replaceInConnectionProhibitions(NBEdge *which, NBEdge *by,
                                        size_t whichLaneOff, size_t byLaneOff)
{
    // replace in keys
    NBConnectionProhibits::iterator j = _blockedConnections.begin();
    while(j!=_blockedConnections.end()) {
        bool changed = false;
        NBConnection c = (*j).first;
        if(c.replaceFrom(which, whichLaneOff, by, byLaneOff)) {
            changed = true;
        }
        if(c.replaceTo(which, whichLaneOff, by, byLaneOff)) {
            changed = true;
        }
        if(changed) {
            _blockedConnections[c] = (*j).second;
            _blockedConnections.erase(j);
            j = _blockedConnections.begin();
        } else {
            j++;
        }
    }
    // replace in values
    for(j=_blockedConnections.begin(); j!=_blockedConnections.end(); j++) {
        const NBConnection &prohibited = (*j).first;
        NBConnectionVector &prohibiting = (*j).second;
        for(NBConnectionVector::iterator k=prohibiting.begin(); k!=prohibiting.end(); k++) {
            NBConnection &sprohibiting = *k;
            sprohibiting.replaceFrom(which, whichLaneOff, by, byLaneOff);
            sprohibiting.replaceTo(which, whichLaneOff, by, byLaneOff);
        }
    }
}



void
NBNode::removeDoubleEdges()
{
    EdgeVector::iterator i;
    // check incoming
    size_t pos = 0;
    for(i=_incomingEdges->begin(); _incomingEdges->size()!=0&&i!=_incomingEdges->end()-1;) {
        EdgeVector::iterator j = find(i+1, _incomingEdges->end(), *i);
        if(j!=_incomingEdges->end()) {
            _incomingEdges->erase(j);
            i = _incomingEdges->begin() + pos;
        } else {
            pos++;
            i++;
        }
    }
    // check outgoing
    pos = 0;
    for(i=_outgoingEdges->begin(); _outgoingEdges->size()!=0&&i!=_outgoingEdges->end()-1; ) {
        EdgeVector::iterator j = find(i+1, _outgoingEdges->end(), *i);
        if(j!=_outgoingEdges->end()) {
            _outgoingEdges->erase(j);
            i = _outgoingEdges->begin() + pos;
        } else {
            pos++;
            i++;
        }
    }
}


bool
NBNode::hasOutgoing(NBEdge *e) const
{
    return find(_outgoingEdges->begin(), _outgoingEdges->end(), e)
        !=
        _outgoingEdges->end();
}


bool
NBNode::hasIncoming(NBEdge *e) const
{
    return find(_incomingEdges->begin(), _incomingEdges->end(), e)
        !=
        _incomingEdges->end();
}


NBEdge *
NBNode::getOppositeIncoming(NBEdge *e) const
{
    EdgeVector edges(*_incomingEdges);
    sort(edges.begin(), edges.end(),
        NBContHelper::edge_similar_direction_sorter(e));
    return edges[0];
}


NBEdge *
NBNode::getOppositeOutgoing(NBEdge *e) const
{
    EdgeVector edges(*_outgoingEdges);
    sort(edges.begin(), edges.end(),
        NBContHelper::edge_similar_direction_sorter(e));
    return edges[0];
}

void
NBNode::addSortedLinkFoes(const NBConnection &mayDrive,
                          const NBConnection &mustStop)
{
    if( mayDrive.getFrom()==0 ||
        mayDrive.getTo()==0 ||
        mustStop.getFrom()==0 ||
        mustStop.getTo()==0) {

        MsgHandler::getWarningInstance()->inform(
            "Something went wrong during the building of a connection...");
        return; // !!! mark to recompute connections
    }
    NBConnectionVector conn = _blockedConnections[mustStop];
    conn.push_back(mayDrive);
    _blockedConnections[mustStop] = conn;
}


NBEdge *
NBNode::getPossiblySplittedIncoming(const std::string &edgeid)
{
    size_t size = edgeid.length();
    for(EdgeVector::iterator i=_incomingEdges->begin(); i!=_incomingEdges->end(); i++) {
        string id = (*i)->getID();
        if(id.substr(0, size)==edgeid) {
            return *i;
        }
    }
    return 0;
}


NBEdge *
NBNode::getPossiblySplittedOutgoing(const std::string &edgeid)
{
    size_t size = edgeid.length();
    for(EdgeVector::iterator i=_outgoingEdges->begin(); i!=_outgoingEdges->end(); i++) {
        string id = (*i)->getID();
        if(id.substr(0, size)==edgeid) {
            return *i;
        }
    }
    return 0;
}


size_t
NBNode::eraseDummies()
{
	size_t ret = 0;
    if(_outgoingEdges==0||_incomingEdges==0) {
        return ret;
    }
    size_t pos = 0;
    EdgeVector::const_iterator j=_incomingEdges->begin();
    while(j!=_incomingEdges->end()) {
        // skip edges which are only incoming and not outgoing
        if(find(_outgoingEdges->begin(), _outgoingEdges->end(), *j)==_outgoingEdges->end()) {
            j++;
            pos++;
            continue;
        }
        // an edge with both its origin and destination being the current
        //  node should be removed
        NBEdge *dummy = *j;
        MsgHandler::getWarningInstance()->inform(
			string(" Removing dummy edge '") + dummy->getID() + string("'"));
        // get the list of incoming edges connected to the dummy
        EdgeVector incomingConnected;
        EdgeVector::const_iterator i;
        for(i=_incomingEdges->begin(); i!=_incomingEdges->end(); i++) {
            if((*i)->isConnectedTo(dummy)&&*i!=dummy) {
                incomingConnected.push_back(*i);
            }
        }
        // get the list of outgoing edges connected to the dummy
        EdgeVector outgoingConnected;
        for(i=_outgoingEdges->begin(); i!=_outgoingEdges->end(); i++) {
            if(dummy->isConnectedTo(*i)&&*i!=dummy) {
                outgoingConnected.push_back(*i);
            }
        }
        // let the dummy remap its connections
        dummy->remapConnections(incomingConnected);
        remapRemoved(dummy, incomingConnected, outgoingConnected);
//        dummy->removeFromProceeding();
        // delete the dummy
        NBEdgeCont::erase(dummy);
        j = _incomingEdges->begin() + pos;
		ret++;
    }
	return ret;
}


void
NBNode::removeOutgoing(NBEdge *edge)
{
    EdgeVector::iterator i =
        find(_outgoingEdges->begin(), _outgoingEdges->end(), edge);
    if(i!=_outgoingEdges->end()) {
        _outgoingEdges->erase(i);
    }
}


void
NBNode::removeIncoming(NBEdge *edge)
{
    EdgeVector::iterator i =
        find(_incomingEdges->begin(), _incomingEdges->end(), edge);
    if(i!=_incomingEdges->end()) {
        _incomingEdges->erase(i);
    }
}




Position2D
NBNode::getEmptyDir() const
{
    Position2D pos(0, 0);
    EdgeVector::const_iterator i;
    for(i=_incomingEdges->begin(); i!=_incomingEdges->end(); i++) {
        NBNode *conn = (*i)->getFromNode();
        Position2D toAdd =
            Position2D(conn->getXCoordinate()-_x,
                conn->getYCoordinate()-_y);
        toAdd.mul(1.0/sqrt(toAdd.x()*toAdd.x()+toAdd.y()*toAdd.y()));
        pos.add(toAdd);
    }
    for(i=_outgoingEdges->begin(); i!=_outgoingEdges->end(); i++) {
        NBNode *conn = (*i)->getToNode();
        Position2D toAdd =
            Position2D(conn->getXCoordinate()-_x,
                conn->getYCoordinate()-_y);
        toAdd.mul(1.0/sqrt(toAdd.x()*toAdd.x()+toAdd.y()*toAdd.y()));
        pos.add(toAdd);
    }
    pos.mul(-1.0/(_incomingEdges->size()+_outgoingEdges->size()));
    return pos;
}



void
NBNode::invalidateIncomingConnections()
{
    for(EdgeVector::const_iterator i=_incomingEdges->begin(); i!=_incomingEdges->end(); i++) {
        (*i)->invalidateConnections();
    }
}


void
NBNode::invalidateOutgoingConnections()
{
    for(EdgeVector::const_iterator i=_outgoingEdges->begin(); i!=_outgoingEdges->end(); i++) {
        (*i)->invalidateConnections();
    }
}


bool
NBNode::mustBrake(NBEdge *from, NBEdge *to, int toLane) const
{
	assert(_request!=0);
    // check whether it is participant to a traffic light
    //  - controlled links are set by the traffic lights, not the normal
    //    right-of-way rules
    //  - uncontrolled participants (spip lanes etc.) should always break
    if(myTrafficLights.size()!=0) {
        return true; // What happens when the lights go out?
            // All would have to break... No problem!
    }
    bool try1 = _request->mustBrake(from, to);
    if(!try1||toLane==-1) {
        return try1;
    }
    for(EdgeVector::const_iterator i=_incomingEdges->begin(); i!=_incomingEdges->end(); i++) {
        if((*i)==from) {
            continue;
        }
        size_t noLanesEdge1 = (*i)->getNoLanes();
        for(size_t j1=0; j1<noLanesEdge1; j1++) {
            const EdgeLaneVector *el1 = (*i)->getEdgeLanesFromLane(j1);
            for(EdgeLaneVector::const_iterator i2=el1->begin(); i2!=el1->end(); i2++) {
                if((*i2).edge==to&&((*i2).lane==-1||(*i2).lane==toLane)) {
                    return true;
                }
            }
        }
    }
    return false;
}



bool
NBNode::isLeftMover(NBEdge *from, NBEdge *to) const
{
    // when the junction has only one incoming edge, there are no
    //  problems caused by left blockings
    if(_incomingEdges->size()==1) {
        return false;
    }
    // now again some heuristics...
    //  how to compute whether an edge is going to the left in the meaning,
    //  that it crosses the opposite straight direction?!
    vector<NBEdge*> incoming(*_incomingEdges);
    sort(incoming.begin(), incoming.end(),
        NBContHelper::edge_opposite_direction_sorter(from));
    NBEdge *opposite = *(incoming.begin());
    assert(opposite!=from);
    EdgeVector::const_iterator i =
        find(_allEdges.begin(), _allEdges.end(), from);
    NBContHelper::nextCW(&_allEdges, i);
    while(true) {
        if((*i)==opposite) {
            return false;
        }
        if((*i)==to) {
            return true;
        }
        NBContHelper::nextCW(&_allEdges, i);
    }
    return false;
}


bool
NBNode::forbids(NBEdge *possProhibitorFrom, NBEdge *possProhibitorTo,
				NBEdge *possProhibitedFrom, NBEdge *possProhibitedTo,
                bool regardNonSignalisedLowerPriority) const
{
    return _request->forbids(possProhibitorFrom, possProhibitorTo,
		possProhibitedFrom, possProhibitedTo,
        regardNonSignalisedLowerPriority);
}


bool
NBNode::foes(NBEdge *from1, NBEdge *to1, NBEdge *from2, NBEdge *to2) const
{
    return _request->foes(from1, to1, from2, to2);
}


void
NBNode::remapRemoved(NBEdge *removed, const EdgeVector &incoming,
                     const EdgeVector &outgoing)
{
    assert(find(incoming.begin(), incoming.end(), removed)==incoming.end());
    bool changed = true;
    while(changed) {
        changed = false;
        NBConnectionProhibits blockedConnectionsTmp = _blockedConnections;
        NBConnectionProhibits blockedConnectionsNew;
        // remap in connections
        for(NBConnectionProhibits::iterator i=blockedConnectionsTmp.begin(); i!=blockedConnectionsTmp.end(); i++) {
            const NBConnection &blocker = (*i).first;
            const NBConnectionVector &blocked = (*i).second;
            // check the blocked connections first
                // check whether any of the blocked must be changed
            bool blockedChanged = false;
            NBConnectionVector newBlocked;
            NBConnectionVector::const_iterator i;
            for(i=blocked.begin(); i!=blocked.end(); i++) {
                const NBConnection &sblocked = *i;
                if(sblocked.getFrom()==removed||sblocked.getTo()==removed) {
                    blockedChanged = true;
                }
            }
                // adapt changes if so
            for(i=blocked.begin(); blockedChanged&&i!=blocked.end(); i++) {
                const NBConnection &sblocked = *i;
                if(sblocked.getFrom()==removed&&sblocked.getTo()==removed) {
                    for(EdgeVector::const_iterator i=incoming.begin(); i!=incoming.end(); i++) {
                        newBlocked.push_back(NBConnection(*i, *i));
                    }
                } else if(sblocked.getFrom()==removed) {
                    assert(sblocked.getTo()!=removed);
                    for(EdgeVector::const_iterator i=incoming.begin(); i!=incoming.end(); i++) {
                        newBlocked.push_back(NBConnection(*i, sblocked.getTo()));
                    }
                } else if(sblocked.getTo()==removed) {
                    assert(sblocked.getFrom()!=removed);
                    for(EdgeVector::const_iterator i=outgoing.begin(); i!=outgoing.end(); i++) {
                        newBlocked.push_back(NBConnection(sblocked.getFrom(), *i));
                    }
                } else {
                    newBlocked.push_back(NBConnection(sblocked.getFrom(), sblocked.getTo()));
                }
            }
            if(blockedChanged) {
                blockedConnectionsNew[blocker] = newBlocked;
                changed = true;
            }
            // if the blocked were kept
            else {
                if(blocker.getFrom()==removed&&blocker.getTo()==removed) {
                    changed = true;
                    for(EdgeVector::const_iterator i=incoming.begin(); i!=incoming.end(); i++) {
                        blockedConnectionsNew[NBConnection(*i, *i)] = blocked;
                    }
                } else if(blocker.getFrom()==removed) {
                    assert(blocker.getTo()!=removed);
                    changed = true;
                    for(EdgeVector::const_iterator i=incoming.begin(); i!=incoming.end(); i++) {
                        blockedConnectionsNew[NBConnection(*i, blocker.getTo())] = blocked;
                    }
                } else if(blocker.getTo()==removed) {
                    assert(blocker.getFrom()!=removed);
                    changed = true;
                    for(EdgeVector::const_iterator i=outgoing.begin(); i!=outgoing.end(); i++) {
                        blockedConnectionsNew[NBConnection(blocker.getFrom(), *i)] = blocked;
                    }
                } else {
                    blockedConnectionsNew[blocker] = blocked;
                }
            }
        }
        _blockedConnections = blockedConnectionsNew;
    }
    // remap in traffic lights
    NBTrafficLightLogicCont::remapRemoved(removed, incoming, outgoing);
}


void
NBNode::addTrafficLight(NBTrafficLightDefinition *tld)
{
    myTrafficLights.insert(tld);
}


NBMMLDirection
NBNode::getMMLDirection(NBEdge *incoming, NBEdge *outgoing) const
{
    if(outgoing==0) {
        return MMLDIR_NODIR;
    }
    if(incoming->isTurningDirection(outgoing)) {
        return MMLDIR_TURN;
    }
    double angle =
        NBHelpers::normRelAngle(
            incoming->getAngle(), outgoing->getAngle());
    // ok, should be a straight connection
    if(abs(angle)<45) {
        return MMLDIR_STRAIGHT;
    }

    NBMMLDirection tmp;
    // check for left and right, first
    if(angle>0) {
        // check whether any othe edge outgoes further to right
        EdgeVector::const_iterator i =
            find(_allEdges.begin(), _allEdges.end(), outgoing);
        NBContHelper::nextCW(&_allEdges, i);
        while((*i)!=incoming) {
            if((*i)->getFromNode()==this) {
                return MMLDIR_PARTRIGHT;
            }
            NBContHelper::nextCW(&_allEdges, i);
        }
        return MMLDIR_RIGHT;
    } else {
        // check whether any othe edge outgoes further to right
        EdgeVector::const_iterator i =
            find(_allEdges.begin(), _allEdges.end(), outgoing);
        NBContHelper::nextCCW(&_allEdges, i);
        while((*i)!=incoming) {
            if((*i)->getFromNode()==this&&!incoming->isTurningDirection(*i)) {
                return MMLDIR_PARTLEFT;
            }
            NBContHelper::nextCCW(&_allEdges, i);
        }
        return MMLDIR_LEFT;
    }
    return tmp;
}


char
NBNode::stateCode(NBEdge *incoming, NBEdge *outgoing, int fromlane)
{
    if(outgoing==0) {
        return 'O'; // always off
    }
    if(mustBrake(incoming, outgoing, fromlane)) {
        return 'm'; // minor road
    }
    if(_type==NODETYPE_RIGHT_BEFORE_LEFT) {
        return '='; // all the same
    }
    // traffic lights are not regardedm here
    return 'M';
}


bool
NBNode::checkIsRemovable() const
{
    // check whether this node is included in a traffic light
    if(myTrafficLights.size()!=0) {
        return false;
    }
    EdgeVector::const_iterator i;
    // check whether the edges are allowed to be removed
    //  (!!! not completely implemented; should be parametrisable)
    for(i=_outgoingEdges->begin(); i!=_outgoingEdges->end(); i++) {
        if(!(*i)->isJoinable()) {
            return false;
        }
    }
    for(i=_incomingEdges->begin(); i!=_incomingEdges->end(); i++) {
        if(!(*i)->isJoinable()) {
            return false;
        }
    }
    // one in, one out -> just a geometry ...
    if(_outgoingEdges->size()==1&&_incomingEdges->size()==1) {
        // ... if types match ...
        if(!(*_incomingEdges)[0]->sameType((*_outgoingEdges)[0])) {
            return false;
        }
        //
        return (*_incomingEdges)[0]->getFromNode()!=(*_outgoingEdges)[0]->getToNode();
    }
    // two in, two out -> may be something else
    if(_outgoingEdges->size()==2&&_incomingEdges->size()==2) {
        // check whether the origin nodes of the incoming edges differ
        std::set<NBNode*> origSet;
        for(i=_incomingEdges->begin(); i!=_incomingEdges->end(); i++) {
            origSet.insert((*i)->getFromNode());
        }
        if(origSet.size()<2) {
            return false;
        }
        //
        for(i=_incomingEdges->begin(); i!=_incomingEdges->end(); i++) {
            // try to find the opposite direction
            NBNode *origin = (*i)->getFromNode();
            EdgeVector::const_iterator j =
                find_if(_outgoingEdges->begin(), _outgoingEdges->end(),
                    NBContHelper::edge_with_destination_finder(origin));
            if(j!=_outgoingEdges->end()) {
                NBContHelper::nextCCW(_outgoingEdges, j);
                // check whether the types allow joining
                if(!(*i)->sameType(*j)) {
                    return false;
                }
            } else {
                // ok, at least one outgoing edge is not an opposite
                //  of an incoming one
                return false;
            }
        }
        return true;
    }
    // ok, a real node
    return false;
}


std::vector<std::pair<NBEdge*, NBEdge*> >
NBNode::getEdgesToJoin() const
{
    assert(checkIsRemovable());
    std::vector<std::pair<NBEdge*, NBEdge*> > ret;
    // one in, one out-case
    if(_outgoingEdges->size()==1&&_incomingEdges->size()==1) {
        ret.push_back(
            std::pair<NBEdge*, NBEdge*>(
                (*_incomingEdges)[0], (*_outgoingEdges)[0]));
        return ret;
    }
    // two in, two out-case
    for(EdgeVector::const_iterator i=_incomingEdges->begin(); i!=_incomingEdges->end(); i++) {
        NBNode *origin = (*i)->getFromNode();
        EdgeVector::const_iterator j =
            find_if(_outgoingEdges->begin(), _outgoingEdges->end(),
                    NBContHelper::edge_with_destination_finder(origin));
        NBContHelper::nextCCW(_outgoingEdges, j);
        ret.push_back(std::pair<NBEdge*, NBEdge*>(*i, *j));
    }
    return ret;
}


const Position2DVector &
NBNode::getShape() const
{
    return myPoly;
}

std::string
NBNode::getInternalLaneID(NBEdge *from, size_t fromlane, NBEdge *to) const
{
    size_t l = 0;
    for(EdgeVector::const_iterator i=_incomingEdges->begin(); i!=_incomingEdges->end(); i++) {
        size_t noLanesEdge = (*i)->getNoLanes();
        for(size_t j=0; j<noLanesEdge; j++) {
            const EdgeLaneVector *elv = (*i)->getEdgeLanesFromLane(j);
            for(EdgeLaneVector::const_iterator k=elv->begin(); k!=elv->end(); k++) {
                if((*k).edge==0) {
                    continue;
                }
                if((from==*i)&&(j==fromlane)&&((*k).edge==to)) {
                    return string(":") + _id + string("_") +
                        toString<size_t>(l);
                }
                l++;
            }
        }
    }
}


bool
NBNode::isTLControlled() const
{
    return myTrafficLights.size()!=0;
}

/*
bool
NBNode::connectionIsTLControlled(NBEdge *from, NBEdge *to) const
{
    for(std::set<NBTrafficLightDefinition*>::const_iterator i=myTrafficLights.begin(); i!=myTrafficLights.end(); i++) {
        if((*i)->includes(from, to)) {
            return true;
        }
    }
    return false;
}
*/


/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/
//#ifdef DISABLE_INLINE
//#include "NBNode.icc"
//#endif

// Local Variables:
// mode:C++
// End:
