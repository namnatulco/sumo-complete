/***************************************************************************
                          MSBitSetLogic.cpp  -  Logic based on std::bitset.
                             -------------------
    begin                : Wed, 12 Dez 2001
    copyright            : (C) 2001 by Christian Roessel
    email                : roessel@zpr.uni-koeln.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// $Log$
// Revision 1.5  2003/12/04 13:30:41  dkrajzew
// work on internal lanes
//
// Revision 1.4  2003/05/20 09:31:46  dkrajzew
// emission debugged; movement model reimplemented (seems ok); detector output debugged; setting and retrieval of some parameter added
//
// Revision 1.3  2003/02/07 10:41:50  dkrajzew
// updated
//
// Revision 1.2  2002/10/16 16:39:01  dkrajzew
// complete deletion within destructors implemented; clear-operator added for container; global file include
//
// Revision 1.1  2002/10/16 14:48:26  dkrajzew
// ROOT/sumo moved to ROOT/src
//
// Revision 1.9  2002/06/25 10:44:42  croessel
// rcsid removed because of multiple-definition problems with MSVC++
//
// Revision 1.8  2002/06/21 10:59:09  dkrajzew
// inclusion of .cpp-files in .cpp files removed
//
// Revision 1.7  2002/06/20 08:00:59  dkrajzew
// template and .cpp inclusion inserted due to problems with MSVC++; should
// be revalidated and removed as soon as possible
//
// Revision 1.6  2002/06/18 18:29:07  croessel
// Added #ifdef EXTERNAL_TEMPLATE_DEFINITION to prevent multiple inclusions.
//
// Revision 1.5  2002/06/18 16:41:50  croessel
// Re-addition of this file. Is now cfront-compliant.
//
// Revision 1.3  2002/04/11 11:33:56  croessel
// in respond(): changed if with &&.
//
// Revision 1.2  2002/04/11 11:30:29  croessel
// in respond(): Perform response-calculation for set-responds only.
//
// Revision 1.1.1.1  2002/04/08 07:21:23  traffic
// new project name
//
// Revision 2.2  2002/03/06 11:04:17  croessel
// Assert added to be sure that passed respond has the correct size.
//
// Revision 2.1  2002/02/21 19:50:22  croessel
// MSVC++ Linking-problems solved, hopefully.
//
// Revision 2.0  2002/02/14 14:43:13  croessel
// Bringing all files to revision 2.0. This is just cosmetics.
//
// Revision 1.5  2002/01/31 14:04:28  croessel
// Changed construtor logic- and transform-parameters to pointer-type.
//
// Revision 1.4  2002/01/31 14:01:52  croessel
// Pleased MSVC++ who cannot handle declarations in for-loops.
//
// Revision 1.3  2002/01/30 14:45:48  croessel
// Destructor-definition added.
//
// Revision 1.2  2002/01/30 10:48:33  croessel
// Introduction of a fixed size MSBitSetLogic<64>=MSBitsetLogic
// (different spelling). Constructor and respond() needed slight
// modifications.
//
// Revision 1.1  2001/12/19 15:47:36  croessel
// Initial commit.
//

/* =========================================================================
 * included modules
 * ======================================================================= */
#include <bitset>
#include <vector>
#include <cassert>
#include <iostream>
#include "MSJunctionLogic.h"
#include "MSLogicJunction.h"
#include "MSBitSetLogic.h"


/* =========================================================================
 * member method definitions
 * ======================================================================= */
template< size_t N >
MSBitSetLogic< N >::MSBitSetLogic< N >( unsigned int nLinks,
                                        unsigned int nInLanes,
                                        Logic* logic, Foes *foes)
    : MSJunctionLogic( nLinks, nInLanes ),
    myLogic( logic ), myInternalLinksFoes(foes)
{
}

//-------------------------------------------------------------------------//

template< size_t N >
MSBitSetLogic< N >::~MSBitSetLogic< N >()
{
    delete myLogic;
}

//-------------------------------------------------------------------------//

template< size_t N > void
MSBitSetLogic< N >::respond(const MSLogicJunction::Request& request,
                            const MSLogicJunction::InnerState& innerState,
                            MSLogicJunction::Respond& respond ) const
{
    size_t i;
    // calculate respond
    for ( i = 0; i < myNLinks; ++i ) {

        bool linkPermit = request.test( i ) &&
            ( request & ( *myLogic )[ i ]).none();
        respond.set( i, linkPermit );
//        std::cout << ( *myLogic )[ i ] << std::endl;
    }
//    std::cout << "- - - - - - -" << std::endl;
    // check whether internal lanes disallow any movement
    //  the number of internal lanes is equal to the number of links
    for ( i = 0; i < myNLinks; ++i ) {

        bool linkPermit = request.test( i ) &&
            ( innerState & ( *myInternalLinksFoes )[ i ]).none();
        std::bitset<64> bla = ( *myInternalLinksFoes )[ i ];
        respond.set( i, linkPermit );
  //      std::cout << ( *myInternalLinksFoes )[ i ] << std::endl;
    }
//    std::cout << "-------------" << std::endl;

}


//-------------------------------------------------------------------------//

/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/
//#ifdef DISABLE_INLINE
//#include "MSLogicJunction.icc"
//#endif


// Local Variables:
// mode:C++
// End:
