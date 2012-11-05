/****************************************************************************/
/// @file    NWWriter_DlrNavteq.h
/// @author  Jakob Erdmann
/// @date    26.10.2012
/// @version $Id: NWWriter_DlrNavteq.h 12012 2012-03-05 09:41:53Z namdre $
///
// Exporter writing networks using DlrNavteq (Elmar) format
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
#ifndef NWWriter_DlrNavteq_h
#define NWWriter_DlrNavteq_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <string>
#include <map>
#include <utils/xml/SUMOSAXHandler.h>
#include <utils/common/UtilExceptions.h>


// ===========================================================================
// class declarations
// ===========================================================================
class NBEdge;
class NBEdgeCont;
class NBNetBuilder;
class NBNode;
class NBNodeCont;
class NBTrafficLightLogicCont;
class NBTypeCont;
class OptionsCont;


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class NWWriter_DlrNavteq
 * @brief Exporter writing networks using XML (native input) format
 *
 */
class NWWriter_DlrNavteq {
public:
    /** @brief Writes the network into XML-files (nodes, edges, connections,
     *   traffic lights)
     * @param[in] oc The options to use
     * @param[in] nb The network builder from which to read data
     */
    static void writeNetwork(const OptionsCont& oc, NBNetBuilder& nb);

private:
    /** @brief Writes the nodes_unsplitted file
     * @param[in] oc The options to use
     * @param[in] nc The node-container from which to read data
     * @param[in] ec The edge-container from which to read data
     */
    static void writeNodesUnsplitted(const OptionsCont& oc, NBNodeCont& nc, NBEdgeCont& ec);

    /** @brief Writes the links_unsplitted files
     * @param[in] oc The options to use
     * @param[in] nc The node-container from which to read data
     * @param[in] ec The edge-container from which to read data
     */
    static void writeLinksUnsplitted(const OptionsCont& oc, NBNodeCont& nc, NBEdgeCont& ec);


    /// @brief write header comments (input paramters, date, etc...)
    static void writeHeader(OutputDevice& device, const OptionsCont& oc);
    
    /// @brief build the ascii-bit-vector for column vehicle_type
    static std::string getAllowedTypes(SVCPermissions permissions);

    /// @brief get the navteq road class
    static int getRoadClass(NBEdge* edge);

    /// @brief get the navteq speed class 
    static int getSpeedCategory(SUMOReal speed);

    /// @brief get the lane number encoding
    static int getNavteqLaneCode(unsigned int numLanes);

    /// @brief magic value for undefined stuff
    static const std::string UNDEFINED;

};


#endif

/****************************************************************************/
