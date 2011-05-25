/****************************************************************************/
/// @file    NIImporter_ArcView.cpp
/// @author  Daniel Krajzewicz
/// @date    Sept 2002
/// @version $Id$
///
// Importer for networks stored in ArcView-shape format
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// Copyright (C) 2001-2011 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
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

#include <string>
#include <utils/common/MsgHandler.h>
#include <utils/common/ToString.h>
#include <utils/common/TplConvert.h>
#include <utils/common/StringUtils.h>
#include <utils/options/OptionsCont.h>
#include <utils/geom/GeomHelper.h>
#include <netbuild/NBNetBuilder.h>
#include <netbuild/NBHelpers.h>
#include <netbuild/NBEdge.h>
#include <netbuild/NBEdgeCont.h>
#include <netbuild/NBTypeCont.h>
#include <netbuild/NBNode.h>
#include <netbuild/NBNodeCont.h>
#include "NIImporter_ArcView.h"
#include <netimport/NINavTeqHelper.h>
#include <utils/geom/GeoConvHelper.h>
#include <utils/common/FileHelpers.h>

#ifdef HAVE_GDAL
#include <ogrsf_frmts.h>
#endif

#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif // CHECK_MEMORY_LEAKS


// ===========================================================================
// method definitions
// ===========================================================================
// ---------------------------------------------------------------------------
// static methods (interface in this case)
// ---------------------------------------------------------------------------
void
NIImporter_ArcView::loadNetwork(const OptionsCont &oc, NBNetBuilder &nb) {
    if (!oc.isSet("shapefile-prefix")) {
        return;
    }
    // check whether the correct set of entries is given
    //  and compute both file names
    std::string dbf_file = oc.getString("shapefile-prefix") + ".dbf";
    std::string shp_file = oc.getString("shapefile-prefix") + ".shp";
    std::string shx_file = oc.getString("shapefile-prefix") + ".shx";
    // check whether the files do exist
    if (!FileHelpers::exists(dbf_file)) {
        MsgHandler::getErrorInstance()->inform("File not found: " + dbf_file);
    }
    if (!FileHelpers::exists(shp_file)) {
        MsgHandler::getErrorInstance()->inform("File not found: " + shp_file);
    }
    if (!FileHelpers::exists(shx_file)) {
        MsgHandler::getErrorInstance()->inform("File not found: " + shx_file);
    }
    if (MsgHandler::getErrorInstance()->wasInformed()) {
        return;
    }
    // load the arcview files
    NIImporter_ArcView loader(oc,
                              nb.getNodeCont(), nb.getEdgeCont(), nb.getTypeCont(),
                              dbf_file, shp_file, oc.getBool("speed-in-kmh"));
    loader.load();
}



// ---------------------------------------------------------------------------
// loader methods
// ---------------------------------------------------------------------------
NIImporter_ArcView::NIImporter_ArcView(const OptionsCont &oc,
                                       NBNodeCont &nc,
                                       NBEdgeCont &ec,
                                       NBTypeCont &tc,
                                       const std::string &dbf_name,
                                       const std::string &shp_name,
                                       bool speedInKMH)
        : myOptions(oc), mySHPName(shp_name),
        myNameAddition(0),
        myNodeCont(nc), myEdgeCont(ec), myTypeCont(tc),
        mySpeedInKMH(speedInKMH),
        myRunningNodeID(0) {
    UNUSED_PARAMETER(dbf_name);
}


NIImporter_ArcView::~NIImporter_ArcView() {}


void
NIImporter_ArcView::load() {
#ifdef HAVE_GDAL
    MsgHandler::getMessageInstance()->beginProcessMsg("Loading data from '" + mySHPName + "'...");
    OGRRegisterAll();
    OGRDataSource *poDS = OGRSFDriverRegistrar::Open(mySHPName.c_str(), FALSE);
    if (poDS == NULL) {
        MsgHandler::getErrorInstance()->inform("Could not open shape description '" + mySHPName + "'.");
        return;
    }

    // begin file parsing
    OGRLayer  *poLayer = poDS->GetLayer(0);
    poLayer->ResetReading();

    // build coordinate transformation
    OGRSpatialReference *origTransf = poLayer->GetSpatialRef();
    OGRSpatialReference destTransf;
    // use wgs84 as destination
    destTransf.SetWellKnownGeogCS("WGS84");
    OGRCoordinateTransformation *poCT = OGRCreateCoordinateTransformation(origTransf, &destTransf);
    if (poCT == NULL) {
        if (myOptions.isSet("shapefile.guess-projection")) {
            OGRSpatialReference origTransf2;
            origTransf2.SetWellKnownGeogCS("WGS84");
            poCT = OGRCreateCoordinateTransformation(&origTransf2, &destTransf);
        }
        if (poCT==0) {
            WRITE_WARNING("Could not create geocoordinates converter; check whether proj.4 is installed.");
        }
    }

    OGRFeature *poFeature;
    poLayer->ResetReading();
    while ((poFeature = poLayer->GetNextFeature()) != NULL) {
        // read in edge attributes
        std::string id =
            myOptions.isSet("shapefile.street-id")
            ? poFeature->GetFieldAsString((char*)(myOptions.getString("shapefile.street-id").c_str()))
            : poFeature->GetFieldAsString("LINK_ID");
        id = StringUtils::prune(id);
        if (id=="") {
            MsgHandler::getErrorInstance()->inform("Could not obtain edge id.");
            return;
        }
        std::string name =
            myOptions.isSet("shapefile.street-id")
            ? poFeature->GetFieldAsString((char*) myOptions.getString("shapefile.street-id").c_str())
            : poFeature->GetFieldAsString("ST_NAME");
        name = StringUtils::prune(StringUtils::replace(name, "&", "&amp;"));

        std::string from_node =
            myOptions.isSet("shapefile.from-id")
            ? poFeature->GetFieldAsString((char*)(myOptions.getString("shapefile.from-id").c_str()))
            : poFeature->GetFieldAsString("REF_IN_ID");
        from_node = StringUtils::prune(from_node);
        std::string to_node =
            myOptions.isSet("shapefile.to-id")
            ? poFeature->GetFieldAsString((char*) myOptions.getString("shapefile.to-id").c_str())
            : poFeature->GetFieldAsString("NREF_IN_ID");
        to_node = StringUtils::prune(to_node);
        if (from_node==""||to_node=="") {
            from_node = toString(myRunningNodeID++);
            to_node = toString(myRunningNodeID++);
        }
        std::string type = poFeature->GetFieldAsString("ST_TYP_AFT");
        SUMOReal width = myTypeCont.getWidth(type);
        SUMOReal speed = getSpeed(*poFeature, id);
        unsigned int nolanes = getLaneNo(*poFeature, id, speed);
        int priority = getPriority(*poFeature, id);
        if (nolanes==0||speed==0) {
            if (myOptions.getBool("shapefile.use-defaults-on-failure")) {
                nolanes = myTypeCont.getNoLanes("");
                speed = myTypeCont.getSpeed("");
            } else {
                OGRFeature::DestroyFeature(poFeature);
                MsgHandler::getErrorInstance()->inform("The description seems to be invalid. Please recheck usage of types.");
                return;
            }
        }
        if (mySpeedInKMH) {
            speed = speed / (SUMOReal) 3.6;
        }


        // read in the geometry
        OGRGeometry *poGeometry = poFeature->GetGeometryRef();
        OGRwkbGeometryType gtype = poGeometry->getGeometryType();
        assert(gtype==wkbLineString);
        OGRLineString *cgeom = (OGRLineString*) poGeometry;
        if (poCT!=0) {
            // try transform to wgs84
            cgeom->transform(poCT);
        }

        PositionVector shape;
        for (int j=0; j<cgeom->getNumPoints(); j++) {
            Position pos((SUMOReal) cgeom->getX(j), (SUMOReal) cgeom->getY(j));
            if (!GeoConvHelper::x2cartesian(pos)) {
                WRITE_WARNING("Unable to project coordinates for edge '" + id + "'.");
            }
            shape.push_back_noDoublePos(pos);
        }

        // build from-node
        NBNode *from = myNodeCont.retrieve(from_node);
        if (from==0) {
            Position from_pos = shape[0];
            from = myNodeCont.retrieve(from_pos);
            if (from==0) {
                from = new NBNode(from_node, from_pos);
                if (!myNodeCont.insert(from)) {
                    MsgHandler::getErrorInstance()->inform("Node '" + from_node + "' could not be added");
                    delete from;
                    continue;
                }
            }
        }
        // build to-node
        NBNode *to = myNodeCont.retrieve(to_node);
        if (to==0) {
            Position to_pos = shape[-1];
            to = myNodeCont.retrieve(to_pos);
            if (to==0) {
                to = new NBNode(to_node, to_pos);
                if (!myNodeCont.insert(to)) {
                    MsgHandler::getErrorInstance()->inform("Node '" + to_node + "' could not be added");
                    delete to;
                    continue;
                }
            }
        }

        if (from==to) {
            WRITE_WARNING("Edge '" + id + "' connects identical nodes, skipping.");
            continue;
        }

        // retrieve the information whether the street is bi-directional
        std::string dir;
        int index = poFeature->GetDefnRef()->GetFieldIndex("DIR_TRAVEL");
        if (index>=0&&poFeature->IsFieldSet(index)) {
            dir = poFeature->GetFieldAsString(index);
        }
        // add positive direction if wanted
        if (dir=="B"||dir=="F"||dir==""||myOptions.getBool("shapefile.all-bidirectional")) {
            if (myEdgeCont.retrieve(id)==0) {
                LaneSpreadFunction spread = dir=="B"||dir=="FALSE" ? LANESPREAD_RIGHT : LANESPREAD_CENTER;
                NBEdge *edge = new NBEdge(id, from, to, type, speed, nolanes, priority, width, -1, shape, spread);
                myEdgeCont.insert(edge);
                checkSpread(edge);
            }
        }
        // add negative direction if wanted
        if (dir=="B"||dir=="T"||myOptions.getBool("shapefile.all-bidirectional")) {
            id = "-" + id;
            if (myEdgeCont.retrieve(id)==0) {
                LaneSpreadFunction spread = dir=="B"||dir=="FALSE" ? LANESPREAD_RIGHT : LANESPREAD_CENTER;
                NBEdge *edge = new NBEdge(id, to, from, type, speed, nolanes, priority, width, -1, shape.reverse(), spread);
                myEdgeCont.insert(edge);
                checkSpread(edge);
            }
        }
        //
        OGRFeature::DestroyFeature(poFeature);
    }
    MsgHandler::getMessageInstance()->endProcessMsg("done.");
#else
    MsgHandler::getErrorInstance()->inform("SUMO was compiled without GDAL support.");
#endif
}

#ifdef HAVE_GDAL
SUMOReal
NIImporter_ArcView::getSpeed(OGRFeature &poFeature, const std::string &edgeid) {
    if (myOptions.isSet("shapefile.type-id")) {
        return myTypeCont.getSpeed(poFeature.GetFieldAsString((char*)(myOptions.getString("shapefile.type-id").c_str())));
    }
    // try to get definitions as to be found in SUMO-XML-definitions
    //  idea by John Michael Calandrino
    int index = poFeature.GetDefnRef()->GetFieldIndex("speed");
    if (index>=0&&poFeature.IsFieldSet(index)) {
        return (SUMOReal) poFeature.GetFieldAsDouble(index);
    }
    index = poFeature.GetDefnRef()->GetFieldIndex("SPEED");
    if (index>=0&&poFeature.IsFieldSet(index)) {
        return (SUMOReal) poFeature.GetFieldAsDouble(index);
    }
    // try to get the NavTech-information
    index = poFeature.GetDefnRef()->GetFieldIndex("SPEED_CAT");
    if (index>=0&&poFeature.IsFieldSet(index)) {
        std::string def = poFeature.GetFieldAsString(index);
        return NINavTeqHelper::getSpeed(edgeid, def);
    }
    return -1;
}


unsigned int
NIImporter_ArcView::getLaneNo(OGRFeature &poFeature, const std::string &edgeid,
                              SUMOReal speed) {
    if (myOptions.isSet("shapefile.type-id")) {
        return (unsigned int) myTypeCont.getNoLanes(poFeature.GetFieldAsString((char*)(myOptions.getString("shapefile.type-id").c_str())));
    }
    // try to get definitions as to be found in SUMO-XML-definitions
    //  idea by John Michael Calandrino
    int index = poFeature.GetDefnRef()->GetFieldIndex("nolanes");
    if (index>=0&&poFeature.IsFieldSet(index)) {
        return (unsigned int) poFeature.GetFieldAsInteger(index);
    }
    index = poFeature.GetDefnRef()->GetFieldIndex("NOLANES");
    if (index>=0&&poFeature.IsFieldSet(index)) {
        return (unsigned int) poFeature.GetFieldAsInteger(index);
    }
    index = poFeature.GetDefnRef()->GetFieldIndex("rnol");
    if (index>=0&&poFeature.IsFieldSet(index)) {
        return (unsigned int) poFeature.GetFieldAsInteger(index);
    }
    index = poFeature.GetDefnRef()->GetFieldIndex("LANE_CAT");
    if (index>=0&&poFeature.IsFieldSet(index)) {
        std::string def = poFeature.GetFieldAsString(index);
        return NINavTeqHelper::getLaneNumber(edgeid, def, speed);
    }
    return 0;
}


int
NIImporter_ArcView::getPriority(OGRFeature &poFeature, const std::string &/*edgeid*/) {
    if (myOptions.isSet("shapefile.type-id")) {
        return myTypeCont.getPriority(poFeature.GetFieldAsString((char*)(myOptions.getString("shapefile.type-id").c_str())));
    }
    // try to get definitions as to be found in SUMO-XML-definitions
    //  idea by John Michael Calandrino
    int index = poFeature.GetDefnRef()->GetFieldIndex("priority");
    if (index>=0&&poFeature.IsFieldSet(index)) {
        return poFeature.GetFieldAsInteger(index);
    }
    index = poFeature.GetDefnRef()->GetFieldIndex("PRIORITY");
    if (index>=0&&poFeature.IsFieldSet(index)) {
        return poFeature.GetFieldAsInteger(index);
    }
    // try to determine priority from NavTechs FUNC_CLASS attribute
    index = poFeature.GetDefnRef()->GetFieldIndex("FUNC_CLASS");
    if (index>=0&&poFeature.IsFieldSet(index)) {
        return poFeature.GetFieldAsInteger(index);
    }
    return 0;
}

void
NIImporter_ArcView::checkSpread(NBEdge *e) {
    NBEdge *ret = e->getToNode()->getConnectionTo(e->getFromNode());
    if (ret!=0) {
        e->setLaneSpreadFunction(LANESPREAD_RIGHT);
        ret->setLaneSpreadFunction(LANESPREAD_RIGHT);
    }
}


#endif



/****************************************************************************/

