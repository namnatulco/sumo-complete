#include <utils/xml/GenericSAX2Handler.h>
#include <utils/xml/AttributesHandler.h>
#include "SUMOXMLDefinitions.h"

size_t noSumoTags = 35;

GenericSAX2Handler::Tag sumotags[35] =
{
    /* 00 */  { "simulation",       SUMO_TAG_SIMULATION },
    /* 01 */  { "edge",             SUMO_TAG_EDGE },
    /* 02 */  { "lane",             SUMO_TAG_LANE },
    /* 03 */  { "lanes",            SUMO_TAG_LANES },
    /* 04 */  { "cedge",            SUMO_TAG_CEDGE },
    /* 05 */  { "junction",         SUMO_TAG_JUNCTION },
    /* 06 */  { "inlanes",          SUMO_TAG_INLANES },
    /* 07 */  { "detector",         SUMO_TAG_DETECTOR },
    /* 08 */  { "vehicle",          SUMO_TAG_VEHICLE },
    /* 09 */  { "vtype",            SUMO_TAG_VTYPE },
    /* 10 */  { "route",            SUMO_TAG_ROUTE },
    /* 11 */  { "succ",             SUMO_TAG_SUCC },
    /* 12 */  { "succlane",         SUMO_TAG_SUCCLANE },
    /* 13 */  { "key",              SUMO_TAG_KEY },
    /* 14 */  { "junctionlogic",    SUMO_TAG_JUNCTIONLOGIC },
    /* 15 */  { "requestsize",      SUMO_TAG_REQUESTSIZE },
    /* 16 */  { "responsesize",     SUMO_TAG_RESPONSESIZE },
    /* 17 */  { "lanenumber",       SUMO_TAG_LANENUMBER },
    /* 18 */  { "logicitem",        SUMO_TAG_LOGICITEM },
    /* 19 */  { "trafoitem",        SUMO_TAG_TRAFOITEM },
    /* 20 */  { "row-logic",        SUMO_TAG_ROWLOGIC },
    /* 21 */  { "source",           SUMO_TAG_SOURCE },
    /* 22 */  { "district",         SUMO_TAG_DISTRICT },
    /* 23 */  { "dsource",          SUMO_TAG_DSOURCE },
    /* 24 */  { "dsink",            SUMO_TAG_DSINK },
    /* 25 */  { "edges",            SUMO_TAG_EDGES },
    /* 26 */  { "nodecount",        SUMO_TAG_NODECOUNT },
    /* 27 */  { "tl-logic",         SUMO_TAG_TLLOGIC },
    /* 28 */  { "offset",           SUMO_TAG_OFFSET },
    /* 29 */  { "initstep",         SUMO_TAG_INITSTEP },
    /* 30 */  { "phaseno",          SUMO_TAG_PHASENO },
    /* 31 */  { "phase",            SUMO_TAG_PHASE },
    /* 32 */  { "edgepos",          SUMO_TAG_EDGEPOS },
    /* 33 */  { "routealt",         SUMO_TAG_ROUTEALT },
    /* 34 */  { "tripdef",          SUMO_TAG_TRIPDEF }
};

size_t noSumoAttrs = 30;

AttributesHandler::Attr sumoattrs[30] =
{
    /* 00 */ { "id",            SUMO_ATTR_ID },
    /* 01 */ { "name",          SUMO_ATTR_NAME },
    /* 02 */ { "type",          SUMO_ATTR_TYPE },
    /* 03 */ { "priority",      SUMO_ATTR_PRIORITY },
    /* 04 */ { "nolanes",       SUMO_ATTR_NOLANES },
    /* 05 */ { "speed",         SUMO_ATTR_SPEED },
    /* 06 */ { "length",        SUMO_ATTR_LENGTH },
    /* 07 */ { "fromnode",      SUMO_ATTR_FROMNODE },
    /* 08 */ { "tonode",        SUMO_ATTR_TONODE },
    /* 09 */ { "xfrom",         SUMO_ATTR_XFROM },
    /* 10 */ { "yfrom",         SUMO_ATTR_YFROM },
    /* 11 */ { "xto",           SUMO_ATTR_XTO },
    /* 12 */ { "yto",           SUMO_ATTR_YTO },
    /* 13 */ { "x",             SUMO_ATTR_X },
    /* 14 */ { "y",             SUMO_ATTR_Y },
    /* 15 */ { "key",           SUMO_ATTR_KEY },
    /* 16 */ { "weight",        SUMO_ATTR_WEIGHT },
    /* 17 */ { "depart",        SUMO_ATTR_DEPART },
    /* 18 */ { "route",         SUMO_ATTR_ROUTE },
    /* 19 */ { "maxspeed",      SUMO_ATTR_MAXSPEED },
    /* 20 */ { "accel",         SUMO_ATTR_ACCEL },
    /* 21 */ { "decel",         SUMO_ATTR_DECEL },
    /* 22 */ { "sigma",         SUMO_ATTR_SIGMA },
    /* 23 */ { "last",          SUMO_ATTR_LAST },
    /* 24 */ { "cost",          SUMO_ATTR_COST },
    /* 25 */ { "propability",   SUMO_ATTR_PROP },
    /* 26 */ { "pos",           SUMO_ATTR_POS },
    /* 27 */ { "lane",          SUMO_ATTR_LANE },
    /* 28 */ { "from",          SUMO_ATTR_FROM },
    /* 29 */ { "to",            SUMO_ATTR_TO }
};
