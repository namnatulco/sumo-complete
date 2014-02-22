/****************************************************************************/
/// @file    PHEMCEPHandler.h
/// @author  Nikolaus Furian
/// @date    Thu, 13.06.2013
/// @version $$
///
// Helper singelton class for PHEM Light, holds CEP data for emission computation
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// Copyright (C) 2001-2014 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This file is part of SUMO.
//   SUMO is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/
#ifndef PHEMCEPHandler_h
#define PHEMCEPHandler_h

// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <vector>
#include <utils/common/StringBijection.h>
#include "PHEMCEP.h"


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class PHEMCEPHandler
 * @brief Data Handler for all CEP emission and vehicle Data
 */
class PHEMCEPHandler
{
public:
    /// @brief Destructor
	~PHEMCEPHandler();


	/** @brief Implementatio of Singelton pattern
     * @return reference on the actual instance
     */
	static PHEMCEPHandler& getHandlerInstance();


	 /** @brief Returns the CEP data for a PHEM emission class
     * @param[in] emissionClass desired PHEM emission class
     * @return CEP Data
     */
	PHEMCEP* GetCep(SUMOEmissionClass emissionClass);


private:
	 /** @brief Helper method to load CEP and vehicle files from file system
     * @param[in] emissionClass desired PHEM emission class
     * @return Indicator if loading was successul
     */
	bool Load(SUMOEmissionClass emissionClass);


	 /** @brief Helper method to read a vehicle file from file system
     * @param[in] path The path to PHEMlight data files
     * @param[in] emissionClass desired PHEM emission class
	 * @param[in] vehicleMass out variable for vehicle mass
	 * @param[in] vehivleLoading out variable for vehicle loading
	 * @param[in] crossArea out variable for crosssectional area of vehicle
	 * @param[in] cwValue dout variable for cd value of vehivle
	 * @param[in] f0 out variable for rolling resistance coefficient f0
	 * @param[in] f1 out variable for rolling resistance coefficient f1
	 * @param[in] f2 out variable for rolling resistance coefficient f2
	 * @param[in] f3 out variable for rolling resistance coefficient f3
	 * @param[in] f4 out variable for rolling resistance coefficient f4
	 * @param[in] ratedPower out variable for rated power of vehicle
	 * @param[in] vehicleMassType out variable for mass tyepe of vehicle, light (LV) or heavy (HV)
	 * @param[in] vehicleFuelType out variable for fuel type (D, G) of vehicle, needed for density of fuel
	 * @param[in] pNormV0 out variable for step function to get maximum normalized rated power over speed
	 * @param[in] pNormP0 out variable for step function to get maximum normalized rated power over speed
	 * @param[in] pNormV1 out variable for step function to get maximum normalized rated power over speed
	 * @param[in] pNormP1 out variable for step function to get maximum normalized rated power over speed
	 * @param[in] matrixRotFactor out variable for rotational factors over speed for more accurate power calculation
     * @return Indicator if reading was successul
     */
	bool ReadVehicleFile(const std::string &path, const std::string &emissionClass,
        double &vehicleMass, double &vehicleLoading, double &vehicleMassRot,
        double &crossArea, double &cWValue,
        double &f0, double &f1, double &f2, double &f3, double &f4, double &ratedPower, std::string &vehicleMassType, std::string &vehicleFuelType,
		double &pNormV0, double &pNormP0, double &pNormV1, double &pNormP1, std::vector< std::vector<double> > &matrixRotFactor);


	/** @brief Helper method to read a CEP file from file system
     * @param[in] path The path to PHEMlight data files
     * @param[in] emissionClass desired PHEM emission class
	 * @param[in] header vector of pollutant identifiers
	 * @param[in] matrix matrix holding power pattern and CEP curves
     * @return Indicator if reading was successul
     */
	bool ReadEmissionData(bool readFC, const std::string &path, const std::string &emissionClass, 
        std::vector<std::string> &header, std::vector<std::vector<double> > &matrix);

	
private:
	/// @brief bijection between PHEMEmissionClass and CEPs
	std::map<SUMOEmissionClass,PHEMCEP*> _ceps;


private:
	/** @brief Implementation of Singelton pattern
     *  private (copy) constructor and =operator to avoid more than one instances
     */
	PHEMCEPHandler();
	PHEMCEPHandler(PHEMCEPHandler const&);              
	void operator=(PHEMCEPHandler const&);

};

#endif
