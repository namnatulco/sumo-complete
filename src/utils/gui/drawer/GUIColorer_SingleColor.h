#ifndef GUIColorer_SingleColor_h
#define GUIColorer_SingleColor_h

//---------------------------------------------------------------------------//
//                        GUIColorer_SingleColor.h -
//
//                           -------------------
//  project              : SUMO - Simulation of Urban MObility
//  begin                : Fri, 29.04.2005
//  copyright            : (C) 2005 by Daniel Krajzewicz
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
// $Log$
// Revision 1.9  2006/12/18 08:25:24  dkrajzew
// consolidation of setting colors
//
// Revision 1.8  2006/11/16 12:30:54  dkrajzew
// warnings removed
//
// Revision 1.7  2006/01/09 11:50:21  dkrajzew
// new visualization settings implemented
//
// Revision 1.6  2005/10/07 11:45:09  dkrajzew
// THIRD LARGE CODE RECHECK: patched problems on Linux/Windows configs
//
// Revision 1.5  2005/09/23 06:07:54  dkrajzew
// SECOND LARGE CODE RECHECK: converted doubles and floats to SUMOReal
//
// Revision 1.4  2005/09/15 12:19:10  dkrajzew
// LARGE CODE RECHECK
//
// Revision 1.3  2005/07/12 12:49:08  dkrajzew
// code style adapted
//
// Revision 1.3  2005/06/14 11:29:50  dksumo
// documentation added
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

#include "GUIBaseColorer.h"
#include <utils/gfx/RGBColor.h>

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>


/* =========================================================================
 * class definitions
 * ======================================================================= */
/**
 * @class GUIColorer_SingleColor
 * @brief Uses a single color for coloring
 */
template<class _T>
class GUIColorer_SingleColor : public GUIBaseColorer<_T> {
public:
    /// Constructor
	GUIColorer_SingleColor(const RGBColor &c) : myColor(c) { }

    /// Destructor
	virtual ~GUIColorer_SingleColor() { }


    ///{ from GUIBaseColorer
    /// Sets the color using a value from the given instance of _T
	void setGlColor(const _T& ) const {
		glColor3d(myColor.red(), myColor.green(), myColor.blue());
	}

    /// Sets the color using the given value
	void setGlColor(SUMOReal ) const {
		glColor3d(myColor.red(), myColor.green(), myColor.blue());
	}

    /// Returns the type of this class (CST_SINGLE)
    virtual ColorSetType getSetType() const {
        return CST_SINGLE;
    }
    ///}


    ///{ from GUIBaseColorerInterface
    /// Sets the given color as the colors to use
    virtual void resetColor(const RGBColor &c) {
        myColor = c;
    }

    /// Returns the single color used
    virtual const RGBColor &getSingleColor() const {
        return myColor;
    }
    ///}

private:
    /// The color to use
	RGBColor myColor;

};


/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

#endif

// Local Variables:
// mode:C++
// End:
