/****************************************************************************/
/// @file    MSSOTLTrafficLightLogic.h
/// @author  Gianfilippo Slager
/// @date    Feb 2010
/// @version $Id: MSPhasedTrafficLightLogic.h 2 2010-03-03 15:00:00Z gslager $
///
// The base abstract class for SOTL logics
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// Copyright 2001-2009 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/
#ifndef MSSOTLTrafficLightLogic_h
#define MSSOTLTrafficLightLogic_h

// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

/*
#include <utility>
#include <vector>
#include <bitset>
#include <microsim/MSEventControl.h>
#include <microsim/MSNet.h>
#include "MSPhaseDefinition.h"
*/

#include <utils/common/SUMOTime.h>
#include "MSSOTLDefinitions.h"
#include "MSPhasedTrafficLightLogic.h"
#include "MSSOTLE2Sensors.h"


// ===========================================================================
// class definitions
// ===========================================================================

/**
 * @class MSSOTLTrafficLightLogic
 * @brief A fixed traffic light logic
 *
 * The base class for SOTL traffic light which switches between
 * it's phases and sets the lights to red in between.
 * Some functions are called with an information about the current step. This
 * is needed as a single logic may be used by many junctions and so the current
 * step is stored within them, not within methods.
 * Basically a SOTL logic overrides the natural flow of steps in order and length
 * according to sensors on input (and output) lanes
 */
class MSSOTLTrafficLightLogic :	public MSPhasedTrafficLightLogic {
public:
	typedef unsigned int CTS;

	//The map to store the cars*timesteps for each target phase
	typedef std::pair<size_t, CTS> TargetPhase_CarsTimesteps;
	typedef std::map<size_t, CTS> TargetPhase_CarsTimestepsMap;

	//The map to store the time each target phase have been checked last
	//This helps to compute the timesteps to get the cars*timesteps value
	typedef std::pair<size_t, SUMOTime> TargetPhase_LastCheck;
	typedef std::map<size_t, SUMOTime> TargetPhase_LastCheckMap;

	/** 
	 * @brief Constructor without sensors passed
     * @param[in] tlcontrol The tls control responsible for this tls
     * @param[in] id This tls' id
     * @param[in] subid This tls' sub-id (program id)
     * @param[in] phases Definitions of the phases
     * @param[in] step The initial phase index
     * @param[in] delay The time to wait before the first switch
     */
	MSSOTLTrafficLightLogic(MSTLLogicControl &tlcontrol, const std::string &id, const std::string &subid, const Phases &phases, unsigned int step, SUMOTime delay) throw();

	/** 
	 * @brief Constructor with sensors passed
     * @param[in] tlcontrol The tls control responsible for this tls
     * @param[in] id This tls' id
     * @param[in] subid This tls' sub-id (program id)
     * @param[in] phases Definitions of the phases
     * @param[in] step The initial phase index
     * @param[in] delay The time to wait before the first switch
	 * @param[in] sensors The already defined sensor logic
     */
	MSSOTLTrafficLightLogic(MSTLLogicControl &tlcontrol, const std::string &id, const std::string &subid, const Phases &phases, unsigned int step, SUMOTime delay, MSSOTLSensors *sensors) throw();

	/// @brief Destructor
	~MSSOTLTrafficLightLogic() throw();

	/** 
	 * @brief Initialises the tls with sensors on incoming lanes
	 * Sensors are built in the simulation according to the type of sensor specified in the simulation parameter
     * @param[in] nb The detector builder
     * @param[in] edgeContinuations Information about edge predecessors/successors
     * @exception ProcessError If something fails on initialisation
     */
    void init(NLDetectorBuilder &nb) throw(ProcessError);

	/*
	 * This member implements the base operations for all SOTL logics.
	 * SOTL politics are implementated through decidePhase() member
	 * @see MSTrafficLightLogic::trySwitch
     */
    SUMOTime trySwitch(bool) throw();

protected:
	/*
	 * This member has to contain the switching logic for SOTL policies
	 */
	virtual SUMOTime decideNextPhase() throw() = 0;

	/*
	* Count the number of vehicles approaching the target lanes for the given phase.
	* If the phase in not a target phase the function member will return 0.
	* @param[in] The target phase index
	*/
	unsigned int countVehiclesForTargetPhase(size_t phaseNumber) throw();
	
	/*
	* Every target step except the one from the current chain is checked.
	* This is because the current chain is not eligible to be directly 
	* targeted again, it would be unfair.
	* @return True if at least a target phase has passed the threshold for input cars-timesteps
	*/
	bool isThresholdPassed() throw();

	/*
	* Every target step except the one from the current chain is checked.
	* This is because the current chain is not eligible to be directly 
	* targeted again, it would be unfair.
	* @return The index of the phase with the maximum value of cars-timesteps
	*/
	size_t getPhaseIndexWithMaxCTS() throw();

	/*
	 * @param[in] phaseStep The step number for the target phase
	 * @return The current cars*timesteps of the given target phase. 0 if not a target phase
	*/
	CTS getCTSForTargetPhase(size_t phaseStep) throw();

	size_t getLastTargetPhase() throw() {
		return lastTargetPhase;
	}

	MSSOTLSensors* getSensors() {
		return mySensors;
	}

	/*
	* Computes how many time will pass after decideNextPhase will be executed again
	*/
	SUMOTime computeReturnTime() {
		if (myPhases[myStep]->isTransient()) {
			return myPhases[myStep]->duration;
        }
        return DELTA_T;
	}

private:
	/*
	* Pointer to the lensor logic regarding the junction controlled by this SOTLTrafficLightLogic
	*/
	MSSOTLSensors *mySensors;

	/*
	* When true means the class has responsibilities to intantiate and delete the SOTLSensors instance,
	* otherwise MSSOTLTrafficLightLogic::init and MSSOTLTrafficLightLogic::~MSSOTLTrafficLightLogic have not to affect SOTLSensors instance lifecycle
	*/
	bool sensorsSelfBuilt;

	TargetPhase_CarsTimestepsMap targetPhasesCTS;

	TargetPhase_LastCheckMap lastCheckForTargetPhase;

	/*
	* This member keeps track which is the current steps chain, i.e.
	* which is the last target phase executed (even if it is currently executed)
	* (a steps chain starts always with a target phase)
	*/
	size_t lastTargetPhase;

	/*
	* @brief Check for phases compliancy
	*/
	void checkPhases() throw();

	/*
	* Find the first target phase and set the current step on it
	*/
	void setToATargetPhase() throw();

	/*
	* This function member helps to set up the map keeping track of target phases and associated timesteps
	*/
	void setupCTS() throw();

	/*
	* Updates the cars-timesteps counters for each target phase except the one belonging to the current steps chain
	*/
	void updateCTS() throw();

	/*
	* To reset the cars-timesteps counter when a target phase is newly selected
	* If phaseStep is not a target phase nothing happens
	*/
	void resetCTSForTargetPhase(size_t phaseStep) throw();
};

#endif
/****************************************************************************/