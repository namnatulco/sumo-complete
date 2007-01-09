//---------------------------------------------------------------------------//
//                        GUIViewTraffic.cpp -
//  A view on the simulation; this view is a microscopic one
//                           -------------------
//  project              : SUMO - Simulation of Urban MObility
//  begin                : Sept 2002
//  copyright            : (C) 2002 by Daniel Krajzewicz
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
// Revision 1.69  2007/01/09 11:12:00  dkrajzew
// the names of nodes, additional structures, vehicles, edges, pois may now be shown
//
// Revision 1.68  2007/01/08 12:11:18  dkrajzew
// visualization of poi and detector names added
//
// Revision 1.67  2006/12/21 13:23:53  dkrajzew
// added visualization of tls/junction link indices
//
// Revision 1.66  2006/12/18 08:25:24  dkrajzew
// consolidation of setting colors
//
// Revision 1.65  2006/12/12 12:10:35  dkrajzew
// removed simple/full geometry options; everything is now drawn using full geometry
//
// Revision 1.64  2006/11/28 12:10:40  dkrajzew
// got rid of FXEX-Mutex (now using the one supplied in FOX)
//
// Revision 1.63  2006/11/22 13:06:33  dkrajzew
// patching problems on choosing an object when using shapes within different layers
//
// Revision 1.62  2006/11/16 13:56:45  dkrajzew
// warnings removed
//
// Revision 1.61  2006/11/16 12:30:53  dkrajzew
// warnings removed
//
// Revision 1.60  2006/11/16 10:50:42  dkrajzew
// warnings removed
//
// Revision 1.59  2006/10/12 07:50:40  dkrajzew
// c2c visualisation added (unfinished)
//
// Revision 1.58  2006/08/01 05:43:46  dkrajzew
// cartesian and geocoordinates are shown; changed the APIs for this
//
// Revision 1.57  2006/07/06 06:26:44  dkrajzew
// added blinker visualisation and vehicle tracking (unfinished)
//
// Revision 1.56  2006/07/06 05:35:54  dkrajzew
// replaced exception throwing in unreachable places by something more friendly
//
// Revision 1.55  2006/06/13 13:12:55  dkrajzew
// made static comboboxes really static
//
// Revision 1.54  2006/05/15 05:45:45  dkrajzew
// removed not needed messages
//
// Revision 1.53  2006/04/18 08:08:21  dkrajzew
// added Danilot Tete-Boyoms poi-interaction
//
// Revision 1.52  2006/03/27 07:22:07  dkrajzew
// shape layers added; extracted drawing of lane geometries
//
// Revision 1.50  2006/03/09 10:57:00  dkrajzew
// beautifying
//
// Revision 1.49  2006/03/08 13:09:11  dkrajzew
// possibility to insert pois on the gui added (danilo tet-boyom)
//
// Revision 1.48  2006/02/23 11:25:19  dkrajzew
// adding pois on the gui implemented (Danilot Tete Boyom)
//
// Revision 1.47  2006/01/31 10:53:02  dkrajzew
// new visualization scheme for lanes added; work on the grid
//
// Revision 1.46  2006/01/19 15:13:51  dkrajzew
// reinsert route visualization
//
// Revision 1.45  2006/01/19 09:26:19  dkrajzew
// debugging
//
// Revision 1.44  2006/01/11 11:56:30  dkrajzew
// added a colorwheel icon for visualization changer
//
// Revision 1.43  2006/01/09 11:50:20  dkrajzew
// new visualization settings implemented
//
// Revision 1.42  2005/11/09 06:33:15  dkrajzew
// removed unneeded stuff
//
// Revision 1.41  2005/10/07 11:36:47  dkrajzew
// THIRD LARGE CODE RECHECK: patched problems on Linux/Windows configs
//
// Revision 1.40  2005/09/22 13:30:40  dkrajzew
// SECOND LARGE CODE RECHECK: converted doubles and floats to SUMOReal
//
// Revision 1.39  2005/09/15 11:05:28  dkrajzew
// LARGE CODE RECHECK
//
// Revision 1.38  2005/07/12 12:10:14  dkrajzew
// further visualisation options added
//
// Revision 1.37  2005/05/04 07:50:15  dkrajzew
// ported to fox1.4
//
// Revision 1.36  2004/12/15 09:20:17  dkrajzew
// made guisim independent of giant/netedit
//
// Revision 1.35  2004/12/13 15:34:47  dkrajzew
// window resize on controls show/hide implemented
//
// Revision 1.34  2004/12/12 17:23:58  agaubatz
// Editor Tool Widgets included
//
// Revision 1.33  2004/11/23 10:11:33  dkrajzew
// adapted the new class hierarchy
//
// Revision 1.32  2004/08/02 11:55:35  dkrajzew
// using coloring schemes stored in a container
//
// Revision 1.31  2004/07/02 08:31:35  dkrajzew
// detector drawer now also draw other additional items; removed some memory
//  leaks;
//  some further drawing options (mainly for the online-router added)
//
// Revision 1.30  2004/06/17 13:06:55  dkrajzew
// Polygon visualisation added
//
// Revision 1.29  2004/03/19 12:54:08  dkrajzew
// porting to FOX
//
// Revision 1.28  2004/02/16 13:56:27  dkrajzew
// renamed some buttons and toolitips
//
// Revision 1.27  2004/02/05 16:29:31  dkrajzew
// memory leaks removed
//
// Revision 1.26  2003/12/09 11:23:07  dkrajzew
// some memory leaks removed
//
// Revision 1.25  2003/11/12 14:07:46  dkrajzew
// clean up after recent changes
//
// Revision 1.24  2003/11/11 08:40:03  dkrajzew
// consequent position2D instead of two SUMOReals implemented
//
// Revision 1.23  2003/10/15 11:37:50  dkrajzew
// old row-drawer replaced by new ones; setting of name information seems to
//  be necessary
//
// Revision 1.22  2003/10/02 14:55:56  dkrajzew
// visualisation of E2-detectors implemented
//
// Revision 1.21  2003/09/23 14:25:13  dkrajzew
// possibility to visualise detectors using different geometry complexities
//  added
//
// Revision 1.20  2003/09/05 14:55:11  dkrajzew
// lighter drawer implementations
//
// Revision 1.19  2003/08/14 13:44:14  dkrajzew
// tls/row - drawer added
//
// Revision 1.18  2003/07/30 08:52:16  dkrajzew
// further work on visualisation of all geometrical objects
//
// Revision 1.17  2003/07/22 14:56:46  dkrajzew
// changes due to new detector handling
//
// Revision 1.16  2003/07/16 15:18:23  dkrajzew
// new interfaces for drawing classes; junction drawer interface added
//
// Revision 1.15  2003/06/06 10:33:47  dkrajzew
// changes due to moving the popup-menus into a subfolder
//
// Revision 1.14  2003/06/05 06:26:16  dkrajzew
// first tries to build under linux: warnings removed; Makefiles added
//
// Revision 1.13  2003/05/20 09:23:54  dkrajzew
// some statistics added; some debugging done
//
// Revision 1.12  2003/04/16 09:50:04  dkrajzew
// centering of the network debugged;
// additional parameter of maximum display size added
//
// Revision 1.11  2003/04/14 08:24:57  dkrajzew
// unneeded display switch and zooming option removed;
// new gl-object concept implemented; comments added
//
// Revision 1.10  2003/04/07 10:15:16  dkrajzew
// glut reinserted
//
// Revision 1.9  2003/04/04 15:13:20  roessel
// Commented out #include <glut.h>
// Added #include <qcursor.h>
//
// Revision 1.8  2003/04/04 08:37:51  dkrajzew
// view centering now applies net size; closing problems debugged;
// comments added; tootip button added
//
// Revision 1.7  2003/04/02 11:50:28  dkrajzew
// a working tool tip implemented
//
// Revision 1.6  2003/03/20 16:17:52  dkrajzew
// windows eol removed
//
// Revision 1.5  2003/03/12 16:55:19  dkrajzew
// centering of objects debugged
//
// Revision 1.3  2003/03/03 15:10:20  dkrajzew
// debugging
//
// Revision 1.2  2003/02/07 10:34:14  dkrajzew
// files updated
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

#include <iostream>
#include <utility>
#include <cmath>
#include <guisim/GUINet.h>
#include <guisim/GUIEdge.h>
#include <guisim/GUILane.h>
#include <guisim/GUIVehicle.h>
#include <microsim/MSEdge.h>
#include <microsim/MSLane.h>
#include <microsim/MSCORN.h>
#include <utils/gfx/RGBColor.h>
#include <utils/geom/Position2DVector.h>
#include <utils/shapes/Polygon2D.h>
#include "GUISUMOViewParent.h"
#include "GUIViewTraffic.h"
#include <utils/gui/windows/GUISUMOAbstractView.h>
#include <utils/gui/windows/GUIPerspectiveChanger.h>
#include <utils/gui/windows/GUIAppEnum.h>
#include <utils/gui/globjects/GUIGlObject_AbstractAdd.h>
#include <utils/foxtools/MFXCheckableButton.h>
#include <utils/gui/images/GUIIconSubSys.h>
#include <gui/GUIApplicationWindow.h>
#include <utils/gui/div/GUIExcp_VehicleIsInvisible.h>
#include <utils/glutils/polyfonts.h>
#include <utils/gui/windows/GUIDialog_ViewSettings.h>
#include <utils/gui/drawer/GUICompleteSchemeStorage.h>
#include <utils/gui/images/GUITexturesHelper.h>
#include <utils/foxtools/MFXImageHelper.h>
#include <utils/gui/globjects/GUIPointOfInterest.h>
#include <utils/shapes/ShapeContainer.h>
#include <utils/gui/globjects/GUIGlObjectGlobals.h>

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
 * static member definitions
 * ======================================================================= */
GUIColoringSchemesMap<GUIVehicle> GUIViewTraffic::myVehicleColoringSchemes;
GUIColoringSchemesMap<GUILaneWrapper> GUIViewTraffic::myLaneColoringSchemes;


/* =========================================================================
 * FOX callback mapping
 * ======================================================================= */
FXDEFMAP(GUIViewTraffic) GUIViewTrafficMap[]={
    FXMAPFUNC(SEL_COMMAND,  MID_COLOURSCHEMECHANGE,   GUIViewTraffic::onCmdChangeColorScheme),

    FXMAPFUNC(SEL_COMMAND,  MID_SHOWTOOLTIPS,   GUIViewTraffic::onCmdShowToolTips),
    FXMAPFUNC(SEL_COMMAND,  MID_SHOWGRID,       GUIViewTraffic::onCmdShowGrid),
};

FXIMPLEMENT(GUIViewTraffic,GUISUMOAbstractView,GUIViewTrafficMap,ARRAYNUMBER(GUIViewTrafficMap))



/* =========================================================================
 * member method definitions
 * ======================================================================= */
GUIViewTraffic::GUIViewTraffic(FXComposite *p,
                               GUIMainWindow &app,
                               GUISUMOViewParent *parent,
                               GUINet &net, FXGLVisual *glVis)
	: GUISUMOAbstractView(p, app, parent, net._grid, glVis),
    myVehicleDrawer(net.myEdgeWrapper), myLaneDrawer(net.myEdgeWrapper),
    myJunctionDrawer(net.myJunctionWrapper),
    myDetectorDrawer(GUIGlObject_AbstractAdd::getObjectList()),
    myROWDrawer(net.myEdgeWrapper),
    myTrackedID(-1),
    _edges2Show(0), _junctions2Show(0), _additional2Show(0), _pointToMove(0),_IdToMove(0),
	_leftButtonPressed(false), _net(&net)
{
    init(net);
}


GUIViewTraffic::GUIViewTraffic(FXComposite *p,
                               GUIMainWindow &app,
                               GUISUMOViewParent *parent,
                               GUINet &net, FXGLVisual *glVis,
                               FXGLCanvas *share)
    : GUISUMOAbstractView(p, app, parent, net._grid, glVis, share),
    myVehicleDrawer(net.myEdgeWrapper), myLaneDrawer(net.myEdgeWrapper),
    myJunctionDrawer(net.myJunctionWrapper),
    myDetectorDrawer(GUIGlObject_AbstractAdd::getObjectList()),
    myROWDrawer(net.myEdgeWrapper),
    myTrackedID(-1),
    _edges2Show(0), _junctions2Show(0), _additional2Show(0), _pointToMove(0),
	_net(&net)
{
    init(net);
}


void
GUIViewTraffic::init(GUINet &)
{
    // build the artifact-instances-to-draw - tables
    _edges2ShowSize = (MSEdge::dictSize()>>5) + 1;
    _edges2Show = new size_t[_edges2ShowSize];
    clearUsetable(_edges2Show, _edges2ShowSize);
    _junctions2ShowSize = (MSJunction::dictSize()>>5) + 1;
    _junctions2Show = new size_t[_junctions2ShowSize];
    clearUsetable(_junctions2Show, _junctions2ShowSize);
    _additional2ShowSize = (GUIGlObject_AbstractAdd::getObjectList().size()>>5) + 1;
    _additional2Show = new size_t[_additional2ShowSize];
    clearUsetable(_additional2Show, _additional2ShowSize);
    // initialise default scheme
    myVisualizationSettings = &gSchemeStorage.get(gSchemeStorage.getNames()[0]);
}


GUIViewTraffic::~GUIViewTraffic()
{
    delete[] _edges2Show;
    delete[] _junctions2Show;
    delete[] _additional2Show;
    delete myLocatorPopup;
}


void
GUIViewTraffic::create()
{
    FXGLCanvas::create();
    myLocatorPopup->create();
}


void
GUIViewTraffic::buildViewToolBars(GUIGlChildWindow &v)
{
    FXToolBar &toolbar = v.getToolBar(*this);
    new FXToolBarGrip(&toolbar,NULL,0,TOOLBARGRIP_SINGLE);
    // build coloring tools
    {
        FXComboBox *myColoringSchemes =
            new FXComboBox(&toolbar, 12, this, MID_COLOURSCHEMECHANGE, FRAME_SUNKEN|LAYOUT_LEFT|LAYOUT_TOP|COMBOBOX_STATIC);
        const std::vector<std::string> &names = gSchemeStorage.getNames();
        for(std::vector<std::string>::const_iterator i=names.begin(); i!=names.end(); ++i) {
            myColoringSchemes->appendItem((*i).c_str());
        }
        myColoringSchemes->setNumVisible(5);
    }

    new FXToolBarGrip(&toolbar,NULL,0,TOOLBARGRIP_SINGLE);

    // build the locator buttons
    myLocatorPopup = new FXPopup(&toolbar, POPUP_VERTICAL);
        // for junctions
        new FXButton(myLocatorPopup,
            "\tLocate Junction\tLocate a Junction within the Network.",
            GUIIconSubSys::getIcon(ICON_LOCATEJUNCTION), &v, MID_LOCATEJUNCTION,
            ICON_ABOVE_TEXT|FRAME_THICK|FRAME_RAISED);
            // for edges
        new FXButton(myLocatorPopup,
            "\tLocate Street\tLocate a Street within the Network.",
            GUIIconSubSys::getIcon(ICON_LOCATEEDGE), &v, MID_LOCATEEDGE,
            ICON_ABOVE_TEXT|FRAME_THICK|FRAME_RAISED);
            // for vehicles
        new FXButton(myLocatorPopup,
            "\tLocate Vehicle\tLocate a Vehicle within the Network.",
            GUIIconSubSys::getIcon(ICON_LOCATEVEHICLE), &v, MID_LOCATEVEHICLE,
            ICON_ABOVE_TEXT|FRAME_THICK|FRAME_RAISED);
            // for additional stuff
        new FXButton(myLocatorPopup,
            "\tLocate Additional\tLocate an additional Structure within the Network.",
            GUIIconSubSys::getIcon(ICON_LOCATEADD), &v, MID_LOCATEADD,
            ICON_ABOVE_TEXT|FRAME_THICK|FRAME_RAISED);
    new FXMenuButton(&toolbar,"\tLocate structures",
        GUIIconSubSys::getIcon(ICON_LOCATE), myLocatorPopup,
        MENUBUTTON_RIGHT|LAYOUT_TOP|BUTTON_TOOLBAR|FRAME_RAISED|FRAME_THICK);
    // add viewport button
    new FXButton(&toolbar,
        "\tEdit Viewport...\tOpens a menu which lets you edit the viewport.",
        GUIIconSubSys::getIcon(ICON_EDITVIEWPORT), this, MID_EDITVIEWPORT,
        ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
    new FXButton(&toolbar,
        "\tEdit Coloring Schemes...\tOpens a menu which lets you edit the coloring schemes.",
        GUIIconSubSys::getIcon(ICON_COLORWHEEL), this, MID_EDITVIEW,
        ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);

    new FXToolBarGrip(&toolbar,NULL,0,TOOLBARGRIP_SINGLE);

    // add toggle button for grid on/off
    new MFXCheckableButton(false,
        &toolbar,
        "\tToggles Net Grid\tToggle whether the Grid shall be visualised.",
        GUIIconSubSys::getIcon(ICON_SHOWGRID), this, MID_SHOWGRID,
        ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
    // add toggle button for tool-tips on/off
    new MFXCheckableButton(false,
        &toolbar,
        "\tToggles Tool Tips\tToggle whether Tool Tips shall be shown.",
        GUIIconSubSys::getIcon(ICON_SHOWTOOLTIPS), this, MID_SHOWTOOLTIPS,
        ICON_ABOVE_TEXT|BUTTON_TOOLBAR|FRAME_RAISED|LAYOUT_TOP|LAYOUT_LEFT);
}


long
GUIViewTraffic::onCmdChangeColorScheme(FXObject*,FXSelector ,void*data)
{
    char *dataC = (char*) data; // !!! unicode
    myVisualizationSettings = &gSchemeStorage.get(dataC);
    // lanes
    switch(myLaneColoringSchemes.getColorSetType(myVisualizationSettings->laneEdgeMode)) {
    case CST_SINGLE:
        myLaneColoringSchemes.getColorerInterface(myVisualizationSettings->laneEdgeMode)->resetColor(
            myVisualizationSettings->laneColorings[myVisualizationSettings->laneEdgeMode][0]);
        break;
    case CST_MINMAX:
        myLaneColoringSchemes.getColorerInterface(myVisualizationSettings->laneEdgeMode)->resetColor(
            myVisualizationSettings->laneColorings[myVisualizationSettings->laneEdgeMode][0],
            myVisualizationSettings->laneColorings[myVisualizationSettings->laneEdgeMode][1]);
        break;
    default:
        break;
    }
    // vehicles
    switch(myVehicleColoringSchemes.getColorSetType(myVisualizationSettings->vehicleMode)) {
    case CST_SINGLE:
        myVehicleColoringSchemes.getColorerInterface(myVisualizationSettings->vehicleMode)->resetColor(
            myVisualizationSettings->vehicleColorings[myVisualizationSettings->vehicleMode][0]);
        break;
    case CST_MINMAX:
        myVehicleColoringSchemes.getColorerInterface(myVisualizationSettings->vehicleMode)->resetColor(
            myVisualizationSettings->vehicleColorings[myVisualizationSettings->vehicleMode][0],
            myVisualizationSettings->vehicleColorings[myVisualizationSettings->vehicleMode][1]);
        break;
    case CST_MINMAX_OPT:
        myVehicleColoringSchemes.getColorerInterface(myVisualizationSettings->vehicleMode)->resetColor(
            myVisualizationSettings->vehicleColorings[myVisualizationSettings->vehicleMode][0],
            myVisualizationSettings->vehicleColorings[myVisualizationSettings->vehicleMode][1],
            myVisualizationSettings->vehicleColorings[myVisualizationSettings->vehicleMode][2]);
        break;
    default:
        break;
    }
    update();
    return 1;
}


void
GUIViewTraffic::doPaintGL(int mode, SUMOReal scale)
{
    if(myTrackedID>0) {
        GUIGlObject *o = gIDStorage.getObjectBlocking(myTrackedID);
        if(o!=0) {
            Boundary b;
            b.add(static_cast<GUIVehicle*>(o)->getPosition());
            b.grow(20);
            _changer->centerTo(myGrid->getBoundary(), b, false);
        }
    }
    // init view settings
    glRenderMode(mode);
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    // get the viewport settings
    const Boundary &nb = _net->getBoundary();
	{
        SUMOReal width = nb.getWidth();
        SUMOReal height = nb.getHeight();
        SUMOReal mzoom = _changer->getZoom();
        SUMOReal cy = _changer->getYPos();//cursorY;
        SUMOReal cx = _changer->getXPos();//cursorY;

        // compute the visible area in horizontal direction
        SUMOReal mratioX = 1;
        SUMOReal mratioY = 1;
        SUMOReal xs = ((SUMOReal) _widthInPixels / (SUMOReal) myApp->getMaxGLWidth())
            / (myGrid->getBoundary().getWidth() / myNetScale) * _ratio;
        SUMOReal ys = ((SUMOReal) _heightInPixels / (SUMOReal) myApp->getMaxGLHeight())
            / (myGrid->getBoundary().getHeight() / myNetScale);
        if(xs<ys) {
            mratioX = 1;
            mratioY = ys/xs;
        } else {
            mratioY = 1;
            mratioX = xs/ys;
        }
        SUMOReal sxmin = nb.getCenter().x()
            - mratioX * width * (SUMOReal) 100 / (mzoom) / (SUMOReal) 2. / (SUMOReal) .97;
        sxmin -= cx;
        SUMOReal sxmax = nb.getCenter().x()
            + mratioX * width * (SUMOReal) 100 / (mzoom) / (SUMOReal) 2. / (SUMOReal) .97;
        sxmax -= cx;

        // compute the visible area in vertical direction
        SUMOReal symin = nb.getCenter().y()
            - mratioY * height / mzoom * (SUMOReal) 100 / (SUMOReal) 2. / (SUMOReal) .97;
        symin -= cy;
        SUMOReal symax = nb.getCenter().y()
            + mratioY * height / mzoom * (SUMOReal) 100 / (SUMOReal) 2. / (SUMOReal) .97;
        symax -= cy;

        // reset the tables of things to show if the viewport has changed
        if(myViewportSettings.differ(sxmin, symin, sxmax, symax)) {
            clearUsetable(_edges2Show, _edges2ShowSize);
            clearUsetable(_junctions2Show, _junctions2ShowSize);
            _net->_grid.get(GLO_LANE|GLO_JUNCTION|GLO_DETECTOR,
			    sxmin, symin, sxmax, symax,
                _edges2Show, _junctions2Show, _additional2Show);
            myViewportSettings.set(sxmin, symin, sxmax, symax);
        }
	}

    // compute lane width
    SUMOReal width = m2p(3.0) * scale;
    // compute which drawer shall be used
    if(_useToolTips) {
        myVehicleDrawer.setGLID(true);
        myLaneDrawer.setGLID(true);
        myDetectorDrawer.setGLID(true);
        myROWDrawer.setGLID(true);
        myJunctionDrawer.setGLID(true);
    } else {
        myVehicleDrawer.setGLID(false);
        myLaneDrawer.setGLID(false);
        myDetectorDrawer.setGLID(true);
        myROWDrawer.setGLID(true);
        myJunctionDrawer.setGLID(true);
    }
    // draw
    {
        myDecalsLock.lock();
        for(std::vector<GUISUMOAbstractView::Decal>::iterator l=myDecals.begin(); l!=myDecals.end(); ++l) {
            GUISUMOAbstractView::Decal &d = *l;
            if(!d.initialised) {
                FXImage *i = MFXImageHelper::loadimage(getApp(), d.filename);
                if(i!=0) {
                    d.glID = GUITexturesHelper::add(i);
                    d.initialised = true;
                }
            }
            glPushMatrix();
            glTranslated(d.left, d.top, 0);
            glRotated(d.rot, 0, 0, 1);
            glColor3d(1,1,1);
            GUITexturesHelper::drawTexturedBox(d.glID, 0, 0, d.right-d.left, d.bottom-d.top);
            glPopMatrix();
        }
        myDecalsLock.unlock();
    }
    drawShapes(_net->getShapeContainer(), 0, width);

    myJunctionDrawer.drawGLJunctions(_junctions2Show, _junctions2ShowSize,
        width, _junctionColScheme, *myVisualizationSettings);
    myLaneDrawer.drawGLLanes(_edges2Show, _edges2ShowSize, width,
        *myLaneColoringSchemes.getColorer(myVisualizationSettings->laneEdgeMode),
        *myVisualizationSettings);
    myDetectorDrawer.drawGLDetectors(_additional2Show, _additional2ShowSize,
        width, *myVisualizationSettings);
    myROWDrawer.drawGLROWs(*_net, _edges2Show, _edges2ShowSize, width,
        *myVisualizationSettings);
    //
    for(std::vector<VehicleOps>::iterator i=myVehicleOps.begin(); i!=myVehicleOps.end(); ++i) {
        const VehicleOps &vo = *i;
        switch(vo.type) {
        case VO_SHOW_ROUTE:
            {
                if(vo.routeNo>=0) {
                    drawRoute(vo, vo.routeNo, 0.25);
                } else {
                    if(vo.vehicle->hasCORNDoubleValue(MSCORN::CORN_VEH_NUMBERROUTE)) {
                        int noReroutePlus1 =
                            (int) vo.vehicle->getCORNDoubleValue(MSCORN::CORN_VEH_NUMBERROUTE) + 1;
                        for(int i=noReroutePlus1-1; i>=0; i--) {
                            SUMOReal darken =
                                (SUMOReal) 0.4
                                / (SUMOReal) noReroutePlus1
                                * (SUMOReal) i;
                            drawRoute(vo, i, darken);
                        }
                    } else {
                        drawRoute(vo, 0, 0.25);
                    }
                }
            }
            break;
        default:
            break;
        }
    }
    // draw the Polygons
    drawShapes(_net->getShapeContainer(), 10, width);
    // draw vehicles only when they're visible
    if(scale*m2p(3)>myVisualizationSettings->minVehicleSize) {
        myVehicleDrawer.drawGLVehicles(_edges2Show, _edges2ShowSize, width,
            myVehicleColoringSchemes, *myVisualizationSettings);
    }
    glPopMatrix();
}


RGBColor
GUIViewTraffic::getEdgeColor(GUIEdge *edge) const
{
    switch(edge->getPurpose()) {
    case GUIEdge::EDGEFUNCTION_NORMAL:
        return RGBColor((SUMOReal) 0,   (SUMOReal) 0,   (SUMOReal) 0);
    case GUIEdge::EDGEFUNCTION_SOURCE:
        return RGBColor((SUMOReal) 0,   (SUMOReal) 0.2, (SUMOReal) 0);
    case GUIEdge::EDGEFUNCTION_SINK:
        return RGBColor((SUMOReal) 0.2, (SUMOReal) 0,   (SUMOReal) 0);
    case GUIEdge::EDGEFUNCTION_INTERNAL:
        return RGBColor((SUMOReal) 0,   (SUMOReal) 0,   (SUMOReal) 0.2);
    default:
        break;
    }
    // hmmmm - should not happen
    return RGBColor((SUMOReal) 1, (SUMOReal) 1, (SUMOReal) 0);
}


void
GUIViewTraffic::startTrack(int id)
{
    myTrackedID = id;
}


void
GUIViewTraffic::stopTrack()
{
    myTrackedID = -1;
}


int
GUIViewTraffic::getTrackedID() const
{
    return myTrackedID;
}


void
GUIViewTraffic::doInit()
{
}


void
GUIViewTraffic::drawRoute(const VehicleOps &vo, int routeNo, SUMOReal darken)
{
    if(_useToolTips) {
        glPushName(vo.vehicle->getGlID());
    }
    myVehicleColoringSchemes.getColorer(myVisualizationSettings->vehicleMode)->setGlColor(*(vo.vehicle));
    GLdouble colors[4];
    glGetDoublev(GL_CURRENT_COLOR, colors);
    colors[0] -= darken;
    if(colors[0]<0) colors[0] = 0;
    colors[1] -= darken;
    if(colors[1]<0) colors[1] = 0;
    colors[2] -= darken;
    if(colors[2]<0) colors[2] = 0;
    colors[3] -= darken;
    if(colors[3]<0) colors[3] = 0;
    glColor3dv(colors);
    draw(vo.vehicle->getRoute(routeNo));
    if(_useToolTips) {
        glPopName();
    }
}


void
GUIViewTraffic::centerTo(GUIGlObject *o)
{
	if(o->getType()!=GLO_VEHICLE) {
        GUISUMOAbstractView::centerTo(o);
    } else {
        try {
            Boundary b;
            b.add(static_cast<GUIVehicle*>(o)->getPosition());
            b.grow(20);
            _changer->centerTo(myGrid->getBoundary(), b);
            _changer->otherChange();
            update();
        } catch (GUIExcp_VehicleIsInvisible) {
        }
    }
}


void
GUIViewTraffic::draw(const MSRoute &r)
{
    MSRouteIterator i = r.begin();
    for(; i!=r.end(); ++i) {
        const MSEdge *e = *i;
        const GUIEdge *ge = static_cast<const GUIEdge*>(e);
        const GUILaneWrapper &lane = ge->getLaneGeometry((size_t) 0);
        GLHelper::drawBoxLines(lane.getShape(), lane.getShapeRotations(), lane.getShapeLengths(), 1.0);
    }
}


void
GUIViewTraffic::showRoute(GUIVehicle *v, int index)
{
    VehicleOps vo;
    vo.vehicle = v;
    vo.type = VO_SHOW_ROUTE;
    vo.routeNo = index;
    myVehicleOps.push_back(vo);
    update();
}


void
GUIViewTraffic::hideRoute(GUIVehicle *v, int index)
{
    std::vector<VehicleOps>::iterator i =
        find_if(myVehicleOps.begin(), myVehicleOps.end(), vehicle_in_ops_finder(v));
    while(i!=myVehicleOps.end()) {
        if((*i).type==VO_SHOW_ROUTE&&(*i).routeNo==index) {
            i = myVehicleOps.erase(i);
		    update();
            return;
        }
        i = find_if(i+1, myVehicleOps.end(), vehicle_in_ops_finder(v));
    }
    update();
}


bool
GUIViewTraffic::amShowingRouteFor(GUIVehicle *v, int index)
{
    std::vector<VehicleOps>::iterator i =
        find_if(myVehicleOps.begin(), myVehicleOps.end(), vehicle_in_ops_finder(v));
    while(i!=myVehicleOps.end()) {
        if((*i).type==VO_SHOW_ROUTE&&(*i).routeNo==index) {
            return true;
        }
        i = find_if(i+1, myVehicleOps.end(), vehicle_in_ops_finder(v));
    }
    return false;
}


long
GUIViewTraffic::onCmdEditView(FXObject*,FXSelector,void*)
{
    if(myVisualizationChanger==0) {
        myVisualizationChanger =
            new GUIDialog_ViewSettings(
                myApp, this, myVisualizationSettings,
                &myLaneColoringSchemes, &myVehicleColoringSchemes,
                &myDecals, &myDecalsLock);
        myVisualizationChanger->create();
    }
    myVisualizationChanger->show();
    return 1;
}


long
GUIViewTraffic::onLeftBtnPress(FXObject *o,FXSelector sel,void *data)
{
    FXEvent *e = (FXEvent*) data;
	_leftButtonPressed=true;
	if(e->state&CAPSLOCKMASK) {
		if(makeCurrent()) {
			unsigned int id = getObjectUnderCursor();
			if(id!=0){
				GUIGlObject *o = gIDStorage.getObjectBlocking(id);
				std::string n= o->getFullName();
				std::string name = n.substr(n.find(":")+1,n.length());
				GUIPointOfInterest *p= static_cast<GUIPointOfInterest *>(_net->getShapeContainer().getPOICont(1).get(name));
				setFirstPoint(p);
			}else{
				FXMessageBox::error(myApp,MBOX_OK,"Error Get Point","No Point at the Position","");
			}
			makeNonCurrent();
		}
	}

    if((e->state&SHIFTMASK)!=0) {
         // try to get the object-id if so
        if(makeCurrent()) {
            unsigned int id = getObjectUnderCursor();
            setIdToMove(id);
            makeNonCurrent();
            if(id!=0){
                GUIGlObject *o = gIDStorage.getObjectBlocking(id);
                std::string n= o->getFullName();
                std::string name = n.substr(n.find(":")+1,n.length());
                GUIPointOfInterest *p= static_cast<GUIPointOfInterest *>
                    (_net->getShapeContainer().getPOICont(1).get(name));
                setPointToMove(p);
            } else {
                Position2D point = getPositionInformation();
                std::string Id= toString(point.x()) +  "," + toString(point.y());
                GUIPointOfInterest *p = new GUIPointOfInterest(gIDStorage, 1, Id, "point",
                    Position2D(point.x(), point.y()),RGBColor(0,0,0) );
                _net->getShapeContainer().add(1, p);
                update();
            }
        }
        return 1;
    }
    return GUISUMOAbstractView::onLeftBtnPress(o, sel, data);
}


void
GUIViewTraffic::setPointToMove(PointOfInterest *p)
{
	_pointToMove = p;
}


void
GUIViewTraffic::setIdToMove(unsigned int id)
{
	_IdToMove = id;
}



long
GUIViewTraffic::onLeftBtnRelease(FXObject *o,FXSelector sel,void *data)
{
    long ret = GUISUMOAbstractView::onLeftBtnRelease(o, sel, data);
	FXEvent *e = (FXEvent*) data;
	if(_leftButtonPressed && _firstPoint!=0 && e->state&CAPSLOCKMASK){
		if(makeCurrent()) {
			unsigned int id = getObjectUnderCursor();
			if(id==-1){
				GUIGlObject *o = gIDStorage.getObjectBlocking(id);
				std::string n= o->getFullName();
				std::string name = n.substr(n.find(":")+1,n.length());
				GUIPointOfInterest *p= static_cast<GUIPointOfInterest *> (_net->getShapeContainer().getPOICont(1).get(name));
				setSecondPoint(p);
//!!!				_net->getShapeContainer().addPair(_firstPoint,_secondPoint);
				update();
			}else{
				Position2D point = getPositionInformation();
				std::string Id= toString(point.x())+toString(point.y());
				GUIPointOfInterest *p =
                    new GUIPointOfInterest(gIDStorage, 1, Id, "point", point, RGBColor(0,0,0) );
				_net->getShapeContainer().add(1, p);
				setSecondPoint(p);
//!!!				_net->getShapeContainer().addPair(_firstPoint,_secondPoint);
				SUMOReal dist = (SUMOReal) (sqrt(pow((_firstPoint->x() - _secondPoint->x()),2) + pow((_firstPoint->y() - _secondPoint->y()),2)));
				update();
			}
			_firstPoint=0;
			makeNonCurrent();
		}

	}else{
		delete _popup;
		_popup = 0;
		ungrab();
	}
	_leftButtonPressed=false;
    return ret;
}


long
GUIViewTraffic::onMouseMove(FXObject *o,FXSelector sel,void *data)
{
    /*
    SUMOReal xpos = _changer->getXPos();
    SUMOReal ypos = _changer->getYPos();
    SUMOReal zoom = _changer->getZoom();
    */
	FXEvent *e=(FXEvent*)data;

	if(_pointToMove!=0 && e->state&SHIFTMASK ){
		// Keep Color Informations
		Position2D point = getPositionInformation(e->win_x, e->win_y);
        _pointToMove->set(point.x(), point.y());
		if(!_leftButtonPressed){
			_pointToMove=0;
		}
        updatePositionInformation();
		update();
    }
	else{
		if(_firstPoint!=0 && e->state&CAPSLOCKMASK && _leftButtonPressed) {
			//do Nothing
            return 1;
		}
	}
    return GUISUMOAbstractView::onMouseMove(o, sel, data);
}

void
GUIViewTraffic::rename(GUIGlObject *o)
{
	std::string n= o->getFullName();
	std::string name = n.substr(n.find(":")+1,n.length());

	FXInputDialog* dialog = new FXInputDialog(myApp,"Please Enter the new Name", n.c_str());
	dialog->setText(name.c_str());
    if(dialog->execute()){
        /*
		FXString input = dialog->getText();
		if(input.length() > 0){
		   PointOfInterest* p =_net->getShapeContainer().getPOICont(1).get(name);
		   RGBColor col(p->red(),p->green(),p->blue());
		   GUIPointOfInterest *p_new = new GUIPointOfInterest(gIDStorage, input.text(), "point",
								  Position2D(p->x(), p->y()),col);
		   _net->getShapeContainer().add(1, p_new);

//!!!           const std::map<PointOfInterest*, PointOfInterest*> &pv = _net->getShapeContainer().getPOIPairCont();
		   std::map<PointOfInterest*, PointOfInterest*>::const_iterator pi = pv.begin();
		   for(; pi!=pv.end(); pi++) {
			   if((*pi).first->getID()==name){
                   (*pi).first->setID(toString(input.text()));
			   }
				if((*pi).second->getID()==name){
                   (*pi).second->setID(toString(input.text()));
			   }
		   }

		   bool b=_net->getShapeContainer().getPOICont().erase(p->getID());
		   //gIDStorage.remove(id); ?? muss ich den Point daraus l�schen??

		}
       */
	}
	delete dialog;
}

void
GUIViewTraffic::moveTo(GUIGlObject *)
{
    /*
	std::string n= o->getFullName();
	std::string name = n.substr(n.find(":")+1,n.length());

	FXDialogBox* dialog = new FXDialogBox(myApp,"Move to new Position",DECOR_TITLE|DECOR_BORDER|DECOR_RESIZE,0,0,300,100, 0,0,0,0, 4,4);

	// Bottom buttons
    FXHorizontalFrame* buttons=new FXHorizontalFrame(dialog,LAYOUT_SIDE_BOTTOM|FRAME_NONE|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH,0,0,0,0,40,40,20,20);

	// Separator
    new FXHorizontalSeparator(dialog,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|SEPARATOR_GROOVE);
	FXHorizontalFrame* contents=new FXHorizontalFrame(dialog,LAYOUT_SIDE_TOP|FRAME_NONE|LAYOUT_FILL_X|LAYOUT_FILL_Y|PACK_UNIFORM_WIDTH);

    new FXLabel(contents,"Enter XPos",NULL,LAYOUT_LEFT);
    FXTextField* textX =new FXTextField(contents,6,NULL,0,FRAME_SUNKEN|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN,0,0,0,0, 2,2,2,2);
    textX->setText((toString((_net->getShapeContainer().getPOICont().get(name))->x())).c_str());

    new FXLabel(contents,"Enter YPos",NULL,LAYOUT_LEFT);
    FXTextField* textY =new FXTextField(contents,6,NULL,0,FRAME_SUNKEN|FRAME_THICK|LAYOUT_CENTER_Y|LAYOUT_FILL_X|LAYOUT_FILL_COLUMN,0,0,0,0, 2,2,2,2);
    textY->setText((toString((_net->getShapeContainer().getPOICont().get(name))->y())).c_str());

	new FXButton(buttons,"&Accept",NULL,dialog,FXDialogBox::ID_ACCEPT,BUTTON_INITIAL|BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0, 20,20);
    new FXButton(buttons,"&Cancel",NULL,dialog,FXDialogBox::ID_CANCEL,BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED|FRAME_THICK,0,0,0,0, 20,20);

    if(dialog->execute()){
		FXString inputX = textX->getText();
		FXString inputY = textX->getText();
		if(inputX.length() > 0 && inputY.length() > 0){
			SUMOReal x = TplConvert<char>::_2SUMOReal(textX->getText().text());
			SUMOReal y = TplConvert<char>::_2SUMOReal(textY->getText().text());
		   _net->getShapeContainer().getPOICont().get(name)->set(x,y);
		}
	}
	delete dialog;
	*/
}


void
GUIViewTraffic::changeCol(GUIGlObject *)
{
    /*
	std::string n= o->getFullName();
	std::string name = n.substr(n.find(":")+1,n.length());

	FXColorDialog * dialog = new FXColorDialog(myApp,"Color");

	RGBColor col = RGBColor(_net->getShapeContainer().getPOICont().get(name)->myRed,
							_net->getShapeContainer().getPOICont().get(name)->myGreen,
							_net->getShapeContainer().getPOICont().get(name)->myBlue);
	dialog->setRGBA(FXRGB(col.red()*255., col.green()*255., col.blue()*255.));
    if(dialog->execute()){
		FXColor c = dialog->getRGBA();
		_net->getShapeContainer().getPOICont().get(name)->myRed=(SUMOReal) FXREDVAL(c) / (SUMOReal) 255.;
		_net->getShapeContainer().getPOICont().get(name)->myGreen=(SUMOReal) FXGREENVAL(c) / (SUMOReal) 255.;
		_net->getShapeContainer().getPOICont().get(name)->myBlue=(SUMOReal) FXBLUEVAL(c) / (SUMOReal) 255.;
	}
	delete dialog;
    */
}

void
GUIViewTraffic::changeTyp(GUIGlObject *)
{
    /*
	std::string n= o->getFullName();
	std::string name = n.substr(n.find(":")+1,n.length());

    FXDialogBox* dialog = new FXDialogBox(myApp,"Change Type... ",DECOR_TITLE|DECOR_BORDER);

	// Bottom buttons
    FXHorizontalFrame* buttons=new FXHorizontalFrame(dialog,LAYOUT_SIDE_BOTTOM|FRAME_NONE|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH,0,0,0,0,40,40,20,20);

	// Separator
    new FXHorizontalSeparator(dialog,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|SEPARATOR_GROOVE);

	FXHorizontalFrame* contents=new FXHorizontalFrame(dialog,LAYOUT_SIDE_TOP|FRAME_NONE|LAYOUT_FILL_X|LAYOUT_FILL_Y|PACK_UNIFORM_WIDTH);


	FXListBox *lstb= new FXListBox (contents, NULL, 0, FRAME_SUNKEN|FRAME_THICK|LAYOUT_FILL_X);
	lstb->insertItem (0,"Point");
	lstb->insertItem (1,"Vehicle");
    lstb->insertItem (2,"Lane");
	lstb->insertItem (3,"POI");
	lstb->insertItem (4,"Polygon");
	lstb->insertItem (5,"Building");

    // Accept
    new FXButton(buttons,"&Accept",NULL,dialog,FXDialogBox::ID_ACCEPT,BUTTON_DEFAULT|BUTTON_INITIAL|FRAME_RAISED|FRAME_THICK|LAYOUT_RIGHT|LAYOUT_CENTER_Y);
    // Cancel
    new FXButton(buttons,"&Cancel",NULL,dialog,FXDialogBox::ID_CANCEL,BUTTON_DEFAULT|FRAME_RAISED|FRAME_THICK|LAYOUT_RIGHT|LAYOUT_CENTER_Y);

	if(dialog->execute()){
		(_net->getShapeContainer().getPOICont().get(name))->setType((lstb->getItemText(lstb->getCurrentItem())).text());
	}

	delete buttons;
	delete lstb;
	delete contents;
	delete dialog;
    */
}

void
GUIViewTraffic::deleteObj(GUIGlObject *)
{
    /*
	std::string n= o->getFullName();
	std::string name = n.substr(n.find(":")+1,n.length());

    FXuint answer=FXMessageBox::question(myApp,MBOX_YES_NO_CANCEL,"Delete the Point","Do you want to delete the Point?");
    if(answer==MBOX_CLICKED_YES){
		gIDStorage.remove(getObjectUnderCursor());
		bool b =_net->getShapeContainer().getPOICont().erase(name);
		if(!b){
			  FXMessageBox::error(myApp,MBOX_OK,"Error by Delete ","Cannot delete the Point at the Position","");
		}
	}
*/
}

void
GUIViewTraffic::setFirstPoint(PointOfInterest *p){
	_firstPoint=p;
}

void
GUIViewTraffic::setSecondPoint(PointOfInterest *p){
	_secondPoint=p;
}


GUIColoringSchemesMap<GUIVehicle> &
GUIViewTraffic::getVehiclesSchemesMap()
{
    return myVehicleColoringSchemes;
}


GUIColoringSchemesMap<GUILaneWrapper> &
GUIViewTraffic::getLaneSchemesMap()
{
    return myLaneColoringSchemes;
}


/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

// Local Variables:
// mode:C++
// End:


