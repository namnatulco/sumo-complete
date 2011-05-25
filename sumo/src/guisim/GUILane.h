/****************************************************************************/
/// @file    GUILane.h
/// @author  Daniel Krajzewicz
/// @date    Sept 2002
/// @version $Id$
///
// Representation of a lane in the micro simulation (gui-version)
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// Copyright (C) 2001-2011 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/
#ifndef GUILane_h
#define GUILane_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <fx.h>
#include <string>
#include <utility>
#include <microsim/MSLane.h>
#include <microsim/MSEdge.h>
#include <utils/geom/Position.h>
#include <utils/geom/PositionVector.h>
#include "GUILaneWrapper.h"


// ===========================================================================
// class declarations
// ===========================================================================
class MSVehicle;
class MSNet;
class MFXMutex;


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class GUILane
 * @brief Representation of a lane in the micro simulation (gui-version)
 *
 * An extended MSLane. A mechanism to avoid concurrent
 * visualisation and simulation what may cause problems when vehicles
 * disappear is implemented using a mutex.
 */
class GUILane : public MSLane {
public:
    /** @brief Constructor
     *
     * @param[in] id The lane's id
     * @param[in] maxSpeed The speed allowed on this lane
     * @param[in] length The lane's length
     * @param[in] edge The edge this lane belongs to
     * @param[in] numericalID The numerical id of the lane
     * @param[in] shape The shape of the lane
     * @param[in] width The width of the lane
     * @param[in] allowed Vehicle classes that explicitly may drive on this lane
     * @param[in] disallowed Vehicle classes that are explicitly forbidden on this lane
     * @see SUMOVehicleClass
     * @see MSLane
     */
    GUILane(const std::string &id, SUMOReal maxSpeed,
            SUMOReal length, MSEdge * const edge, unsigned int numericalID,
            const PositionVector &shape, SUMOReal width,
            const SUMOVehicleClasses &allowed,
            const SUMOVehicleClasses &disallowed) throw();


    /// @brief Destructor
    ~GUILane() throw();



    /// @name Access to vehicles
    /// @{

    /** @brief Returns the vehicles container; locks it for microsimulation
     *
     * Locks "myLock" preventing usage by microsimulation.
     *
     * Please note that it is necessary to release the vehicles container
     *  afterwards using "releaseVehicles".
     * @return The vehicles on this lane
     * @see MSLane::getVehiclesSecure
     */
    const VehCont &getVehiclesSecure() const throw();


    /** @brief Allows to use the container for microsimulation again
     *
     * Unlocks "myLock" preventing usage by microsimulation.
     * @see MSLane::releaseVehicles
     */
    void releaseVehicles() const throw();
    /// @}



    /// @name Vehicle movement (longitudinal)
    /// @{

    /** the same as in MSLane, but locks the access for the visualisation
        first; the access will be granted at the end of this method */
    bool moveCritical(SUMOTime t);

    /** the same as in MSLane, but locks the access for the visualisation
        first; the access will be granted at the end of this method */
    bool setCritical(SUMOTime t, std::vector<MSLane*> &into);

    /** the same as in MSLane, but locks the access for the visualisation
        first; the access will be granted at the end of this method */
    bool integrateNewVehicle(SUMOTime t);
    ///@}



    void detectCollisions(SUMOTime timestep);


    GUILaneWrapper *buildLaneWrapper(unsigned int index);
    MSVehicle *removeVehicle(MSVehicle *remVehicle);

protected:
    /// moves myTmpVehicles int myVehicles after a lane change procedure
    void swapAfterLaneChange(SUMOTime t);

    /** @brief Inserts the vehicle into this lane, and informs it about entering the network
     * 
     * Calls the vehicles enterLaneAtInsertion function,
     *  updates statistics and modifies the active state as needed
     * @param[in] veh The vehicle to be incorporated
     * @param[in] pos The position of the vehicle
     * @param[in] speed The speed of the vehicle
     * @param[in] at
     * @param[in] notification The cause of insertion (i.e. departure, teleport, parking) defaults to departure
     * @see MSLane::incorporateVehicle
     */
    virtual void incorporateVehicle(MSVehicle *veh, SUMOReal pos, SUMOReal speed,
                                    const MSLane::VehCont::iterator &at,
                                    MSMoveReminder::Notification notification = MSMoveReminder::NOTIFICATION_DEPARTED) throw(ProcessError);


private:
    /// The mutex used to avoid concurrent updates of the vehicle buffer
    mutable MFXMutex myLock;


};


#endif

/****************************************************************************/

