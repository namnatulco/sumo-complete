/****************************************************************************/
/// @file    GUIVehicle.h
/// @author  Daniel Krajzewicz
/// @date    Sept 2002
/// @version $Id: $
///
// A MSVehicle extended by some values for usage within the gui
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
#ifndef GUIVehicle_h
#define GUIVehicle_h
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

#include <vector>
#include <set>
#include <string>
#include <utils/gui/globjects/GUIGlObject.h>
#include <utils/gfx/RGBColor.h>
#include <microsim/MSVehicle.h>
#include "GUIVehicleType.h"
#include "GUIRoute.h"
#include <utils/gui/globjects/GUIGLObjectPopupMenu.h>


// ===========================================================================
// class declarations
// ===========================================================================
class GUISUMOAbstractView;
class GUIGLObjectPopupMenu;


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * A visualisable MSVehicle. Extended by the possibility to retrieve names
 * of all available vehicles (static) and the possibility to retrieve the
 * color of the vehicle which is available in different forms allowing an
 * easier recognition of done actions such as lane changing.
 */
class GUIVehicle :
            public MSVehicle,
            public GUIGlObject
{
public:
    /// destructor
    ~GUIVehicle();

    inline void setOwnDefinedColor() const
    {
        if (hasCORNDoubleValue(MSCORN::CORN_VEH_OWNCOL_RED)) {
            glColor3d(
                getCORNDoubleValue(MSCORN::CORN_VEH_OWNCOL_RED),
                getCORNDoubleValue(MSCORN::CORN_VEH_OWNCOL_GREEN),
                getCORNDoubleValue(MSCORN::CORN_VEH_OWNCOL_BLUE));
            return;
        }
        glColor3d(1,1,0);
    }

    inline void setOwnTypeColor() const
    {
        static_cast<const GUIVehicleType&>(getVehicleType()).setColor();
    }
    inline void setOwnRouteColor() const
    {
        static_cast<const GUIRoute&>(getRoute()).setColor();
    }

    //@{ From GUIGlObject
    /// returns the popup-menu for vehicles
    GUIGLObjectPopupMenu *getPopUpMenu(GUIMainWindow &app,
                                       GUISUMOAbstractView &parent);

    // Returns the parameter window
    GUIParameterTableWindow *getParameterWindow(
        GUIMainWindow &app, GUISUMOAbstractView &parent);

    /// Returns the type of the object as coded in GUIGlObjectType
    GUIGlObjectType getType() const;

    /// returns the id of the object as known to microsim
    const std::string &microsimID() const;

    /// Returns the information whether this object is still active
    bool active() const;

    /// Returns the boundary to which the object shall be centered
    Boundary getCenteringBoundary() const;
    //@}


    /** returns a random color based on the vehicle's name
        (should stay the same across simulations */
    const RGBColor &getRandomColor1() const;

    /** retunrs a random color
        (this second random color is fully randomly computed) */
    const RGBColor &getRandomColor2() const;

    /** returns a color that describes how long ago the vehicle has
        changed the lane (is white after a lane change and becomes darker
        with each timestep */
    SUMOReal getTimeSinceLastLaneChangeAsReal() const;

    /** @brief Returns the next "periodical" vehicle with the same route
        We have to duplicate the vehicle if a further has to be emitted with
        the same settings */
    virtual MSVehicle *getNextPeriodical() const;

    friend class GUIVehicleControl;

    void setRemoved();

    int getRepetitionNo() const;
    int getPeriod() const;
    size_t getLastLaneChangeOffset() const;
    size_t getDesiredDepart() const;

    /**
     * @class GUIVehiclePopupMenu
     * A popup-menu for vehicles. In comparison to the normal popup-menu, this one
     *  also allows to:
     * - show/hide the vehicle route
     */
class GUIVehiclePopupMenu : public GUIGLObjectPopupMenu
    {
        FXDECLARE(GUIVehiclePopupMenu)
    public:
        /// Constructor
        GUIVehiclePopupMenu(GUIMainWindow &app,
                            GUISUMOAbstractView &parent, GUIGlObject &o);

        /// Destructor
        ~GUIVehiclePopupMenu();

        /// Called if all routes of the vehicle shall be shown
        long onCmdShowAllRoutes(FXObject*,FXSelector,void*);

        /// Called if all routes of the vehicle shall be hidden
        long onCmdHideAllRoutes(FXObject*,FXSelector,void*);

        /// Called if the current route of the vehicle shall be shown
        long onCmdShowCurrentRoute(FXObject*,FXSelector,void*);

        /// Called if the current route of the vehicle shall be hidden
        long onCmdHideCurrentRoute(FXObject*,FXSelector,void*);

        /// Called if the vehicle shall be tracked
        long onCmdStartTrack(FXObject*,FXSelector,void*);

        /// Called if the current shall not be tracked any longer
        long onCmdStopTrack(FXObject*,FXSelector,void*);

    protected:
        GUIVehiclePopupMenu()
        { }

    };


protected:
    /// Use this constructor only.
    GUIVehicle(GUIGlObjectStorage &idStorage,
               std::string id, MSRoute* route, SUMOTime departTime,
               const MSVehicleType* type, int repNo, int repOffset);

private:
    /// random color #1 (build from name)
    RGBColor _randomColor1;

    /// random color #2 (completely random)
    RGBColor _randomColor2;

};


#endif

/****************************************************************************/

