/****************************************************************************/
/// @file    PCLoaderElmar.h
/// @author  Daniel Krajzewicz
/// @date    Thu, 02.11.2006
/// @version $Id: PCLoaderElmar.h 5095 2008-02-21 09:01:33Z dkrajzew $
///
// A reader of pois and polygons stored in Elmar-format
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
#ifndef PCLoaderElmar_h
#define PCLoaderElmar_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <string>
#include "PCPolyContainer.h"
#include "PCTypeMap.h"
#include <utils/common/UtilExceptions.h>


// ===========================================================================
// class definitions
// ===========================================================================
class OptionsCont;


// ===========================================================================
// class declarations
// ===========================================================================
/**
 * @class PCLoaderElmar
 * @brief A reader of pois and polygons stored in Elmar-format
 *
 * Reads pois stored in "pointcollection.txt" and polygons stored in 
 *  "...polygons.txt"/"...water_polygons.txt", applies the given projection 
 *  and network offset and stores the so build pois/polys into the given map.
 */
class PCLoaderElmar
{
public:
    /** @brief Loads pois/polygons assumed to be stored as according Elmar-files
     *
     * If the option "elmar-poi-files" is set within the given options container,
     *  the files stored herein are parsed using "loadPOIFiles", assuming this
     *  option contains file paths to files containing pois stored in Elmar's
     *  "pointcollection.txt"-format.
     *
     * If the option "elmar-poly-files" is set within the given options container,
     *  the files stored herein are parsed using "loadPolyFiles", assuming this
     *  option contains file paths to files containing polygons stored in Elmar's
     *  "...polygons.txt"/"...water_polygons.txt"-format.
     *
     * @param[in] oc The options container to get further options from
     * @param[in] toFill The poly/pois container to add loaded polys/pois to
     * @param[in] tm The type map to use for setting values of loaded polys/pois
     * @exception ProcessError if something fails
     */
    static void loadIfSet(OptionsCont &oc, PCPolyContainer &toFill,
        PCTypeMap &tm) throw(ProcessError);

protected:
    /** @brief Loads pois assumed to be stored as according Elmar-files
     *
     * Goes through the list of files given in "elmar-poi-files". Calls
     *  "loadPOIFile" using each of these as the first parameter.
     *
     * @param[in] oc The options container to get further options from
     * @param[in] toFill The poly/pois container to add loaded pois to
     * @param[in] tm The type map to use for setting values of loaded pois
     * @exception ProcessError if something fails
     */
    static void loadPOIFiles(OptionsCont &oc, PCPolyContainer &toFill,
        PCTypeMap &tm) throw(ProcessError);


    /** @brief Loads polygons assumed to be stored as according Elmar-files
     *
     * Goes through the list of files given in "elmar-poly-files". Calls
     *  "loadPolyFile" using each of these as the first parameter.
     *
     * @param[in] oc The options container to get further options from
     * @param[in] toFill The poly/pois container to add loaded polys to
     * @param[in] tm The type map to use for setting values of loaded polys
     * @exception ProcessError if something fails
     */
    static void loadPolyFiles(OptionsCont &oc, PCPolyContainer &toFill,
        PCTypeMap &tm) throw(ProcessError);


    /** @brief Loads Elmar-pois from the given file
     * @param[in] file The name of the file to parse
     * @param[in] oc The options container to get further options from
     * @param[in] toFill The poly/pois container to add loaded polys to
     * @param[in] tm The type map to use for setting values of loaded polys
     * @exception ProcessError if something fails
     */
    static void loadPOIFile(const std::string &file, 
        OptionsCont &oc, PCPolyContainer &toFill,
        PCTypeMap &tm) throw(ProcessError);


    /** @brief Loads Elmar-polygons from the given file
     * @param[in] file The name of the file to parse
     * @param[in] oc The options container to get further options from
     * @param[in] toFill The poly/pois container to add loaded polys to
     * @param[in] tm The type map to use for setting values of loaded polys
     * @exception ProcessError if something fails
     */
    static void loadPolyFile(const std::string &file, 
        OptionsCont &oc, PCPolyContainer &toFill,
        PCTypeMap &tm) throw(ProcessError);


};


#endif

/****************************************************************************/

