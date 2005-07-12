#ifndef GUILaneStateReporter_h
#define GUILaneStateReporter_h
//---------------------------------------------------------------------------//
//                        GUILaneStateReporter.h -
//
//                           -------------------
//  project              : SUMO - Simulation of Urban MObility
//  begin                : Tue, 29.04.2005
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
//

/* =========================================================================
 * compiler pragmas
 * ======================================================================= */
#pragma warning(disable: 4786)

/* =========================================================================
 * included modules
 * ======================================================================= */
#include <microsim/output/e2_detectors/MSE2Collector.h>
#include <microsim/logging/LoggedValue_TimeFloating.h>
#include <microsim/MSUpdateEachTimestep.h>

/* =========================================================================
 * class definitions
 * ======================================================================= */
class GUILaneStateReporter : public MSE2Collector,
    public MSUpdateEachTimestep<GUILaneStateReporter>
{
public:
	/// Constructor
    GUILaneStateReporter(LoggedValue_TimeFloating<double> *densityRetriever,
        LoggedValue_TimeFloating<double> *speedRetriever,
        LoggedValue_TimeFloating<double> *haltingDurRetriever,
        float &floatingDensity, float &floatingSpeed, float &floatingHaltings,
        const std::string &id, MSLane *lane, SUMOTime interval);
	/// Destructor
    ~GUILaneStateReporter();
    bool updateEachTimestep( void );
protected:
    LoggedValue_TimeFloating<double> *myDensityRetriever;
    LoggedValue_TimeFloating<double> *mySpeedRetriever;
    LoggedValue_TimeFloating<double> *myHaltingDurRetriever;
    float &myFloatingDensity;
    float &myFloatingSpeed;
    float &myFloatingHaltings;

};

/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

#endif

// Local Variables:
// mode:C++
// End:

