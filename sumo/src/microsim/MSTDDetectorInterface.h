#ifndef MSTDDETECTORINTERFACE_H
#define MSTDDETECTORINTERFACE_H

///
/// @file    MSTDDetectorInterface.h
/// @author  Christian Roessel <christian.roessel@dlr.de>
/// @date    Started Tue Dec 02 2003 20:03 CET
/// @version $Id$
///
/// @brief   
///
///

/* Copyright (C) 2003 by German Aerospace Center (http://www.dlr.de) */

//---------------------------------------------------------------------------//
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//---------------------------------------------------------------------------//

#include "MSDetectorInterfaceCommon.h"
#include "MSUpdateEachTimestep.h"

namespace TD // timestep data
{
    
    class MSDetectorInterface
        :
        public MSDetectorInterfaceCommon
        , public MSUpdateEachTimestep< MSDetectorInterface >
    {
    public:

        // returns the last aggregated data value
        virtual double getCurrent( void ) const = 0;
        
        virtual ~MSDetectorInterface( void )
            {}
        
    protected:

        MSDetectorInterface( std::string id )
            :
            MSDetectorInterfaceCommon( id )
            {}
        
    private:
        
    };
    
} // end namespace TD

namespace Detector
{
    typedef MSUpdateEachTimestep< TD::MSDetectorInterface > UpdateE2Detectors;
}


// Local Variables:
// mode:C++
// End:

#endif // MSTDDETECTORINTERFACE_H
