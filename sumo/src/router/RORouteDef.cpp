//---------------------------------------------------------------------------//
//                        RORouteDef.cpp -
//  Basic class for route definitions (not the computed routes)
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
// Revision 1.7  2003/06/18 11:20:54  dkrajzew
// new message and error processing: output to user may be a message, warning or an error now; it is reported to a Singleton (MsgHandler); this handler puts it further to output instances. changes: no verbose-parameter needed; messages are exported to singleton
//
// Revision 1.6  2003/05/20 09:48:35  dkrajzew
// debugging
//
// Revision 1.5  2003/03/20 16:39:17  dkrajzew
// periodical car emission implemented; windows eol removed
//
// Revision 1.4  2003/03/17 14:25:28  dkrajzew
// windows eol removed
//
// Revision 1.3  2003/03/03 15:22:34  dkrajzew
// debugging
//
// Revision 1.2  2003/02/07 10:45:06  dkrajzew
// updated
//
//


/* =========================================================================
 * included modules
 * ======================================================================= */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <string>
#include <utils/convert/TplConvert.h>
#include <utils/convert/ToString.h>
#include <utils/common/Named.h>
#include <utils/common/MsgHandler.h>
#include <utils/options/OptionsCont.h>
#include "ROEdge.h"
#include "RORoute.h"
#include "RORouter.h"
#include "ReferencedItem.h"
#include "RORouteDef.h"

using namespace std;


RORouteDef::RORouteDef(const std::string &id)
    : ReferencedItem(), Named(id)
{
}

RORouteDef::~RORouteDef()
{
}


bool
RORouteDef::computeAndSave(OptionsCont &options,
                           RORouter &router, long begin,
                           std::ostream &res, std::ostream &altres,
                           bool isPeriodical)
{
    RORoute *current =
        buildCurrentRoute(router, begin,
            options.getBool("continue-on-unbuild"));
    if(current->size()<2) {
        cout << endl;
        MsgHandler::getWarningInstance()->inform(
            string("The route '") + _id
            + string("' is too short, propably ending at the starting edge."));
        MsgHandler::getErrorInstance()->inform("Skipping...");
        delete current;
        return false;
    }
    addAlternative(current, begin);
    xmlOutCurrent(res, isPeriodical);
//    current->xmlOut(res);
    xmlOutAlternatives(altres);
    return true;
}


void
RORouteDef::patchID()
{
    // patch the name
    size_t idx = _id.rfind('_');
    if(idx!=string::npos) {
        try {
            int no = TplConvert<char>::_2int(_id.substr(idx+1).c_str());
            _id = _id.substr(0, idx+1) + toString<int>(no+1);
        } catch (NumberFormatException) {
            _id = _id + "_0";
        }
    } else {
        _id = _id + "_0";
    }
}


/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/
//#ifdef DISABLE_INLINE
//#include "RORouteDef.icc"
//#endif

// Local Variables:
// mode:C++
// End:


