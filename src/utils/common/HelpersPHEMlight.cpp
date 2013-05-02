/****************************************************************************/
/// @file    HelpersPHEMlight.cpp
/// @author  Daniel Krajzewicz
/// @date    Sat, 20.04.2013
/// @version $Id: HelpersPHEMlight.cpp 13107 2012-12-02 13:57:34Z behrisch $
///
// Helper methods for PHEMlight-based emission computation
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// Copyright (C) 2001-2012 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This file is part of SUMO.
//   SUMO is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include "HelpersPHEMlight.h"
#include <limits>
#include <cmath>

#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif // CHECK_MEMORY_LEAKS


// ===========================================================================
// method definitions
// ===========================================================================
SUMOReal
HelpersPHEMlight::getMaxAccel(SUMOEmissionClass c, double v, double a, double slope) {
    return -1;
}


SUMOReal
HelpersPHEMlight::computeCO(SUMOEmissionClass c, double v, double a, double slope) {
    return -1;
}


SUMOReal
HelpersPHEMlight::computeCO2(SUMOEmissionClass c, double v, double a, double slope) {
    return -1;
}


SUMOReal
HelpersPHEMlight::computeHC(SUMOEmissionClass c, double v, double a, double slope) {
    return -1;
}


SUMOReal
HelpersPHEMlight::computeNOx(SUMOEmissionClass c, double v, double a, double slope) {
    return -1;
}


SUMOReal
HelpersPHEMlight::computePMx(SUMOEmissionClass c, double v, double a, double slope) {
    return -1;
}


SUMOReal
HelpersPHEMlight::computeFuel(SUMOEmissionClass c, double v, double a, double slope) {
    return -1;
}


SUMOReal
HelpersPHEMlight::computeDefaultCO(SUMOEmissionClass c, double v, double a, double slope, SUMOReal tt) {
    return -1;
}


SUMOReal
HelpersPHEMlight::computeDefaultCO2(SUMOEmissionClass c, double v, double a, double slope, SUMOReal tt) {
    return -1;
}


SUMOReal
HelpersPHEMlight::computeDefaultHC(SUMOEmissionClass c, double v, double a, double slope, SUMOReal tt) {
    return -1;
}


SUMOReal
HelpersPHEMlight::computeDefaultNOx(SUMOEmissionClass c, double v, double a, double slope, SUMOReal tt) {
    return -1;
}


SUMOReal
HelpersPHEMlight::computeDefaultPMx(SUMOEmissionClass c, double v, double a, double slope, SUMOReal tt) {
    return -1;
}


SUMOReal
HelpersPHEMlight::computeDefaultFuel(SUMOEmissionClass c, double v, double a, double slope, SUMOReal tt) {
    return -1;
}


/****************************************************************************/
