//---------------------------------------------------------------------------//
//                        GUINetWrapper.cpp -
//  No geometrical information is hold, here. Still, some methods for
//      displaying network-information are stored in here
//                           -------------------
//  project              : SUMO - Simulation of Urban MObility
//  begin                :
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
// Revision 1.16  2006/04/11 10:56:32  dkrajzew
// microsimID() now returns a const reference
//
// Revision 1.15  2006/03/28 06:12:54  dkrajzew
// unneeded string wrapping removed
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
// Revision 1.11  2005/05/04 08:02:54  dkrajzew
// level 3 warnings removed; a certain SUMOTime time description added
//
// Revision 1.10  2005/02/01 10:07:24  dkrajzew
// performance computation added
//
// Revision 1.9  2004/11/24 08:46:43  dkrajzew
// recent changes applied
//
// Revision 1.8  2004/07/02 08:54:11  dkrajzew
// some design issues
//
// Revision 1.7  2004/04/02 11:18:37  dkrajzew
// recenter view - icon added to the popup menu
//
// Revision 1.6  2004/03/19 12:57:55  dkrajzew
// porting to FOX
//
// Revision 1.5  2003/12/11 06:24:55  dkrajzew
// implemented MSVehicleControl as the instance responsible for vehicles
//
// Revision 1.4  2003/11/12 13:59:04  dkrajzew
// redesigned some classes by changing them to templates
//
// Revision 1.3  2003/11/11 08:11:05  dkrajzew
// logging (value passing) moved from utils to microsim
//
// Revision 1.2  2003/08/14 13:47:44  dkrajzew
// false usage of function-pointers patched; false inclusion of
//  .moc-files removed
//
// Revision 1.1  2003/07/30 08:54:14  dkrajzew
// the network is capable to display the networks state, now
//
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
#include <iostream> // !!!
#include <utility>
#include <guisim/GUINet.h>
#include <utils/gui/windows/GUISUMOAbstractView.h>
#include "GUINetWrapper.h"
#include <utils/gui/globjects/GUIGLObjectPopupMenu.h>
#include <utils/gui/div/GUIParameterTableWindow.h>
#include <utils/gui/windows/GUIAppEnum.h>
#include <utils/gui/images/GUIIconSubSys.h>
#include <microsim/MSVehicleControl.h>
#include <microsim/logging/CastingFunctionBinding.h>
#include <microsim/logging/FunctionBinding.h>
#include <microsim/logging/FuncBinding_IntParam.h>
#include <utils/options/OptionsSubSys.h>
#include <utils/options/OptionsCont.h>
#include <utils/foxtools/MFXMenuHeader.h>
#include <gui/GUIApplicationWindow.h>

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
GUINetWrapper::GUINetWrapper( GUIGlObjectStorage &idStorage, GUINet &net)
    : GUIGlObject(idStorage, "network"),
    myNet(net)
{
}


GUINetWrapper::~GUINetWrapper()
{
}


GUIGLObjectPopupMenu *
GUINetWrapper::getPopUpMenu(GUIMainWindow &app,
                                 GUISUMOAbstractView &parent)
{
    GUIGLObjectPopupMenu *ret = new GUIGLObjectPopupMenu(app, parent, *this);
    new MFXMenuHeader(ret, app.getBoldFont(), getFullName().c_str(), 0, 0, 0);
    new FXMenuSeparator(ret);
    new FXMenuCommand(ret, "Center",
        GUIIconSubSys::getIcon(ICON_RECENTERVIEW), ret, MID_CENTER);
    new FXMenuSeparator(ret);
    new FXMenuCommand(ret, "Show Parameter",
        GUIIconSubSys::getIcon(ICON_APP_TABLE), ret, MID_SHOWPARS);
    return ret;
}


GUIParameterTableWindow *
GUINetWrapper::getParameterWindow(GUIMainWindow &app,
                                       GUISUMOAbstractView &parent)
{
    GUIParameterTableWindow *ret =
        new GUIParameterTableWindow(app, *this, 13);
    // add items
    ret->mkItem("vehicles running [#]", true,
        new CastingFunctionBinding<MSVehicleControl, SUMOReal, size_t>(
            &(getNet().getVehicleControl()),
            &MSVehicleControl::getRunningVehicleNo));
    ret->mkItem("vehicles ended [#]", true,
        new CastingFunctionBinding<MSVehicleControl, SUMOReal, size_t>(
            &(getNet().getVehicleControl()),
            &MSVehicleControl::getEndedVehicleNo));
    ret->mkItem("vehicles emitted [#]", true,
        new CastingFunctionBinding<MSVehicleControl, SUMOReal, size_t>(
            &(getNet().getVehicleControl()),
            &MSVehicleControl::getEmittedVehicleNo));
    ret->mkItem("vehicles loaded [#]", true,
        new CastingFunctionBinding<MSVehicleControl, SUMOReal, size_t>(
            &(getNet().getVehicleControl()),
            &MSVehicleControl::getLoadedVehicleNo));
    ret->mkItem("vehicles waiting [#]", true,
        new CastingFunctionBinding<MSVehicleControl, SUMOReal, size_t>(
            &(getNet().getVehicleControl()),
            &MSVehicleControl::getWaitingVehicleNo));
    ret->mkItem("end time [s]", false,
        (SUMOReal) OptionsSubSys::getOptions().getInt("e"));
    ret->mkItem("begin time [s]", false,
        (SUMOReal) OptionsSubSys::getOptions().getInt("b"));
    ret->mkItem("time step [s]", true,
        new CastingFunctionBinding<GUINet, SUMOReal, int>(
            &(getNet()), &GUINet::getCurrentTimeStep));
    if(getNet().logSimulationDuration()) {
        ret->mkItem("step duration [ms]", true,
            new CastingFunctionBinding<GUINet, SUMOReal, int>(
                &(getNet()), &GUINet::getWholeDuration));
        ret->mkItem("simulation duration [ms]", true,
            new CastingFunctionBinding<GUINet, SUMOReal, int>(
                &(getNet()), &GUINet::getSimDuration));
        /*
        ret->mkItem("visualisation duration [ms]", true,
            new CastingFunctionBinding<GUINet, SUMOReal, int>(
                &(getNet()), &GUINet::getVisDuration));
        */
        ret->mkItem("idle duration [ms]", true,
            new CastingFunctionBinding<GUINet, SUMOReal, int>(
                &(getNet()), &GUINet::getIdleDuration));
        ret->mkItem("duration factor []", true,
            new FunctionBinding<GUINet, SUMOReal>(
                &(getNet()), &GUINet::getRTFactor));
        /*
        ret->mkItem("mean duration factor []", true,
            new FuncBinding_IntParam<GUINet, SUMOReal>(
                &(getNet()), &GUINet::getMeanRTFactor), 1);
                */
        ret->mkItem("ups [#]", true,
            new FunctionBinding<GUINet, SUMOReal>(
                &(getNet()), &GUINet::getUPS));
        ret->mkItem("mean ups [#]", true,
            new FunctionBinding<GUINet, SUMOReal>(
                &(getNet()), &GUINet::getMeanUPS));
    }
    // close building
    ret->closeBuilding();
    return ret;
}



GUIGlObjectType
GUINetWrapper::getType() const
{
    return GLO_NETWORK;
}


const std::string &
GUINetWrapper::microsimID() const
{
    return "";
}


Boundary
GUINetWrapper::getBoundary() const
{
    return myNet.getBoundary();
}


GUINet &
GUINetWrapper::getNet() const
{
    return myNet;
}

Boundary
GUINetWrapper::getCenteringBoundary() const
{
	return getBoundary();
}


/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

// Local Variables:
// mode:C++
// End:


