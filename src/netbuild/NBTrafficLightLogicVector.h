#ifndef NBTrafficLightLogicVector_h
#define NBTrafficLightLogicVector_h
//---------------------------------------------------------------------------//
//                        NBTrafficLightLogicVector.h -
//  A vector of traffic lights logics
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
// Revision 1.4  2003/03/20 16:23:10  dkrajzew
// windows eol removed; multiple vehicle emission added
//
// Revision 1.3  2003/03/03 14:59:22  dkrajzew
// debugging; handling of imported traffic light definitions
//
// Revision 1.2  2003/02/07 10:43:44  dkrajzew
// updated
//
//


/* =========================================================================
 * included modules
 * ======================================================================= */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <vector>
#include <iostream>
#include "NBCont.h"

/* =========================================================================
 * class declarations
 * ======================================================================= */
class NBTrafficLightLogic;

/* =========================================================================
 * class definitions
 * ======================================================================= */
/**
 *
 */
class NBTrafficLightLogicVector {
private:
    typedef std::vector<NBTrafficLightLogic*> LogicVector;
    LogicVector _cont;
public:
    NBTrafficLightLogicVector(const EdgeVector &inLanes);
    ~NBTrafficLightLogicVector();
    void add(NBTrafficLightLogic *logic);
    void add(const NBTrafficLightLogicVector &cont);
    void writeXML(std::ostream &os) const;
    bool contains(NBTrafficLightLogic *logic) const;
    int size() const;
private:
    EdgeVector myInLanes;
};



/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/
//#ifndef DISABLE_INLINE
//#include "NBTrafficLightLogicVector.icc"
//#endif

#endif

// Local Variables:
// mode:C++
// End:

