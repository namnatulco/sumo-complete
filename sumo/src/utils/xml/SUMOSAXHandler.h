/****************************************************************************/
/// @file    SUMOSAXHandler.h
/// @author  Daniel Krajzewicz
/// @date    Sept 2002
/// @version $Id$
///
// SAX-handler base for SUMO-files
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
#ifndef SUMOSAXHandler_h
#define SUMOSAXHandler_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <string>
#include <utils/xml/GenericSAXHandler.h>
#include <utils/common/FileErrorReporter.h>


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class SUMOSAXHandler
 * @brief SAX-handler base for SUMO-files
 *
 * This class is a GenericSAXHandler which knows all tags SUMO uses, so all 
 *  SUMO-XML - loading classes should be derived from it.
 *
 * Besides the functionality of GenericSAXHandler it offers interfaces for
 *  setting and reading the name of the currently processed file. Additionally,
 *  this class also implements methods for dealing with SAX-parser errors.
 */
class SUMOSAXHandler : public GenericSAXHandler
{
public:
    /// Constructor
    SUMOSAXHandler(const std::string &filetype,
                   const std::string &file="") throw();


    /// Destructor
    virtual ~SUMOSAXHandler() throw();


    ///{ Handlers for the SAX ErrorHandler interface
    /**
     * @brief Handler for XML-warnings
     *
     * The message is built using buildErrorMessage and reported
     *  to the warning-instance of the MsgHandler.
     */
    void warning(const SAXParseException& exception) throw();

    /**
     * @brief Handler for XML-errors
     *
     * The message is built using buildErrorMessage and thrown within a ProcessError.
     */
    void error(const SAXParseException& exception) throw(ProcessError);

    /**
     * @brief Handler for XML-errors
     *
     * The message is built using buildErrorMessage and thrown within a ProcessError.
     */
    void fatalError(const SAXParseException& exception) throw(ProcessError);
    ///}


    /** 
     * @brief Sets the current file name
     * @todo Hmmm - this is as unsafe as having a direct access to the variable; recheck
     */
    void setFileName(const std::string &name) throw();


    /** 
     * @brief returns the current file name
     */
    const std::string &getFileName() const throw();


protected:
    /** 
     * @brief Builds an error message
     * 
     * The error message includes the file name and the line/column information
     *  as supported by the given SAXParseException
     */
    std::string buildErrorMessage(const SAXParseException& exception) throw();


private:
    /// The name of the currently parsed file
    std::string myFileName;


private:
    /// invalidated copy constructor
    SUMOSAXHandler(const SUMOSAXHandler &s);

    /// invalidated assignment operator
    const SUMOSAXHandler &operator=(const SUMOSAXHandler &s);

};


#endif

/****************************************************************************/

