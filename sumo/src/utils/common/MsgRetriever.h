#ifndef MsgRetriever_h
#define MsgRetriever_h
/***************************************************************************
                          MsgRetriever.h  -
    Retrieves messages about the process
                             -------------------
    project              : SUMO
    begin                : Tue, 17 Jun 2003
    copyright            : (C) 2001 by DLR/IVF http://ivf.dlr.de/
    author               : Daniel Krajzewicz
    email                : Daniel.Krajzewicz@dlr.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// $Log$
// Revision 1.6  2005/10/07 11:43:30  dkrajzew
// THIRD LARGE CODE RECHECK: patched problems on Linux/Windows configs
//
// Revision 1.5  2005/09/15 12:13:08  dkrajzew
// LARGE CODE RECHECK
//
// Revision 1.4  2005/04/28 09:02:46  dkrajzew
// level3 warnings removed
//
// Revision 1.3  2004/11/23 10:27:45  dkrajzew
// debugging
//
// Revision 1.2  2003/12/09 11:32:14  dkrajzew
// destructor must be visble for deletion of msg-retrival methods
//
// Revision 1.1  2003/06/18 11:22:56  dkrajzew
// new message and error processing: output to user may be a message, warning or an error now; it is reported to a Singleton (MsgHandler); this handler puts it further to output instances. changes: no verbose-parameter needed; messages are exported to singleton
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


/* =========================================================================
 * class definitions
 * ======================================================================= */
/**
 * MsgRetriever
 */
class MsgRetriever {
public:
    /// adds a new error to the list
    virtual void inform(const std::string &error) = 0;

    /// destructor
    virtual ~MsgRetriever() { }

protected:
    /// standard constructor
    MsgRetriever() { }

};


/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

#endif

// Local Variables:
// mode:C++
// End:

