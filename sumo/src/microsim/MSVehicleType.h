#ifndef MSVehicleType_H
#define MSVehicleType_H
/***************************************************************************
                          MSVehicleType.h  -  Base Class for Vehicle
                          parameters.
                             -------------------
    begin                : Mon, 12 Mar 2001
    copyright            : (C) 2001 by ZAIK http://www.zaik.uni-koeln.de/AFS
    author               : Christian Roessel
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
// Revision 1.6  2003/07/30 10:02:38  dkrajzew
// support for the vehicle display removed by now
//
// Revision 1.5  2003/05/27 18:40:49  roessel
// Removed superfluous const.
//
// Revision 1.4  2003/05/20 09:31:47  dkrajzew
// emission debugged; movement model reimplemented (seems ok); detector output
// debugged; setting and retrieval of some parameter added
//
// Revision 1.3  2003/02/07 10:41:51  dkrajzew
// updated
//
// Revision 1.2  2002/10/16 16:39:03  dkrajzew
// complete deletion within destructors implemented; clear-operator added for
// container; global file include
//
// Revision 1.1  2002/10/16 14:48:26  dkrajzew
// ROOT/sumo moved to ROOT/src
//
// Revision 1.4  2002/07/31 17:33:01  roessel
// Changes since sourceforge cvs request.
//
// Revision 1.4  2002/07/31 14:41:05  croessel
// New methods return often used precomputed values.
//
// Revision 1.3  2002/05/29 17:06:04  croessel
// Inlined some methods. See the .icc files.
//
// Revision 1.2  2002/04/11 15:25:56  croessel
// Changed float to double.
//
// Revision 1.1.1.1  2002/04/08 07:21:24  traffic
// new project name
//
// Revision 2.0  2002/02/14 14:43:20  croessel
// Bringing all files to revision 2.0. This is just cosmetics.
//
// Revision 1.9  2002/02/13 10:10:20  croessel
// Added two static methods: minDecel() and maxLength(). They are needed
// to calculate safe gaps in the case no predecessor resp. successor is
// known.
//
// Revision 1.8  2002/02/05 13:51:53  croessel
// GPL-Notice included.
// In *.cpp files also config.h included.
//
// Revision 1.7  2002/02/01 13:57:07  croessel
// Changed methods and members bmax/dmax/sigma to more meaningful names
// accel/decel/dawdle.
//
// Revision 1.6  2001/12/20 14:53:34  croessel
// using namespace std replaced by std::
// Copy-ctor and assignment-operator moved to private.
//
// Revision 1.5  2001/11/15 17:12:14  croessel
// Outcommented the inclusion of the inline *.iC files. Currently not
// needed.
//
// Revision 1.4  2001/11/14 11:45:54  croessel
// Resolved conflicts which appeared during suffix-change and
// CR-line-end commits.
//
// Revision 1.3  2001/09/06 15:37:06  croessel
// Set default values in the constructor.
//
// Revision 1.2  2001/07/16 12:55:47  croessel
// Changed id type from unsigned int to string. Added string-pointer dictionaries and dictionary methods.
//
// Revision 1.1.1.1  2001/07/11 15:51:13  traffic
// new start
//
/* =========================================================================
 * included modules
 * ======================================================================= */
#include <map>
#include <string>


/* =========================================================================
 * class declarations
 * ======================================================================= */
class MSLane;


/* =========================================================================
 * class definitions
 * ======================================================================= */
/**
 */
class MSVehicleType
{
public:

	// !!!
    friend class MSVehicle;

    /// Constructor.
    MSVehicleType( std::string id, double length, double maxSpeed,
                   double accel = 0.8, double decel = 4.5, double dawdle = 0.5 );

    /// Destructor.
    ~MSVehicleType();

    /// Get vehicle type's length [m].
    double length() const;

    /// Get vehicle type's maximum speed [m/s].
    double maxSpeed() const;

    /// Get the vehicle type's maximum acceleration [m/s^2]
    double accel() const;

    /// Get the vehicle type's maximum deceleration [m/s^2]
    double decel() const;

    /// Get the vehicle type's dawdle-probability. Out of [0,1]
    double dawdle() const;

    /// Returns the minimum deceleration-ability of all vehicle-types.
    static double minDecel();

    /// Returns the maximum length of all vehicle-types.
    static double maxLength();

    /// Returns precomputed accel * deltaT
    double accelSpeed( void ) const;

    /// Returns precomputed decel * deltaT
    double decelSpeed( void ) const;

    /// Returns precomputed ( accel + decel ) * deltaT
    double accelPlusDecelSpeed( void ) const;

    /// Returns precomputed 1 / ( 2 * decel )
    double inversTwoDecel( void ) const;

    /// Returns precomputed accel * deltaT^2
    double accelDist( void ) const;

    /// Returns precomputed decel * deltaT^2
    double decelDist( void ) const;

    /** Inserts a MSVehicleType into the static dictionary and returns true
        if the key id isn't already in the dictionary. Otherwise returns
        false. */
    static bool dictionary( std::string id, MSVehicleType* edge);

    /** Returns the MSVehicleType associated to the key id if exists,
        otherwise returns 0. */
    static MSVehicleType* dictionary( std::string id);

    /** Clears the dictionary */
    static void clear();

	/// returns the name of the vehicle type
    const std::string &id() const;

private:
    /// Unique ID.
    std::string myID;

    /// Vehicle type's length [m].
    double myLength;

    /// Vehicle type's maximum speed [m/s].
    double myMaxSpeed;

    /// The vehicle type's maximum acceleration [m/s^2]
    double myAccel;

    /// The vehicle type's maximum deceleration [m/s^2]
    double myDecel;

    /// The vehicle type's dawdle-parameter. 0 for no dawdling, 1 for max.
    double myDawdle;


    double myAccelSpeed;
    double myDecelSpeed;
    double myAccelPlusDecelSpeed;
    double myInversTwoDecel;
    double myAccelDist;
    double myDecelDist;

    /// Minimum deceleration-ability of all vehicle-types.
    static double myMinDecel;

    /// Maximum length of all vehicle-types.
    static double myMaxLength;

    /// Definition of the type of the static dictionary to associate string-ids with objects.
    typedef std::map< std::string, MSVehicleType* > DictType;
    /// Static dictionary to associate string-ids with objects.
    static DictType myDict;

    /// Default constructor.
    MSVehicleType();

    /// Copy constructor.
    MSVehicleType( const MSVehicleType& );

    /// Assignment operator.
    MSVehicleType& operator=( const MSVehicleType& );

};


/**************** DO NOT DECLARE ANYTHING AFTER THE INCLUDE ****************/

#ifndef DISABLE_INLINE
#include "MSVehicleType.icc"
#endif

#endif

// Local Variables:
// mode:C++
// End:
