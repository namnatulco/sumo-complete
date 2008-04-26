#!/usr/bin/env python
"""
@file    networkStatisticsWithSgT.py
@author  Yun-Pang.Wang@dlr.de
@date    2007-02-27
@version $Id$

This script is to calculate the global performance indices according to the SUMO-based simulation results.
Besides, this script is also to execute the significance test for evaluating the results from different assignment methods.
The t test and the Kruskal-Wallis test are available in this script. 
If not specified, the Kruskal-Wallis test will be applied with the assumption that data are not normally distributed.

The analyzed parameters include:
- travel length
- travel time
- travel speed
- stop time

Copyright (C) 2008 DLR/TS, Germany
All rights reserved
"""

import os, string, sys, datetime, random, math

from xml.sax import saxutils, make_parser, handler
from optparse import OptionParser
from elements import Vertex, Edge, Vehicle, Assign, T_Value, H_Value
from network import Net, NetworkReader, DistrictsReader, VehInformationReader
from calStatistics import getBasicStats, doTTestForAvg, doKruskalWallisTest
from outputs import getStatisticsOutput, getSignificanceTestOutput


optParser = OptionParser()
optParser.add_option("-t", "--tripinform-file", dest="vehfile",
                     help="read vehicle information generated by the DUA assignment from FILE (mandatory)", metavar="FILE")
optParser.add_option("-o", "--output-file", dest="outputfile", default="Global_MOE.txt",
                     help="write output to FILE", metavar="FILE")
optParser.add_option("-g", "--SGToutput-file", dest="sgtoutputfile", default="significanceTest.txt",
                     help="write output to FILE", metavar="FILE")
optParser.add_option("-v", "--verbose", action="store_true", dest="verbose",
                     default=False, help="tell me what you are doing")
                     
                                       
(options, args) = optParser.parse_args()

if not options.vehfile:
    optParser.print_help()
    sys.exit()
parser = make_parser()

net = Net()

for filename in options.vehfile.split(","):
    net._allvehicles[filename] = []
    parser.setContentHandler(VehInformationReader(net._allvehicles[filename]))
    parser.parse(filename)

# Vehicles from dua, incremental, clogit and oneshot are in included in the allvehlist.
allvehlist = []
duavehlist = []
oneshotvehlist = []
# The results of the t test are stored in the tValueAvg.
tValueAvg = {}
# The resultes of the Kruskal-Wallis test are stored in the hValues.
hValues = []

# intitalization
allgroups = 0.
duagroups = 0.
oneshotgroups = 0.
allmethodlabel = ''
combilabel = ''

# calculate/read the basic statistics
for method, vehicles in net._allvehicles.iteritems():
    allvehlist, duavehlist, oneshotvehlist, duagroups, oneshotgroups = getBasicStats(net, options.verbose, method, vehicles, allvehlist, duavehlist, oneshotvehlist, duagroups, oneshotgroups)
    if method == "tripinfo_oneshot_1800.xml" or method == "tripinfo_successive.xml" or method == "tripinfo_dua_24.xml" or method == "tripinfo_clogit.xml":
        allmethodlabel = allmethodlabel + method + "_"
        allgroups += 1

getStatisticsOutput(net, options.outputfile)
print 'The calculation of network statistics is done!'              
   
# begin the significance test for the observations with a normal distribution
print 'begin the t test!'
for A in net._assignments.itervalues():
    tValueAvg[A] = {}
doTTestForAvg(options.verbose, tValueAvg, list(net._assignments.itervalues()))
print 'The t test is done!'        

# The Kruskal-Wallis test is applied for the data, not drawn from a normally distributed population.
groups = 2
values = list(net._allvehicles.iteritems())
for num, A in enumerate(values):
    for B in values[num+1: ]:
        combilabel = ''
        combivehlist = []
        combilabel = A[0] + '_' + B[0]
        print 'Test for:', combilabel
        for veh in A[1]:
            combivehlist.append(veh)
        for veh in B[1]:
            combivehlist.append(veh)
            
        doKruskalWallisTest(options.verbose, groups, combivehlist, net._assignments, combilabel, hValues)
    
doKruskalWallisTest(options.verbose, allgroups, allvehlist, net._assignments, allmethodlabel, hValues)
print 'Test for:', allmethodlabel
doKruskalWallisTest(options.verbose, oneshotgroups, oneshotvehlist, net._assignments, "alloneshots", hValues)
print 'Test for: alloneshots'
print 'groups:', oneshotgroups
doKruskalWallisTest(options.verbose, duagroups, duavehlist, net._assignments, "duas", hValues)
print 'Test for: duas'
print 'groups:', duagroups

getSignificanceTestOutput(net, tValueAvg, hValues)
print 'The Significance test is done!'
