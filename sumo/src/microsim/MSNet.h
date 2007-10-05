/****************************************************************************/
/// @file    MSNet.h
/// @author  Christian Roessel
/// @date    Mon, 12 Mar 2001
/// @version $Id$
///
// Holds all necessary objects for micro-simulation.
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
#ifndef MSNet_h
#define MSNet_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _SPEEDCHECK
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <ctime>
#endif

#include <typeinfo>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <iomanip>

#include "output/meandata/MSMeanData_Net_Cont.h"
#include "MSVehicleControl.h"
#include "MSPersonControl.h"
#include "MSEventControl.h"
#include <utils/geom/Boundary.h>
#include <utils/geom/Position2D.h>
#include <utils/common/SUMOTime.h>
#include "MSPhoneNet.h"


// ===========================================================================
// class declarations
// ===========================================================================
class MSEdge;
class MSEdgeControl;
class MSJunctionControl;
class MSEmitControl;
class MSRouteLoaderControl;
class Event;
class RGBColor;
class MSVehicle;
class MSRoute;
class MSVehicleType;
class MSLane;
class MSTLLogicControl;
class MSVehicleTransfer;
class OutputDevice;
class NLBuilder;
class MSTrigger;
class MSDetectorControl;
class MSTriggerControl;
class ShapeContainer;
class BinaryInputDevice;
class MSRouteLoader;
class MSBuildCells;


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class MSNet
 * The main simulation class. Holds the network and indirectly vehicles which
 * are stored within a MSEmitControl - emitter which itself is a part of
 * MSNet.
 */
class MSNet
{
public:
    /** Get a pointer to the unique instance of MSNet (singleton).
     * @return Pointer to the unique MSNet-instance.
     */
    static MSNet* getInstance(void);

    MSNet(SUMOTime startTimestep, MSVehicleControl *vc,
          SUMOReal tooSlowRTF, bool logExecTime, bool logStep);


    /// Destructor.
    virtual ~MSNet();

    /** @brief Simulates from timestep start to stop.
        start and stop in timesteps.
        In each timestep we emit Vehicles, move Vehicles,
        the Vehicles change Lanes.  The method returns true when the
        simulation could be finished without errors, otherwise
        false. */
    bool simulate(SUMOTime start, SUMOTime stop);

    void initialiseSimulation();

    void closeSimulation(SUMOTime start, SUMOTime stop);


    /// performs a single simulation step
    void simulationStep(SUMOTime start, SUMOTime step);

    /// compute the distance between equipped vehicle
    void computeCar2Car(void);

    /** Clears all dictionaries */
    static void clearAll();

    /// Returns the timestep-length in seconds.
    static SUMOReal deltaT()
    {
        return myDeltaT;
    }

    /** @brief Returns the current simulation time in seconds.
        Current means start-time plus runtime. */
    SUMOTime simSeconds()
    {
        return (SUMOTime)(myStep * myDeltaT);
    }


    /** @brief Returns the number of unique mean-data-dump-intervalls.
        In vehicles and lanes you will need one element more for the
        GUI-dump. */
    unsigned getNDumpIntervalls(void);

    long getSimStepDurationInMillis() const;

    /// route handler may add routes and vehicles
    friend class MSRouteHandler;

    /// ----------------- debug variables -------------
    /*
    #ifdef ABS_DEBUG
    static SUMOTime searchedtime;
    static std::string searched1, searched2, searchedJunction;
    #endif
    */
    /// ----------------- speedcheck variables -------------


    SUMOTime getCurrentTimeStep() const;

    static SUMOReal getSeconds(SUMOReal steps)
    {
        return steps * myDeltaT;
    }

    static SUMOTime getSteps(SUMOReal seconds)
    {
        return static_cast< SUMOTime >(
                   floor(seconds / myDeltaT));
    }

    static SUMOReal getMeterPerSecond(SUMOReal cellsPerTimestep)
    {
        return cellsPerTimestep / myDeltaT;
    }

    static SUMOReal getVehPerKm(SUMOReal vehPerCell)
    {
        return (SUMOReal)(vehPerCell * 1000.0);
    }

    static SUMOReal getVehPerHour(SUMOReal vehPerStep)
    {
        return (SUMOReal)(vehPerStep / myDeltaT * 3600.0);
    }

    MSVehicleControl &getVehicleControl() const;
    MSPersonControl &getPersonControl() const;

    void writeOutput();

    friend class MSTriggeredSource;

    bool haveAllVehiclesQuit();

    size_t getMeanDataSize() const;
    MSEdgeControl &getEdgeControl();
    MSDetectorControl &getDetectorControl();
    MSTriggerControl &getTriggerControl();
    MSTLLogicControl &getTLSControl();
    MSJunctionControl &getJunctionControl();
    void addMeanData(MSMeanData_Net *newMeanData);

    virtual void closeBuilding(MSEdgeControl *edges,
                               MSJunctionControl *junctions, MSRouteLoaderControl *routeLoaders,
                               MSTLLogicControl *tlc, //ShapeContainer *sc,
                               const MSMeanData_Net_Cont &meanData,
                               std::vector<int> stateDumpTimes, std::string stateDumpFiles);

    bool logSimulationDuration() const
    {
        return myLogExecutionTime;
    }

    //{
    /// to be called before a simulation step is done, this prints the current step number
    void preSimStepOutput() const
    {
        std::cout << "Step #" << myStep;
        if (!myLogExecutionTime) {
            std::cout << (char) 13;
        }
    }

    /// to be called after a simulation step is done, this prints some further statistics
    void postSimStepOutput() const
    {
        if (myLogExecutionTime) {
            if (mySimStepDuration!=0) {
                std::cout.setf(std::ios::fixed , std::ios::floatfield) ;    // use decimal format
                std::cout.setf(std::ios::showpoint) ;    // print decimal point
                std::cout << std::setprecision(OUTPUT_ACCURACY) ;
                std::cout << " (" << mySimStepDuration << "ms ~= "
                << (1000./ (SUMOReal) mySimStepDuration) << "*RT, ~"
                << ((SUMOReal) myVehicleControl->getRunningVehicleNo()/(SUMOReal) mySimStepDuration*1000.)
                << "UPS, vehicles"
                << " TOT " << myVehicleControl->getEmittedVehicleNo()
                << " ACT " << myVehicleControl->getRunningVehicleNo()
                << ")       "
                << (char) 13;
            } else {
                std::cout << " (0ms; no further information available)          \r";
            }
        }
    }
    //}

    void saveState(std::ostream &os, long what);
    void loadState(BinaryInputDevice &bis, long what);

    ShapeContainer &getShapeContainer() const
    {
        return *myShapeContainer;
    }

    virtual MSRouteLoader *buildRouteLoader(const std::string &file, int incDUABase, int incDUAStage);

    SUMOReal getTooSlowRTF() const;

    MSEventControl &getBeginOfTimestepEvents()
    {
        return myBeginOfTimestepEvents;
    }

    MSEventControl &getEndOfTimestepEvents()
    {
        return myEndOfTimestepEvents;
    }

    /////////////////////////////////////////////
    MSPhoneNet * getMSPhoneNet()
    {
        return myMSPhoneNet;
    } ;
    /////////////////////////////////////////////

protected:
    MSPhoneNet * myMSPhoneNet;
    /** initialises the MeanData-container */
    static void initMeanData(std::vector<int> dumpMeanDataIntervalls,
                             std::string baseNameDumpFiles);

    /// Unique instance of MSNet
    static MSNet* myInstance;

    /// Unique ID.
    std::string myID;

    /** Lane-changing is done by this object. */
    MSEdgeControl* myEdges;

    /// Sets the right-of-way rules and moves first cars.
    MSJunctionControl* myJunctions;

    /// Masks requests from cars having red
    MSTLLogicControl *myLogics;

    /// Emits cars into the lanes.
    MSEmitControl* myEmitter;

    /** route loader for dynamic loading of routes */
    MSRouteLoaderControl *myRouteLoaders;

    // cells Builder
    MSBuildCells *myCellsBuilder;

    /// Timestep [sec]
    static SUMOReal myDeltaT;

    /// Current time step.
    SUMOTime myStep;



    /** @brief List of intervals and filehandles.
        At the end of each intervall the mean data (flow, density, speed ...)
        of each lane is calculated and written to file. */
    MSMeanData_Net_Cont myMeanData;

    /** @brief An instance responsible for vehicle */
    MSVehicleControl *myVehicleControl;
    mutable MSPersonControl *myPersonControl;
    MSDetectorControl *myDetectorControl;
    MSTriggerControl *myTriggerControl;
    MSEventControl myBeginOfTimestepEvents;
    MSEventControl myEndOfTimestepEvents;
    ShapeContainer *myShapeContainer; // could be a direct member

    //{@ data needed for computing performance values
    /// Information whether the simulation duration shall be logged
    bool myLogExecutionTime;

    /// Information whether the number of the simulation step shall be logged
    bool myLogStepNumber;

    /// The last simulation step begin, end and duration
    long mySimStepBegin, mySimStepEnd, mySimStepDuration;

    /// The overall simulation duration
    long mySimDuration;

    /// The overall number of vehicle movements
    long myVehiclesMoved;
    //}

    std::vector<int> myStateDumpTimes;
    std::string myStateDumpFiles;

    SUMOReal myTooSlowRTF;

    std::vector<MSVehicle*> myConnected;
    std::vector<MSVehicle*> myClusterHeaders;
    std::vector<MSEdge*> myAllEdges;

private:
    /// Copy constructor.
    MSNet(const MSNet&);

    /// Assignment operator.
    MSNet& operator=(const MSNet&);

};


#endif

/****************************************************************************/

