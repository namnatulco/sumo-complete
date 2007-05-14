/****************************************************************************/
/// @file    PCXMLPoints.h
/// @author  Daniel Krajzewicz
/// @date    Thu, 02.11.2006
/// @version $Id: PCXMLPoints.h 3547 2007-02-21 14:38:24 +0100 (Mi, 21 Feb 2007) dkrajzew $
///
// A reader of pois stored in XML-format
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
#ifndef PCXMLPoints_h
#define PCXMLPoints_h
// ===========================================================================
// compiler pragmas
// ===========================================================================
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif


// ===========================================================================
// included modules
// ===========================================================================
#ifdef WIN32
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <string>
#include "PCPolyContainer.h"
#include "PCTypeMap.h"
#include <utils/xml/SUMOSAXHandler.h>


// ===========================================================================
// xerces 2.2 compatibility
// ===========================================================================
#if defined(XERCES_HAS_CPP_NAMESPACE)
using namespace XERCES_CPP_NAMESPACE;
#endif


// ===========================================================================
// class definitions
// ===========================================================================
class OptionsCont;


// ===========================================================================
// class declarations
// ===========================================================================
/**
 * @class PCXMLPoints
 * @brief A reader of pois stored in XML-format
 *
 * Reads pois stored as XML definition.
 */
class PCXMLPoints : public SUMOSAXHandler
{
public:
    /// Constructor
    PCXMLPoints(PCPolyContainer &toFill,
                  PCTypeMap &tm, OptionsCont &oc);

    /// Destructor
    ~PCXMLPoints();

    /// loads Elmar's data parsed from GDF
    void load(OptionsCont &oc);

protected:
    /** implementation of the GenericSAXHandler-myStartElement - interface */
    virtual void myStartElement(SumoXMLTag element, const std::string &name,
                                const Attributes &attrs);

    /** implementation of the GenericSAXHandler-myCharacters - interface */
    virtual void myCharacters(SumoXMLTag element, const std::string &name,
                              const std::string &chars);

    /** implementation of the GenericSAXHandler-myEndElement - interface */
    virtual void myEndElement(SumoXMLTag element, const std::string &name);


private:
    /// The container to store the converted polygons into
    PCPolyContainer &myCont;

    /// The type map to use
    PCTypeMap &myTypeMap;

    /// Settings to use
    OptionsCont &myOptions;

};


#endif

/****************************************************************************/

