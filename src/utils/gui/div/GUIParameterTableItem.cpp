//---------------------------------------------------------------------------//
//                        GUIParameterTableItem.cpp -
//  A single line in the parameter window
//                           -------------------
//  project              : SUMO - Simulation of Urban MObility
//  begin                :
//  copyright            : (C) 2003 by Daniel Krajzewicz
//  organisation         : IVF/DLR http://ivf.dlr.de
//  email                : Daniel.Krajzewicz@dlr.de
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//---------------------------------------------------------------------------//
namespace
{
    const char rcsid[] =
    "$Id$";
}
// $Log$
// Revision 1.2  2005/09/15 12:18:59  dkrajzew
// LARGE CODE RECHECK
//
// Revision 1.1  2004/11/23 10:38:29  dkrajzew
// debugging
//
// Revision 1.1  2004/10/22 12:50:46  dksumo
// initial checkin into an internal, standalone SUMO CVS
//
// Revision 1.5  2004/03/19 12:40:14  dkrajzew
// porting to FOX
//
// Revision 1.4  2003/11/12 14:09:12  dkrajzew
// clean up after recent changes; comments added
//
/* =========================================================================
 * included modules
 * ======================================================================= */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <string>
#include "GUIParameterTableItem.h"
#include <utils/convert/ToString.h>
#include <utils/gui/images/GUIIconSubSys.h>

#ifdef _DEBUG
#include <utils/dev/debug_new.h>
#endif // _DEBUG


/* =========================================================================
 * method definitions
 * ======================================================================= */
GUIParameterTableItem::GUIParameterTableItem(FXTable *table, size_t pos,
                                             const std::string &name,
                                             bool dynamic,
                                             ValueSource<double> *src)
    : myAmDynamic(dynamic), myName(name), myTablePosition(pos), mySource(src),
    myValue(src->getValue()), myTable(table)
{
    init(dynamic, toString<double>(src->getValue()));
}


GUIParameterTableItem::GUIParameterTableItem(FXTable *table, size_t pos,
                                             const std::string &name,
                                             bool dynamic,
                                             double value)
    : myAmDynamic(dynamic), myName(name), myTablePosition(pos), mySource(0),
    myValue(value), myTable(table)
{
    init(dynamic, toString<double>(value));
}


GUIParameterTableItem::GUIParameterTableItem(FXTable *table, size_t pos,
                                             const std::string &name,
                                             bool dynamic,
                                             std::string value)
    : myAmDynamic(dynamic), myName(name), myTablePosition(pos), mySource(0),
    myValue(0), myTable(table)
{
    init(dynamic, value);
}


void
GUIParameterTableItem::init(bool dynamic, std::string value)
{
    myTable->setItemText(myTablePosition, 0, myName.c_str());
    myTable->setItemText(myTablePosition, 1, value.c_str());
    if(dynamic) {
        myTable->setItemIcon(myTablePosition, 2,
            GUIIconSubSys::getIcon(ICON_YES));
    } else {
        myTable->setItemIcon(myTablePosition, 2,
            GUIIconSubSys::getIcon(ICON_NO));
    }
//    myTable->setItemIconPosition(myTablePosition, 2, FXTableItem::ABOVE);
    myTable->setItemJustify(myTablePosition, 2,
        FXTableItem::CENTER_X|FXTableItem::CENTER_Y);
}


GUIParameterTableItem::~GUIParameterTableItem()
{
}


bool
GUIParameterTableItem::dynamic() const
{
    return myAmDynamic;
}


const std::string &
GUIParameterTableItem::getName() const
{
    return myName;
}


void
GUIParameterTableItem::update()
{
    if(!dynamic()||mySource==0) {
        return;
    }
    double value = mySource->getValue();
    if(value!=myValue) {
        myValue = value;
        myTable->setItemText(myTablePosition, 1,
            toString<double>(myValue).c_str());
    }
}


ValueSource<double> *
GUIParameterTableItem::getSourceCopy() const
{
    if(mySource==0) {
        return 0;
    }
    return mySource->copy();
}


/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/

// Local Variables:
// mode:C++
// End:

