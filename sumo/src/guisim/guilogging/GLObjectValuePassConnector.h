#ifndef GLObjectValuePassConnector_h
#define GLObjectValuePassConnector_h
//---------------------------------------------------------------------------//
//                        GLObjectValuePassConnector.h -
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
// Revision 1.3  2005/07/12 12:16:10  dkrajzew
// level 3 warnings removed; code style adapted
//
//
/* =========================================================================
 * compiler pragmas
 * ======================================================================= */
#pragma warning(disable: 4786)


/* =========================================================================
 * included modules
 * ======================================================================= */
#include <utils/gui/globjects/GUIGlObject.h>
#include <microsim/MSUpdateEachTimestep.h>
#include <helpers/ValueSource.h>
#include <helpers/ValueRetriever.h>

/* =========================================================================
 * class declarations
 * ======================================================================= */
class GUIGlObject;

/* =========================================================================
 * class definitions
 * ======================================================================= */
/**
 *
 */
template<typename T>
class GLObjectValuePassConnector :
    public MSUpdateEachTimestep<GLObjectValuePassConnector<T> >
{
public:
    GLObjectValuePassConnector(GUIGlObject &o,
        ValueSource<T> *source,
        ValueRetriever<T> *retriever)
        : myObject(o), mySource(source), myRetriever(retriever)
        { }
    virtual ~GLObjectValuePassConnector() { }
    virtual bool updateEachTimestep( void ) {
        if(!myObject.active()) {
            return false;
        }
        myRetriever->addValue(mySource->getValue());
        return true;
    }
private:
    GUIGlObject &myObject;
    ValueSource<T> *mySource;
    ValueRetriever<T> *myRetriever;
};
/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

#endif

// Local Variables:
// mode:C++
// End:
