#ifndef GUILoadThread_h
#define GUILoadThread_h
//---------------------------------------------------------------------------//
//                        GUILoadThread.h -
//  Class describing the thread that performs the loading of a simulation
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
// Revision 1.2  2003/02/07 10:34:14  dkrajzew
// files updated
//
//


/* =========================================================================
 * included modules
 * ======================================================================= */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <string>
#include <qthread.h>


/* =========================================================================
 * class declarations
 * ======================================================================= */
class GUIApplicationWindow;


/* =========================================================================
 * class definitions
 * ======================================================================= */
class GUILoadThread : public QThread
{
public:
    /// constructor
    GUILoadThread(GUIApplicationWindow *mw);

    /// destructor
    ~GUILoadThread();

    /// begins the loading of the given file
    void init(const std::string &file);

    /** starts the thread
	the thread ends after the net has been loaded */
    void run();

private:
    /// the parent window to inform about the loading
    GUIApplicationWindow *_parent;

    /// the path to load the simulation from
    std::string _file;
};


/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/
//#ifndef DISABLE_INLINE
//#include "GUILoadThread.icc"
//#endif

#endif

// Local Variables:
// mode:C++
// End:

