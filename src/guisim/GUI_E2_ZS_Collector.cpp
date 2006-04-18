//---------------------------------------------------------------------------//
//                        GUI_E2_ZS_Collector.cpp -
//  The gui-version of the MS_E2_ZS_Collector, together with the according
//   wrapper
//                           -------------------
//  project              : SUMO - Simulation of Urban MObility
//  begin                : Okt 2003
//  copyright            : (C) 2003 by Daniel Krajzewicz
//  organisation         : IVF/DLR http://ivf.dlr.de
//  email                : Daniel.Krajzewicz@dlr.de
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//---------------------------------------------------------------------------//
namespace
{
    const char rcsid[] =
    "$Id$";
}
// $Log$
// Revision 1.21  2006/04/18 08:12:04  dkrajzew
// consolidation of interaction with gl-objects
//
// Revision 1.20  2006/04/11 10:56:32  dkrajzew
// microsimID() now returns a const reference
//
// Revision 1.19  2006/04/05 05:22:36  dkrajzew
// retrieval of microsim ids is now also done using getID() instead of id()
//
// Revision 1.18  2006/03/28 06:12:54  dkrajzew
// unneeded string wrapping removed
//
// Revision 1.17  2006/03/27 07:24:45  dkrajzew
// extracted drawing of lane geometries
//
// Revision 1.16  2006/03/17 11:03:02  dkrajzew
// made access to positions in Position2DVector c++ compliant
//
// Revision 1.15  2006/01/09 11:50:21  dkrajzew
// new visualization settings implemented
//
// Revision 1.14  2005/10/07 11:37:17  dkrajzew
// THIRD LARGE CODE RECHECK: patched problems on Linux/Windows configs
//
// Revision 1.13  2005/09/22 13:39:35  dkrajzew
// SECOND LARGE CODE RECHECK: converted doubles and floats to SUMOReal
//
// Revision 1.12  2005/09/15 11:06:37  dkrajzew
// LARGE CODE RECHECK
//
// Revision 1.11  2005/05/04 07:59:59  dkrajzew
// level 3 warnings removed; a certain SUMOTime time description added
//
// Revision 1.10  2004/11/24 08:46:42  dkrajzew
// recent changes applied
//
// Revision 1.9  2004/07/02 08:40:42  dkrajzew
// changes in the detector drawer applied
//
// Revision 1.8  2004/03/19 12:57:55  dkrajzew
// porting to FOX
//
// Revision 1.7  2004/01/26 06:59:38  dkrajzew
// work on detectors: e3-detectors loading and visualisation;
//  variable offsets and lengths for lsa-detectors; coupling of detectors to
//  tl-logics;
//  different detector visualistaion in dependence to his controller
//
// Revision 1.6  2003/12/04 13:31:28  dkrajzew
// detector name changes applied
//
// Revision 1.5  2003/11/18 14:27:39  dkrajzew
// debugged and completed lane merging detectors
//
// Revision 1.4  2003/11/12 14:00:19  dkrajzew
// commets added; added parameter windows to all detectors
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

#include <microsim/output/MSInductLoop.h>
#include <utils/gui/globjects/GUIGlObject.h>
#include <utils/geom/Position2DVector.h>
#include "GUILaneWrapper.h"
#include "GUI_E2_ZS_Collector.h"
#include <utils/glutils/GLHelper.h>
#include <utils/geom/Line2D.h>
#include <utils/geom/GeomHelper.h>
#include <utils/gui/div/GUIParameterTableWindow.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

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
/* -------------------------------------------------------------------------
 * GUI_E2_ZS_Collector-methods
 * ----------------------------------------------------------------------- */
GUI_E2_ZS_Collector::GUI_E2_ZS_Collector( std::string id, DetectorUsage usage,
        MSLane* lane, MSUnit::Meters startPos, MSUnit::Meters detLength,
        MSUnit::Seconds haltingTimeThreshold,
        MSUnit::MetersPerSecond haltingSpeedThreshold,
        MSUnit::Meters jamDistThreshold,
        SUMOTime deleteDataAfterSeconds)
    : MSE2Collector(id, usage, lane, startPos, detLength, haltingTimeThreshold,
            haltingSpeedThreshold, jamDistThreshold, deleteDataAfterSeconds)
{
}



GUI_E2_ZS_Collector::~GUI_E2_ZS_Collector()
{
}


GUIDetectorWrapper *
GUI_E2_ZS_Collector::buildDetectorWrapper(GUIGlObjectStorage &idStorage,
                                          GUILaneWrapper &wrapper)
{
    return new MyWrapper(*this, idStorage, wrapper);
}

GUIDetectorWrapper *
GUI_E2_ZS_Collector::buildDetectorWrapper(GUIGlObjectStorage &idStorage,
                                          GUILaneWrapper &wrapper,
                                          GUI_E2_ZS_CollectorOverLanes& p,
                                          size_t glID)
{
    return new MyWrapper(*this, idStorage, glID, p, wrapper);
}



/* -------------------------------------------------------------------------
 * GUI_E2_ZS_Collector::MyWrapper-methods
 * ----------------------------------------------------------------------- */
GUI_E2_ZS_Collector::MyWrapper::MyWrapper(GUI_E2_ZS_Collector &detector,
                                          GUIGlObjectStorage &idStorage,
                                          GUILaneWrapper &wrapper)
    : GUIDetectorWrapper(idStorage, "E2 detector:"+detector.getID()),
    myDetector(detector)
{
    myConstruct(detector, wrapper);
}


GUI_E2_ZS_Collector::MyWrapper::MyWrapper(
        GUI_E2_ZS_Collector &detector, GUIGlObjectStorage &idStorage,
        size_t glID, GUI_E2_ZS_CollectorOverLanes &mustBe,
        GUILaneWrapper &wrapper)
    : GUIDetectorWrapper(idStorage, "E2 detector:"+detector.getID(), glID),
    myDetector(detector)
{
    myConstruct(detector, wrapper);
}

void
GUI_E2_ZS_Collector::MyWrapper::myConstruct(GUI_E2_ZS_Collector &detector,
                                            GUILaneWrapper &wrapper)
{
    const Position2DVector &v = wrapper.getShape();
    Line2D l(v.getBegin(), v.getEnd());
    mySGPosition = l.getPositionAtDistance(detector.getStartPos());
    Position2D endPos = l.getPositionAtDistance(detector.getEndPos());
    mySGRotation = -l.atan2DegreeAngle();
    mySGLength = GeomHelper::distance(mySGPosition, endPos);

    // build geometry
    myFullGeometry = v.getSubpart(detector.getStartPos(), detector.getEndPos());
    //
    myShapeRotations.reserve(myFullGeometry.size()-1);
    myShapeLengths.reserve(myFullGeometry.size()-1);
    for(size_t i=0; i<myFullGeometry.size()-1; i++) {
        const Position2D &f = myFullGeometry[i];
        const Position2D &s = myFullGeometry[i+1];
        myShapeLengths.push_back(GeomHelper::distance(f, s));
        myShapeRotations.push_back((SUMOReal) atan2((s.x()-f.x()), (f.y()-s.y()))*(SUMOReal) 180.0/(SUMOReal) 3.14159265);
    }
    //
    myBoundary = myFullGeometry.getBoxBoundary();
    myBoundary.add(mySGPosition);
    myBoundary.add(endPos);
}


GUI_E2_ZS_Collector::MyWrapper::~MyWrapper()
{
}


Boundary
GUI_E2_ZS_Collector::MyWrapper::getBoundary() const
{
    return myBoundary;
}


GUIParameterTableWindow *
GUI_E2_ZS_Collector::MyWrapper::getParameterWindow(GUIMainWindow &app,
                                                   GUISUMOAbstractView &parent)
{
    GUIParameterTableWindow *ret =
        new GUIParameterTableWindow(app, *this, 14);
    // add items
    myMkExistingItem(*ret, "density [?]", E2::DENSITY);
    myMkExistingItem(*ret, "jam lengths [veh]", E2::MAX_JAM_LENGTH_IN_VEHICLES);
    myMkExistingItem(*ret, "jam length [m]", E2::MAX_JAM_LENGTH_IN_METERS);
    myMkExistingItem(*ret, "jam len sum [veh]", E2::JAM_LENGTH_SUM_IN_VEHICLES);
    myMkExistingItem(*ret, "jam len sum [m]", E2::JAM_LENGTH_SUM_IN_METERS);
    myMkExistingItem(*ret, "queue length [veh]", E2::QUEUE_LENGTH_AHEAD_OF_TRAFFIC_LIGHTS_IN_VEHICLES);
    myMkExistingItem(*ret, "queue length [m]", E2::QUEUE_LENGTH_AHEAD_OF_TRAFFIC_LIGHTS_IN_METERS);
    myMkExistingItem(*ret, "vehicles [veh]", E2::N_VEHICLES);
    myMkExistingItem(*ret, "occupancy degree [?]", E2::OCCUPANCY_DEGREE);
    myMkExistingItem(*ret, "space mean speed [?]", E2::SPACE_MEAN_SPEED);
    myMkExistingItem(*ret, "halting duration [?]", E2::CURRENT_HALTING_DURATION_SUM_PER_VEHICLE);
    //
    ret->mkItem("length [m]", false, myDetector.getEndPos()-myDetector.getStartPos());
    ret->mkItem("position [m]", false, myDetector.getStartPos());
    ret->mkItem("lane", false, myDetector.getLane()->getID());
    // close building
    ret->closeBuilding();
    return ret;
}


void
GUI_E2_ZS_Collector::MyWrapper::myMkExistingItem(GUIParameterTableWindow &ret,
                                                 const std::string &name,
                                                 E2::DetType type)
{
    if(!myDetector.hasDetector(type)) {
        return;
    }
    MyValueRetriever *binding =
        new MyValueRetriever(myDetector, type, 1);
    ret.mkItem(name.c_str(), true, binding);
}


const std::string &
GUI_E2_ZS_Collector::MyWrapper::microsimID() const
{
    return myDetector.getID();
}


bool
GUI_E2_ZS_Collector::MyWrapper::active() const
{
    return true;
}


void
GUI_E2_ZS_Collector::MyWrapper::drawGL_SG(SUMOReal scale, SUMOReal upscale)
{
    SUMOReal myWidth = 1;
    if(myDetector.getUsageType()==DU_TL_CONTROL) {
        myWidth = (SUMOReal) 0.3;
        glColor3d(0, (SUMOReal) .6, (SUMOReal) .8);
    } else {
        glColor3d(0, (SUMOReal) .8, (SUMOReal) .8);
    }
    SUMOReal width=2; // !!!
    if(width*upscale>1.0) {
        glPushMatrix();
        glScaled(upscale, upscale, upscale);
        glTranslated(mySGPosition.x(), mySGPosition.y(), 0);
        glRotated( mySGRotation, 0, 0, 1 );
        glBegin( GL_QUADS );
        glVertex2d(-myWidth, 0);
        glVertex2d(-myWidth, -mySGLength);
        glVertex2d(myWidth, -mySGLength);
        glVertex2d(myWidth, 0);
        glEnd();
        glBegin( GL_LINES);
        glVertex2d(0, 0);
        glVertex2d(0, -mySGLength);
        glEnd();
        glPopMatrix();
    } else {
        glPushMatrix();
        glTranslated(mySGPosition.x(), mySGPosition.y(), 0);
        glRotated( mySGRotation, 0, 0, 1 );
        glBegin( GL_LINES);
        glVertex2d(0, 0);
        glVertex2d(0, -mySGLength);
        glEnd();
        glPopMatrix();
    }
}


void
GUI_E2_ZS_Collector::MyWrapper::drawGL_FG(SUMOReal scale, SUMOReal upscale)
{
    SUMOReal myWidth = 1;
    if(myDetector.getUsageType()==DU_TL_CONTROL) {
        myWidth = (SUMOReal) 0.3;
        glColor3d(0, (SUMOReal) .6, (SUMOReal) .8);
    } else {
        glColor3d(0, (SUMOReal) .8, (SUMOReal) .8);
    }
    SUMOReal width=2; // !!!
    if(width*upscale>1.0) {
        glScaled(upscale, upscale, upscale);
        GLHelper::drawBoxLines(myFullGeometry, myShapeRotations, myShapeLengths, myWidth);
    } else {
        for(size_t i=0; i<myFullGeometry.size()-1; i++) {
            GLHelper::drawLine(myFullGeometry[i],
                myShapeRotations[i], myShapeLengths[i]);
        }
    }
}


Position2D
GUI_E2_ZS_Collector::MyWrapper::getPosition() const
{
    return myBoundary.getCenter();
}


GUI_E2_ZS_Collector &
GUI_E2_ZS_Collector::MyWrapper::getDetector()
{
    return myDetector;
}


/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

// Local Variables:
// mode:C++
// End:

