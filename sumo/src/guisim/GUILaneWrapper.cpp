/****************************************************************************/
/// @file    GUILaneWrapper.cpp
/// @author  Daniel Krajzewicz
/// @date    Mon, 25 Nov 2002
/// @version $Id$
///
// A MSLane extended for visualisation purposes.
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

#include <string>
#include <iostream>
#include <utility>
#include <microsim/MSLane.h>
#include <microsim/MSGlobals.h>
#include <utils/geom/PositionVector.h>
#include <microsim/MSNet.h>
#include <gui/GUIGlobals.h>
#include <utils/gui/windows/GUISUMOAbstractView.h>
#include "GUILaneWrapper.h"
#include <utils/common/ToString.h>
#include <utils/geom/GeomHelper.h>
#include <guisim/GUINet.h>
#include <utils/gui/windows/GUIAppEnum.h>
#include <utils/gui/images/GUIIconSubSys.h>
#include <utils/gui/div/GUIParameterTableWindow.h>
#include <utils/gui/globjects/GUIGLObjectPopupMenu.h>
#include <gui/GUIApplicationWindow.h>
#include <utils/gui/div/GUIGlobalSelection.h>
#include <utils/common/RandHelper.h>
#include <utils/gui/div/GLHelper.h>
#include <gui/GUIViewTraffic.h>
#include <utils/gui/images/GUITexturesHelper.h>
#include <guisim/GUIVehicle.h>
#include <foreign/polyfonts/polyfonts.h>
#include <utils/common/HelpersHarmonoise.h>
#include <microsim/MSEdgeWeightsStorage.h>


#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif // CHECK_MEMORY_LEAKS


// ===========================================================================
// static member definitions
// ===========================================================================
SUMOReal GUILaneWrapper::myAllMaxSpeed = 0;


// ===========================================================================
// method definitions
// ===========================================================================
GUILaneWrapper::GUILaneWrapper(MSLane &lane, const PositionVector &shape, unsigned int index) throw() : 
    GUIGlObject(GLO_LANE, lane.getID()),
    myLane(lane), 
    myShape(shape),
    myIndex(index)
{
    SUMOReal x1 = shape[0].x();
    SUMOReal y1 = shape[0].y();
    SUMOReal x2 = shape[-1].x();
    SUMOReal y2 = shape[-1].y();
    // also the virtual length is set in here
    myVisLength = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
    // check maximum speed
    if (myAllMaxSpeed<lane.getMaxSpeed()) {
        myAllMaxSpeed = lane.getMaxSpeed();
    }
    //
    myShapeRotations.reserve(myShape.size()-1);
    myShapeLengths.reserve(myShape.size()-1);
    int e = (int) myShape.size() - 1;
    for (int i=0; i<e; ++i) {
        const Position &f = myShape[i];
        const Position &s = myShape[i+1];
        myShapeLengths.push_back(f.distanceTo(s));
        myShapeRotations.push_back((SUMOReal) atan2((s.x()-f.x()), (f.y()-s.y()))*(SUMOReal) 180.0/(SUMOReal) PI);
    }
    //
    myHalfLaneWidth = (SUMOReal) (myLane.getWidth() / 2.);
    myQuarterLaneWidth = (SUMOReal) (myLane.getWidth() / 4.);
}


GUILaneWrapper::~GUILaneWrapper() throw() {}


SUMOReal
GUILaneWrapper::getOverallMaxSpeed() {
    return myAllMaxSpeed;
}


bool
GUILaneWrapper::forLane(const MSLane &lane) const {
    return (&myLane)==(&lane);
}



void
GUILaneWrapper::ROWdrawAction_drawLinkNo() const {
    unsigned int noLinks = getLinkNumber();
    if (noLinks==0) {
        return;
    }

    // draw all links
    SUMOReal w = myLane.getWidth() / (SUMOReal) noLinks;
    SUMOReal x1 = myLane.getWidth() / (SUMOReal) 2.;
    glPushMatrix();
    glColor3d(.5, .5, 1);
    const PositionVector &g = getShape();
    const Position &end = g.getEnd();
    const Position &f = g[-2];
    const Position &s = end;
    SUMOReal rot = (SUMOReal) atan2((s.x()-f.x()), (f.y()-s.y()))*(SUMOReal) 180.0/(SUMOReal) PI;
    glTranslated(end.x(), end.y(), 0);
    glRotated(rot, 0, 0, 1);
    for (unsigned int i=0; i<noLinks; ++i) {
        SUMOReal x2 = x1 - (SUMOReal)(w/2.);
        int linkNo = getLane().getLinkCont()[i]->getRespondIndex();
        glPushMatrix();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        pfSetPosition(0, 0);
        pfSetScale(1);
        SUMOReal tw = pfdkGetStringWidth(toString(linkNo).c_str());
        glRotated(180, 0, 1, 0);
        glTranslated(x2-tw/2., 0.5, 0);
        pfDrawString(toString(linkNo).c_str());
        glPopMatrix();
        x1 -= w;
    }
    glPopMatrix();
}


void
GUILaneWrapper::ROWdrawAction_drawTLSLinkNo(const GUINet &net) const {
    unsigned int noLinks = getLinkNumber();
    if (noLinks==0) {
        return;
    }

    // draw all links
    SUMOReal w = myLane.getWidth() / (SUMOReal) noLinks;
    SUMOReal x1 = (SUMOReal)(myLane.getWidth() / 2.);
    glPushMatrix();
    glColor3d(.5, .5, 1);
    const PositionVector &g = getShape();
    const Position &end = g.getEnd();
    const Position &f = g[-2];
    const Position &s = end;
    SUMOReal rot = (SUMOReal) atan2((s.x()-f.x()), (f.y()-s.y()))*(SUMOReal) 180.0/(SUMOReal) PI;
    glTranslated(end.x(), end.y(), 0);
    glRotated(rot, 0, 0, 1);
    for (unsigned int i=0; i<noLinks; ++i) {
        SUMOReal x2 = x1 - (SUMOReal)(w/2.);
        int linkNo = net.getLinkTLIndex(getLane().getLinkCont()[i]);
        if (linkNo<0) {
            continue;
        }
        glPushMatrix();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        pfSetPosition(0, 0);
        pfSetScale(1);
        SUMOReal tw = pfdkGetStringWidth(toString(linkNo).c_str());
        glRotated(180, 0, 1, 0);
        glTranslated(x2-tw/2., 0.5, 0);
        pfDrawString(toString(linkNo).c_str());
        glPopMatrix();
        x1 -= w;
    }
    glPopMatrix();
}


void
GUILaneWrapper::ROWdrawAction_drawLinkRules(const GUINet &net) const {
    unsigned int noLinks = getLinkNumber();
    const PositionVector &g = getShape();
    const Position &end = g.getEnd();
    const Position &f = g[-2];
    const Position &s = end;
    SUMOReal rot = (SUMOReal) atan2((s.x()-f.x()), (f.y()-s.y()))*(SUMOReal) 180.0/(SUMOReal) PI;
    if (noLinks==0) {
        glPushName(getGlID());
        // draw a grey bar if no links are on the street
        glColor3d(0.5, 0.5, 0.5);
        glPushMatrix();
        glTranslated(end.x(), end.y(), 0);
        glRotated(rot, 0, 0, 1);
        glBegin(GL_QUADS);
        glVertex2d(-myHalfLaneWidth, 0.0);
        glVertex2d(-myHalfLaneWidth, 0.5);
        glVertex2d(myHalfLaneWidth, 0.5);
        glVertex2d(myHalfLaneWidth, 0.0);
        glEnd();
        glPopMatrix();
        glPopName();
        return;
    }
    // draw all links
    SUMOReal w = myLane.getWidth() / (SUMOReal) noLinks;
    SUMOReal x1 = 0;
    glPushMatrix();
    glTranslated(end.x(), end.y(), 0);
    glRotated(rot, 0, 0, 1);
    for (unsigned int i=0; i<noLinks; ++i) {
        SUMOReal x2 = x1 + w;
        MSLink::LinkState state = getLane().getLinkCont()[i]->getState();
        switch (state) {
            case MSLink::LINKSTATE_TL_GREEN_MAJOR:
            case MSLink::LINKSTATE_TL_GREEN_MINOR:
            case MSLink::LINKSTATE_TL_RED:
            case MSLink::LINKSTATE_TL_YELLOW_MAJOR:
            case MSLink::LINKSTATE_TL_YELLOW_MINOR:
            case MSLink::LINKSTATE_TL_OFF_BLINKING:
                glPushName(net.getLinkTLID(getLane().getLinkCont()[i]));
                break;
            case MSLink::LINKSTATE_MAJOR:
            case MSLink::LINKSTATE_MINOR:
            case MSLink::LINKSTATE_EQUAL:
            case MSLink::LINKSTATE_TL_OFF_NOSIGNAL:
            default:
                glPushName(getGlID());
                break;
        }
        switch (state) {
        case MSLink::LINKSTATE_TL_GREEN_MAJOR:
        case MSLink::LINKSTATE_TL_GREEN_MINOR:
            glColor3d(0, 1, 0);
            break;
        case MSLink::LINKSTATE_TL_RED:
            glColor3d(1, 0, 0);
            break;
        case MSLink::LINKSTATE_TL_YELLOW_MAJOR:
        case MSLink::LINKSTATE_TL_YELLOW_MINOR:
            glColor3d(1, 1, 0);
            break;
        case MSLink::LINKSTATE_TL_OFF_BLINKING:
            glColor3d(1, 1, 0);
            break;
        case MSLink::LINKSTATE_TL_OFF_NOSIGNAL:
            glColor3d(0, 1, 1);
            break;
        case MSLink::LINKSTATE_MAJOR:
            glColor3d(1, 1, 1);
            break;
        case MSLink::LINKSTATE_MINOR:
            glColor3d(.2, .2, .2);
            break;
        case MSLink::LINKSTATE_EQUAL:
            glColor3d(.5, .5, .5);
            break;
        case MSLink::LINKSTATE_DEADEND:
            glColor3d(0, 0, 0);
            break;
        }
        glBegin(GL_QUADS);
        glVertex2d(x1-myHalfLaneWidth, 0.0);
        glVertex2d(x1-myHalfLaneWidth, 0.5);
        glVertex2d(x2-myHalfLaneWidth, 0.5);
        glVertex2d(x2-myHalfLaneWidth,0.0);
        glEnd();
        glPopName();
        x1 = x2;
        x2 += w;
    }
    glPopMatrix();
}


void
GUILaneWrapper::ROWdrawAction_drawArrows() const {
    unsigned int noLinks = getLinkNumber();
    if (noLinks==0) {
        return;
    }
    // draw all links
    const Position &end = getShape().getEnd();
    const Position &f = getShape()[-2];
    SUMOReal rot = (SUMOReal) atan2((end.x()-f.x()), (f.y()-end.y()))*(SUMOReal) 180.0/(SUMOReal) PI;
    glPushMatrix();
    glPushName(0);
    glColor3d(1, 1, 1);
    glTranslated(end.x(), end.y(), 0);
    glRotated(rot, 0, 0, 1);
    for (unsigned int i=0; i<noLinks; ++i) {
        MSLink::LinkDirection dir = getLane().getLinkCont()[i]->getDirection();
        MSLink::LinkState state = getLane().getLinkCont()[i]->getState();
        if (state==MSLink::LINKSTATE_TL_OFF_NOSIGNAL||dir==MSLink::LINKDIR_NODIR) {
            continue;
        }
        switch (dir) {
        case MSLink::LINKDIR_STRAIGHT:
            GLHelper::drawBoxLine(Position(0, 4), 0, 2, .05);
            GLHelper::drawTriangleAtEnd(Line(Position(0, 4), Position(0, 1)), (SUMOReal) 1, (SUMOReal) .25);
            break;
        case MSLink::LINKDIR_TURN:
            GLHelper::drawBoxLine(Position(0, 4), 0, 1.5, .05);
            GLHelper::drawBoxLine(Position(0, 2.5), 90, .5, .05);
            GLHelper::drawBoxLine(Position(0.5, 2.5), 180, 1, .05);
            GLHelper::drawTriangleAtEnd(Line(Position(0.5, 2.5), Position(0.5, 4)), (SUMOReal) 1, (SUMOReal) .25);
            break;
        case MSLink::LINKDIR_LEFT:
            GLHelper::drawBoxLine(Position(0, 4), 0, 1.5, .05);
            GLHelper::drawBoxLine(Position(0, 2.5), 90, 1, .05);
            GLHelper::drawTriangleAtEnd(Line(Position(0, 2.5), Position(1.5, 2.5)), (SUMOReal) 1, (SUMOReal) .25);
            break;
        case MSLink::LINKDIR_RIGHT:
            GLHelper::drawBoxLine(Position(0, 4), 0, 1.5, .05);
            GLHelper::drawBoxLine(Position(0, 2.5), -90, 1, .05);
            GLHelper::drawTriangleAtEnd(Line(Position(0, 2.5), Position(-1.5, 2.5)), (SUMOReal) 1, (SUMOReal) .25);
            break;
        case MSLink::LINKDIR_PARTLEFT:
            GLHelper::drawBoxLine(Position(0, 4), 0, 1.5, .05);
            GLHelper::drawBoxLine(Position(0, 2.5), 45, .7, .05);
            GLHelper::drawTriangleAtEnd(Line(Position(0, 2.5), Position(1.2, 1.3)), (SUMOReal) 1, (SUMOReal) .25);
            break;
        case MSLink::LINKDIR_PARTRIGHT:
            GLHelper::drawBoxLine(Position(0, 4), 0, 1.5, .05);
            GLHelper::drawBoxLine(Position(0, 2.5), -45, .7, .05);
            GLHelper::drawTriangleAtEnd(Line(Position(0, 2.5), Position(-1.2, 1.3)), (SUMOReal) 1, (SUMOReal) .25);
            break;
        }
    }
    glPopMatrix();
    glPopName();
}


void
GUILaneWrapper::ROWdrawAction_drawLane2LaneConnections() const {
    unsigned int noLinks = getLinkNumber();
    for (unsigned int i=0; i<noLinks; ++i) {
        MSLink::LinkState state = getLane().getLinkCont()[i]->getState();
        const MSLane *connected = getLane().getLinkCont()[i]->getLane();
        if (connected==0) {
            continue;
        }
        switch (state) {
        case MSLink::LINKSTATE_TL_GREEN_MAJOR:
        case MSLink::LINKSTATE_TL_GREEN_MINOR:
            glColor3d(0, 1, 0);
            break;
        case MSLink::LINKSTATE_TL_RED:
            glColor3d(1, 0, 0);
            break;
        case MSLink::LINKSTATE_TL_YELLOW_MAJOR:
        case MSLink::LINKSTATE_TL_YELLOW_MINOR:
            glColor3d(1, 1, 0);
            break;
        case MSLink::LINKSTATE_TL_OFF_BLINKING:
            glColor3d(1, 1, 0);
            break;
        case MSLink::LINKSTATE_TL_OFF_NOSIGNAL:
            glColor3d(0, 1, 1);
            break;
        case MSLink::LINKSTATE_MAJOR:
            glColor3d(1, 1, 1);
            break;
        case MSLink::LINKSTATE_MINOR:
            glColor3d(.2, .2, .2);
            break;
        case MSLink::LINKSTATE_EQUAL:
            glColor3d(.5, .5, .5);
            break;
        case MSLink::LINKSTATE_DEADEND:
            glColor3d(0, 0, 0);
            break;
        }

        glBegin(GL_LINES);
        const Position &p1 = getShape()[-1];
        const Position &p2 = connected->getShape()[0];
        glVertex2f(p1.x(), p1.y());
        glVertex2f(p2.x(), p2.y());
        glEnd();
        GLHelper::drawTriangleAtEnd(Line(p1, p2), (SUMOReal) .4, (SUMOReal) .2);
    }
}


void
GUILaneWrapper::drawGL(const GUIVisualizationSettings &s) const throw() {
    glPushMatrix();
    const bool isInternal = getLane().getEdge().getPurpose()==MSEdge::EDGEFUNCTION_INTERNAL;
    bool mustDrawMarkings = false;
    if (isInternal) {
        // draw internal lanes on top of junctions
        glTranslated(0, 0, GLO_JUNCTION + 0.1);
    } else {
        glTranslated(0, 0, getType());
    }
    // set lane color
#ifdef HAVE_MESOSIM
    if (!MSGlobals::gUseMesoSim)
#endif
        s.laneColorer.setGlColor(*this);
    glPushName(getGlID());
    // draw lane
    // check whether it is not too small
    if (s.scale<1.) {
        GLHelper::drawLine(myShape);
        glPopName();
        glPopMatrix();
    } else {
        if (!isInternal) {
            GLHelper::drawBoxLines(myShape, myShapeRotations, myShapeLengths, myHalfLaneWidth);
            mustDrawMarkings = true;
        } else {
            GLHelper::drawBoxLines(myShape, myShapeRotations, myShapeLengths, myQuarterLaneWidth);
        }
        glPopName();
        glPopMatrix();
        // draw ROWs (not for inner lanes)
        if (!isInternal) {
            glPushMatrix();
            glTranslated(0, 0, GLO_JUNCTION); // must draw on top of junction shape
            GUINet *net = (GUINet*) MSNet::getInstance();
            glTranslated(0, 0, .2);
            ROWdrawAction_drawLinkRules(*net);
            if (s.showLinkDecals) {
                ROWdrawAction_drawArrows();
            }
            if (s.showLane2Lane) {
                // this should be independent to the geometry:
                //  draw from end of first to the begin of second
                ROWdrawAction_drawLane2LaneConnections();
            }
            glTranslated(0, 0, .1);
            if (s.drawLinkJunctionIndex) {
                ROWdrawAction_drawLinkNo();
            }
            if (s.drawLinkTLIndex) {
                ROWdrawAction_drawTLSLinkNo(*net);
            }
            glPopMatrix();
        }
    }
    if (mustDrawMarkings) { // needs matrix reset
        drawMarkings(s);
    }
    // draw vehicles
    if (s.scale>s.minVehicleSize) {
        // retrieve vehicles from lane; disallow simulation
        const MSLane::VehCont &vehicles = myLane.getVehiclesSecure();
        for (MSLane::VehCont::const_iterator v=vehicles.begin(); v!=vehicles.end(); ++v) {
            static_cast<const GUIVehicle*const>(*v)->drawGL(s);
        }
        // allow lane simulation
        myLane.releaseVehicles();
    }
}


void
GUILaneWrapper::drawMarkings(const GUIVisualizationSettings &s) const {
    glPushMatrix();
    glPushName(0);
    glTranslated(0, 0, GLO_EDGE);
#ifdef HAVE_MESOSIM
    if (!MSGlobals::gUseMesoSim)
#endif
        s.laneColorer.setGlColor(*this);
    // optionally draw inverse markings 
    if (myIndex > 0) {
        int e = (int) getShape().size() - 1;
        for (int i=0; i<e; i++) {
            glPushMatrix();
            glTranslated(getShape()[i].x(), getShape()[i].y(), 0.1);
            glRotated(myShapeRotations[i], 0, 0, 1);
            for (SUMOReal t=0; t<myShapeLengths[i]; t+=6) {
                glBegin(GL_QUADS);
                glVertex2d(-1.8, -t);
                glVertex2d(-1.8, -t-3.);
                glVertex2d(1.0, -t-3.);
                glVertex2d(1.0, -t);
                glEnd();
            }
            glPopMatrix();
        }
    }
    // draw white boundings and white markings
    glColor3d(1,1,1);
    GLHelper::drawBoxLines(
            getShape(), 
            getShapeRotations(), 
            getShapeLengths(), 
            getHalfWidth() + SUMO_const_laneOffset);
    glPopMatrix();
    glPopName();
}


GUIGLObjectPopupMenu *
GUILaneWrapper::getPopUpMenu(GUIMainWindow &app,
                             GUISUMOAbstractView &parent) throw() {
    GUIGLObjectPopupMenu *ret = new GUIGLObjectPopupMenu(app, parent, *this);
    buildPopupHeader(ret, app);
    buildCenterPopupEntry(ret);
    //
    buildNameCopyPopupEntry(ret);
    buildSelectionPopupEntry(ret);
    //
    buildShowParamsPopupEntry(ret, false);
    SUMOReal pos = myShape.nearest_position_on_line_to_point(parent.getPositionInformation());
    new FXMenuCommand(ret, ("pos: " + toString(pos)).c_str(), 0, 0, 0);
    new FXMenuSeparator(ret);
    buildPositionCopyEntry(ret, false);
    return ret;
}


GUIParameterTableWindow *
GUILaneWrapper::getParameterWindow(GUIMainWindow &app,
                                   GUISUMOAbstractView &) throw() {
    GUIParameterTableWindow *ret =
        new GUIParameterTableWindow(app, *this, 2);
    // add items
    ret->mkItem("maxspeed [m/s]", false, myLane.getMaxSpeed());
    ret->mkItem("length [m]", false, myLane.getLength());
    // close building
    ret->closeBuilding();
    return ret;
}


Boundary
GUILaneWrapper::getCenteringBoundary() const throw() {
    Boundary b;
    b.add(myShape[0]);
    b.add(myShape[-1]);
    b.grow(20);
    return b;
}




const PositionVector &
GUILaneWrapper::getShape() const {
    return myShape;
}


unsigned int
GUILaneWrapper::getLinkNumber() const {
    return (unsigned int) myLane.getLinkCont().size();
}


const DoubleVector &
GUILaneWrapper::getShapeRotations() const {
    return myShapeRotations;
}


const DoubleVector &
GUILaneWrapper::getShapeLengths() const {
    return myShapeLengths;
}


SUMOReal
GUILaneWrapper::firstWaitingTime() const {
    return myLane.myVehicles.size()==0
           ? 0
           : (*(myLane.myVehicles.end()-1))->getWaitingSeconds();
}


SUMOReal
GUILaneWrapper::getEdgeLaneNumber() const {
    return (SUMOReal) myLane.getEdge().getLanes().size();
}


// ------------ Current state retrieval
SUMOReal
GUILaneWrapper::getNormedHBEFA_CO2Emissions() const throw() {
    return myLane.getHBEFA_CO2Emissions() / myLane.getLength();
}


SUMOReal
GUILaneWrapper::getNormedHBEFA_COEmissions() const throw() {
    return myLane.getHBEFA_COEmissions() / myLane.getLength();
}


SUMOReal
GUILaneWrapper::getNormedHBEFA_PMxEmissions() const throw() {
    return myLane.getHBEFA_PMxEmissions() / myLane.getLength();
}


SUMOReal
GUILaneWrapper::getNormedHBEFA_NOxEmissions() const throw() {
    return myLane.getHBEFA_NOxEmissions() / myLane.getLength();
}


SUMOReal
GUILaneWrapper::getNormedHBEFA_HCEmissions() const throw() {
    return myLane.getHBEFA_HCEmissions() / myLane.getLength();
}


SUMOReal
GUILaneWrapper::getNormedHBEFA_FuelConsumption() const throw() {
    return myLane.getHBEFA_FuelConsumption() / myLane.getLength();
}



// ------------
GUILaneWrapper::Colorer::Colorer() {
    mySchemes.push_back(GUIColorScheme("uniform", RGBColor(0,0,0), "", true));
    mySchemes.push_back(GUIColorScheme("by selection (lane-/streetwise)", RGBColor(0.7f, 0.7f, 0.7f), "unselected", true));
    mySchemes.back().addColor(RGBColor(0, .4f, .8f), 1, "selected");
    mySchemes.push_back(GUIColorScheme("by vclass", RGBColor(0,0,0), "all", true));
    mySchemes.back().addColor(RGBColor(0, .1f, .5f), 1, "public");
    // ... traffic states ...
    mySchemes.push_back(GUIColorScheme("by allowed speed (lanewise)", RGBColor(1,0,0)));
    mySchemes.back().addColor(RGBColor(0, 0, 1), (SUMOReal)(150.0/3.6));
    mySchemes.push_back(GUIColorScheme("by current occupancy (lanewise)", RGBColor(0,0,1)));
    mySchemes.back().addColor(RGBColor(1, 0, 0), (SUMOReal)0.95);
    mySchemes.push_back(GUIColorScheme("by first vehicle waiting time (lanewise)", RGBColor(0,1,0)));
    mySchemes.back().addColor(RGBColor(1,0,0), (SUMOReal)200);
    mySchemes.push_back(GUIColorScheme("by lane number (streetwise)", RGBColor(1,0,0)));
    mySchemes.back().addColor(RGBColor(0,0,1), (SUMOReal)5);
    // ... emissions ...
    mySchemes.push_back(GUIColorScheme("by CO2 emissions (HBEFA)", RGBColor(0,1,0)));
    mySchemes.back().addColor(RGBColor(1,0,0), (SUMOReal)(10./7.5/5.));
    mySchemes.push_back(GUIColorScheme("by CO emissions (HBEFA)", RGBColor(0,1,0)));
    mySchemes.back().addColor(RGBColor(1,0,0), (SUMOReal)(0.05/7.5/2.));
    mySchemes.push_back(GUIColorScheme("by PMx emissions (HBEFA)", RGBColor(0,1,0)));
    mySchemes.back().addColor(RGBColor(1,0,0), (SUMOReal)(.005/7.5/5.));
    mySchemes.push_back(GUIColorScheme("by NOx emissions (HBEFA)", RGBColor(0,1,0)));
    mySchemes.back().addColor(RGBColor(1,0,0), (SUMOReal)(.125/7.5/5.));
    mySchemes.push_back(GUIColorScheme("by HC emissions (HBEFA)", RGBColor(0,1,0)));
    mySchemes.back().addColor(RGBColor(1,0,0), (SUMOReal)(.02/7.5/4.));
    mySchemes.push_back(GUIColorScheme("by fuel consumption (HBEFA)", RGBColor(0,1,0)));
    mySchemes.back().addColor(RGBColor(1,0,0), (SUMOReal)(.005/7.5*100.));
    mySchemes.push_back(GUIColorScheme("by noise emissions (Harmonoise)", RGBColor(0,1,0)));
    mySchemes.back().addColor(RGBColor(1,0,0), (SUMOReal)100);
    // ... weights (experimental) ...
    mySchemes.push_back(GUIColorScheme("by global travel time", RGBColor(0,1,0)));
    mySchemes.back().addColor(RGBColor(1,0,0), (SUMOReal)100);
    mySchemes.back().setAllowsNegativeValues(true);
    mySchemes.push_back(GUIColorScheme("by global speed percentage", RGBColor(1,0,0)));
    mySchemes.back().addColor(RGBColor(1,1,0), (SUMOReal)50);
    mySchemes.back().addColor(RGBColor(0,1,0), (SUMOReal)100);
    mySchemes.back().setAllowsNegativeValues(true);
}


SUMOReal
GUILaneWrapper::Colorer::getColorValue(const GUILaneWrapper& lane) const {
    switch (myActiveScheme) {
    case 1:
        return gSelected.isSelected(lane.getType(), lane.getGlID());
    case 2: {
        const SUMOVehicleClasses &allowed = lane.myLane.getAllowedClasses();
        const SUMOVehicleClasses &disallowed = lane.myLane.getNotAllowedClasses();
        if ((allowed.size()==0 || find(allowed.begin(), allowed.end(), SVC_PASSENGER)!=allowed.end()) && find(disallowed.begin(), disallowed.end(), SVC_PASSENGER)==disallowed.end()) {
            return 0;
        } else {
            return 1;
        }
    }
    case 3:
        return lane.getLane().getMaxSpeed();
    case 4:
        return lane.getLane().getOccupancy();
    case 5:
        return lane.firstWaitingTime();
    case 6:
        return lane.getEdgeLaneNumber();
    case 7:
        return lane.getNormedHBEFA_CO2Emissions();
    case 8:
        return lane.getNormedHBEFA_COEmissions();
    case 9:
        return lane.getNormedHBEFA_PMxEmissions();
    case 10:
        return lane.getNormedHBEFA_NOxEmissions();
    case 11:
        return lane.getNormedHBEFA_HCEmissions();
    case 12:
        return lane.getNormedHBEFA_FuelConsumption();
    case 13:
        return lane.getLane().getHarmonoise_NoiseEmissions();
    case 14: {
        MSEdgeWeightsStorage &ews = MSNet::getInstance()->getWeightsStorage();
        MSEdge &e = lane.getLane().getEdge();
        if(!ews.knowsTravelTime(&e)) {
            return -1;
        } else {
            SUMOReal value(0);
            ews.retrieveExistingTravelTime(&e, 0, 0, value);
            return value;
        }
             }
    case 15: {
        MSEdgeWeightsStorage &ews = MSNet::getInstance()->getWeightsStorage();
        MSEdge &e = lane.getLane().getEdge();
        if(!ews.knowsTravelTime(&e)) {
            return -1;
        } else {
            SUMOReal value(0);
            ews.retrieveExistingTravelTime(&e, 0, 0, value);
            return (lane.getLane().getLength() / lane.getLane().getMaxSpeed()) / (lane.getLane().getMaxSpeed() / value);
        }
             }
    }
    return 0;
}

/****************************************************************************/

