/****************************************************************************/
/// @file    SUMOXMLDefinitions.cpp
/// @author  Daniel Krajzewicz
/// @date    Sept 2002
/// @version $Id: SUMOXMLDefinitions.cpp 3486 2007-02-19 13:38:00 +0100 (Mo, 19 Feb 2007) behrisch $
///
// Definitions of SUMO-tags
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

#include <utils/xml/GenericSAXHandler.h>
#include "SUMOXMLDefinitions.h"

#ifdef CHECK_MEMORY_LEAKS
#include <foreign/nvwa/debug_new.h>
#endif // CHECK_MEMORY_LEAKS


// ===========================================================================
// definitions
// ===========================================================================
GenericSAXHandler::Tag sumotags[] =
    {
        { "simulation",       SUMO_TAG_SIMULATION },
        { "edge",             SUMO_TAG_EDGE },
        { "lane",             SUMO_TAG_LANE },
        { "poi",              SUMO_TAG_POI },
        { "poly",             SUMO_TAG_POLY },
        { "lanes",            SUMO_TAG_LANES },
        { "cedge",            SUMO_TAG_CEDGE },
        { "junction",         SUMO_TAG_JUNCTION },
        { "detector",         SUMO_TAG_DETECTOR },
        { "e1-detector",      SUMO_TAG_E1DETECTOR },
        { "e2-detector",      SUMO_TAG_E2DETECTOR },
        { "e3-detector",      SUMO_TAG_E3DETECTOR },
        { "det_entry",        SUMO_TAG_DET_ENTRY },
        { "det_exit",         SUMO_TAG_DET_EXIT },
        { "vehicle",          SUMO_TAG_VEHICLE },
        { "vtype",            SUMO_TAG_VTYPE },
        { "route",            SUMO_TAG_ROUTE },
        { "succ",             SUMO_TAG_SUCC },
        { "succlane",         SUMO_TAG_SUCCLANE },
        { "key",              SUMO_TAG_KEY },
        { "junctionlogic",    SUMO_TAG_JUNCTIONLOGIC },
        { "requestsize",      SUMO_TAG_REQUESTSIZE },
        { "lanenumber",       SUMO_TAG_LANENUMBER },
        { "logicitem",        SUMO_TAG_LOGICITEM },
        { "row-logic",        SUMO_TAG_ROWLOGIC },
        { "source",           SUMO_TAG_SOURCE },
        { "district",         SUMO_TAG_DISTRICT },
        { "dsource",          SUMO_TAG_DSOURCE },
        { "dsink",            SUMO_TAG_DSINK },
        { "edges",            SUMO_TAG_EDGES },
        { "node_count",       SUMO_TAG_NODECOUNT },
        { "tl-logic",         SUMO_TAG_TLLOGIC },
        { "offset",           SUMO_TAG_OFFSET },
        { "initstep",         SUMO_TAG_INITSTEP },
        { "phaseno",          SUMO_TAG_PHASENO },
        { "phase",            SUMO_TAG_PHASE },
        { "routealt",         SUMO_TAG_ROUTEALT },
        { "tripdef",          SUMO_TAG_TRIPDEF },
        { "flow",             SUMO_TAG_FLOW },
        { "subkey",           SUMO_TAG_SUBKEY },
        { "trigger",          SUMO_TAG_TRIGGER },
        { "step",             SUMO_TAG_STEP },
        { "interval",         SUMO_TAG_INTERVAL },
        { "shape",            SUMO_TAG_SHAPE },
        { "timed_event",      SUMO_TAG_TIMEDEVENT },
        { "inclanes",         SUMO_TAG_INCOMING_LANES },
        { "intlanes",         SUMO_TAG_INTERNAL_LANES },
        { "fromedge",         SUMO_TAG_FROMEDGE },
        { "toedge",           SUMO_TAG_TOEDGE },
        { "sink",             SUMO_TAG_SINK },
        { "supplementary-weights", SUMO_TAG_SUPPLEMENTARY_WEIGHTS },
        { "weight",           SUMO_TAG_WEIGHT },
        { "reroute-agent",	  SUMO_TAG_REROUTE_AGENT },
        { "param",	          SUMO_TAG_PARAM },
        { "",                 SUMO_TAG_NOTHING }
    };


GenericSAXHandler::Attr sumoattrs[] =
    {
        { "id",             SUMO_ATTR_ID },
        { "name",           SUMO_ATTR_NAME },
        { "type",           SUMO_ATTR_TYPE },
        { "priority",       SUMO_ATTR_PRIORITY },
        { "nolanes",        SUMO_ATTR_NOLANES },
        { "speed",          SUMO_ATTR_SPEED },

        { "length",         SUMO_ATTR_LENGTH },
        { "fromnode",       SUMO_ATTR_FROMNODE },
        { "tonode",         SUMO_ATTR_TONODE },
        { "xfrom",          SUMO_ATTR_XFROM },
        { "yfrom",          SUMO_ATTR_YFROM },
        { "xto",            SUMO_ATTR_XTO },
        { "yto",            SUMO_ATTR_YTO },

        { "x",              SUMO_ATTR_X },
        { "y",              SUMO_ATTR_Y },

        { "key",            SUMO_ATTR_KEY },
        { "request",        SUMO_ATTR_REQUEST },
        { "response",       SUMO_ATTR_RESPONSE },

        { "weight",         SUMO_ATTR_WEIGHT },

        { "depart",         SUMO_ATTR_DEPART },
        { "route",          SUMO_ATTR_ROUTE },
        { "maxspeed",       SUMO_ATTR_MAXSPEED },
        { "accel",          SUMO_ATTR_ACCEL },
        { "decel",          SUMO_ATTR_DECEL },
        { "sigma",          SUMO_ATTR_SIGMA },
        { "tau",            SUMO_ATTR_TAU },
        { "vclass",         SUMO_ATTR_VCLASS },
        { "repno",          SUMO_ATTR_REPNUMBER },

        { "last",           SUMO_ATTR_LAST },
        { "cost",           SUMO_ATTR_COST },
        { "probability",    SUMO_ATTR_PROB },

        { "pos",            SUMO_ATTR_POS },
        { "lane",           SUMO_ATTR_LANE },
        { "from",           SUMO_ATTR_FROM },
        { "to",             SUMO_ATTR_TO },
        { "period",         SUMO_ATTR_PERIOD },

        { "function",       SUMO_ATTR_FUNC },
        { "pos",            SUMO_ATTR_POSITION },
        { "freq",           SUMO_ATTR_SPLINTERVAL },
        { "style",          SUMO_ATTR_STYLE },
        { "file",           SUMO_ATTR_FILE },
        { "junction",       SUMO_ATTR_JUNCTION },
        { "yield",          SUMO_ATTR_YIELD },
        { "no",             SUMO_ATTR_NO },
        { "phase",          SUMO_ATTR_PHASE },
        { "brake",          SUMO_ATTR_BRAKE },
        { "yellow",         SUMO_ATTR_YELLOW },
        { "duration",       SUMO_ATTR_DURATION },

        { "objecttype",     SUMO_ATTR_OBJECTTYPE },
        { "attr",           SUMO_ATTR_ATTR },
        { "objectid",       SUMO_ATTR_OBJECTID },
        { "time",           SUMO_ATTR_TIME },
        { "multi_ref",      SUMO_ATTR_MULTIR },
        { "traveltime",     SUMO_ATTR_TRAVELTIME },
        { "begin",          SUMO_ATTR_BEGIN },
        { "end",            SUMO_ATTR_END },
        { "tl",             SUMO_ATTR_TLID },
        { "linkno",         SUMO_ATTR_TLLINKNO },
        { "shape",          SUMO_ATTR_SHAPE },
        { "spread_type",    SUMO_ATTR_SPREADFUNC },
        { "color",          SUMO_ATTR_COLOR },
        { "dir",            SUMO_ATTR_DIR },
        { "state",          SUMO_ATTR_STATE },
        { "layer",          SUMO_ATTR_LAYER },
        { "fill",           SUMO_ATTR_FILL },
        { "prefix",         SUMO_ATTR_PREFIX },
        { "discard",        SUMO_ATTR_DISCARD },

        { "dest",           SUMO_ATTR_DEST },
        { "source",         SUMO_ATTR_SOURCE },
        { "via",            SUMO_ATTR_VIA },
        { "int_end",        SUMO_ATTR_INTERNALEND },

        { "min_dur",        SUMO_ATTR_MINDURATION },
        { "max_dur",        SUMO_ATTR_MAXDURATION },
        { "foes",           SUMO_ATTR_FOES },
        { "measures",       SUMO_ATTR_MEASURES },

        { "cont",           SUMO_ATTR_CONT },
        { "time_treshold",  SUMO_ATTR_HALTING_TIME_THRESHOLD },
        { "speed_treshold", SUMO_ATTR_HALTING_SPEED_THRESHOLD },
        { "jam_treshold",   SUMO_ATTR_JAM_DIST_THRESHOLD },
        { "keep_for",       SUMO_ATTR_DELETE_DATA_AFTER_SECONDS },
        { "det_offset",     SUMO_ATTR_DET_OFFSET },

        { "vehno",          SUMO_ATTR_VEHNO },
        { "perc",           SUMO_ATTR_PERC },
        { "edge-id",        SUMO_ATTR_EDGE_ID },
        { "absolut",        SUMO_ATTR_ABSOLUT },
        { "mult",           SUMO_ATTR_MULT },
        { "add",            SUMO_ATTR_ADD },

        { "dist",           SUMO_ATTR_DIST },
        { "onEdges",        SUMO_ATTR_ONEDGES },
        { "seesEdges",      SUMO_ATTR_SEESEDGES },
        { "periodBegin",    SUMO_ATTR_PERIODBEGIN },
        { "viewDist",       SUMO_ATTR_VIEWDIST },
        { "oneShot",        SUMO_ATTR_ONESHOT },

        { "host",			SUMO_ATTR_HOST },
        { "port",			SUMO_ATTR_PORT },
        { "protocol",		SUMO_ATTR_PROTOCOL },
        { "wautID",		    SUMO_ATTR_WAUT_ID },
        { "junctionID",     SUMO_ATTR_JUNCTION_ID },
        { "procedure",	    SUMO_ATTR_PROCEDURE },
        { "synchron",	    SUMO_ATTR_SYNCHRON },
        { "refTime",	    SUMO_ATTR_REF_TIME },
        { "startProg",	    SUMO_ATTR_START_PROG },
        { "off",            SUMO_ATTR_OFF },
        { "friendly_pos",   SUMO_ATTR_FRIENDLY_POS },
        { "uncontrolled",   SUMO_ATTR_UNCONTROLLED },
        { "vehtype",        SUMO_ATTR_VEHTYPE },
        { "pass",           SUMO_ATTR_PASS },
        { "forceLength",    SUMO_ATTR_FORCE_LENGTH },
        { "bus_stop",       SUMO_ATTR_BUS_STOP },
        { "lines",          SUMO_ATTR_LINES },
        { "vclasses",       SUMO_ATTR_VCLASSES },
        { "value",          SUMO_ATTR_VALUE },
        { "prohibitor",     SUMO_ATTR_PROHIBITOR },
        { "prohibited",     SUMO_ATTR_PROHIBITED },
        { "allow",          SUMO_ATTR_ALLOW },
        { "disallow",       SUMO_ATTR_DISALLOW },
        { "controlled_inner", SUMO_ATTR_CONTROLLED_INNER },
        { "v",              SUMO_ATTR_V },
        { "q",              SUMO_ATTR_Q },
        { "rfile",          SUMO_ATTR_RFILE },

        { "entering",       SUMO_ATTR_ENTERING },
        
        { "",               SUMO_ATTR_NOTHING }
    };



/****************************************************************************/

