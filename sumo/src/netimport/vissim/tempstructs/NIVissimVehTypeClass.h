#ifndef NIVissimVehTypeClass_h
#define NIVissimVehTypeClass_h
//---------------------------------------------------------------------------//
//                        NIVissimVehTypeClass.h -  ccc
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
// $Log$
// Revision 1.8  2005/10/07 11:40:10  dkrajzew
// THIRD LARGE CODE RECHECK: patched problems on Linux/Windows configs
//
// Revision 1.7  2005/09/23 06:02:57  dkrajzew
// SECOND LARGE CODE RECHECK: converted doubles and floats to SUMOReal
//
// Revision 1.6  2005/04/27 12:24:37  dkrajzew
// level3 warnings removed; made netbuild-containers non-static
//
// Revision 1.5  2003/10/27 10:51:55  dkrajzew
// edges speed setting implemented (only on an edges begin)
//
// Revision 1.4  2003/06/05 11:46:57  dkrajzew
// class templates applied; documentation added
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


#include <string>
#include <map>
#include <utils/gfx/RGBColor.h>
#include <utils/common/IntVector.h>

/* =========================================================================
 * class definitions
 * ======================================================================= */
/**
 *
 */
class NIVissimVehTypeClass {
public:
    NIVissimVehTypeClass(int id, const std::string &name,
        const RGBColor &color, IntVector &types);
    ~NIVissimVehTypeClass();
    static bool dictionary(int id, const std::string &name,
        const RGBColor &color, IntVector &types);
    static bool dictionary(int id, NIVissimVehTypeClass *o);
    static NIVissimVehTypeClass *dictionary(int name);
    static void clearDict();
    SUMOReal meanSpeed() const;
private:
    int myID;
    std::string myName;
    RGBColor myColor;
    IntVector myTypes;
private:
    typedef std::map<int, NIVissimVehTypeClass*> DictType;
    static DictType myDict;
};



/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

#endif

// Local Variables:
// mode:C++
// End:

