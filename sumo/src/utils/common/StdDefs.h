#ifndef StdDefs_h
#define StdDefs_h
//---------------------------------------------------------------------------//
//                        StdDefs.h -
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
// Revision 1.8  2006/01/09 13:31:04  dkrajzew
// using definitions of lane widths instead of hard-coded values
//
// Revision 1.7  2005/10/07 11:43:30  dkrajzew
// THIRD LARGE CODE RECHECK: patched problems on Linux/Windows configs
//
// Revision 1.6  2005/09/23 06:05:45  dkrajzew
// SECOND LARGE CODE RECHECK: converted doubles and floats to SUMOReal
//
// Revision 1.5  2005/09/15 12:13:08  dkrajzew
// LARGE CODE RECHECK
//
// Revision 1.4  2005/07/12 12:43:49  dkrajzew
// code style adapted
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


/* -------------------------------------------------------------------------
 * some constant defaults used by SUMO
 * ----------------------------------------------------------------------- */
const SUMOReal SUMO_const_laneWidth = (SUMOReal) 3.2;
const SUMOReal SUMO_const_halfLaneWidth = (SUMOReal) 1.6;
const SUMOReal SUMO_const_laneOffset = (SUMOReal) .1;
const SUMOReal SUMO_const_laneWidthAndOffset = (SUMOReal) 3.3;
const SUMOReal SUMO_const_halfLaneAndOffset = (SUMOReal) (3.2/2.+.1);


/* -------------------------------------------------------------------------
 * definitions of common used conversions
 * ----------------------------------------------------------------------- */
#define mBYs2kmBYh(x) ((SUMOReal) (x/3.6))


/* -------------------------------------------------------------------------
 * templates for mathematical functions missing in some c++-implementations
 * ----------------------------------------------------------------------- */
template<typename T>
inline T
MIN2(T a, T b)
{
    return a<b?a:b;
}

template<typename T>
inline T
MAX2(T a, T b)
{
    return a>b?a:b;
}


template<typename T>
inline T
MIN3(T a, T b, T c)
{
    return MIN2(c, a<b?a:b);
}


template<typename T>
inline T
MAX3(T a, T b, T c)
{
    return MAX2(c, a>b?a:b);
}


template<typename T>
inline T
MIN4(T a, T b, T c, T d)
{
    return MIN2(MIN2(a,b),MIN2(c,d));
}


template<typename T>
inline T
MAX4(T a, T b, T c, T d)
{
    return MAX2(MAX2(a,b),MAX2(c,d));
}


/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

#endif

// Local Variables:
// mode:C++
// End:

