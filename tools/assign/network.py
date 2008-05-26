"""
@file    network.py
@author  Yun-Pang.Wang@dlr.de
@date    2007-12-25
@version $Id$

This script is to retrive the network data, the district data and the vehicle data, generated by SUMO, from the respective XML files.
Besides, the class 'Net' is also definded here.

Copyright (C) 2008 DLR/TS, Germany
All rights reserved
"""

import os, random, string, sys, datetime, math, operator
from xml.sax import saxutils, make_parser, handler
from elements import Predecessor, Vertex, Edge, Vehicle, Path, TLJunction, Signalphase

# Net class stores the network (vertex and edge collection). 
# Moreover, the methods for finding k shortest paths and for generating vehicular releasing times
#  are also in this class.

class Net:
    def __init__(self):
        self._vertices = []
        self._edges = {}
        self._vehicles = []
        self._startVertices = []
        self._endVertices = []
        self._paths = {}
        self._allvehicles = {}
        self._assignments = {}
        self._junctions = {}
        self._detectedLinkCounts = 0
        self._flowVarianceMatrices = {}
        
    def newVertex(self):
        v = Vertex(len(self._vertices))
        self._vertices.append(v)
        return v

    def addVehicle(self, label):
        t = Vehicle(label)
        self._vehicles.append(t)
        return t
    
    def getEdge(self, edgeLabel):
        return self._edges[edgeLabel]

    def addEdge(self, edgeObj):
        edgeObj.source.outEdges.add(edgeObj)
        edgeObj.target.inEdges.add(edgeObj)
        if edgeObj.kind == "real":
            self._edges[edgeObj.label] = edgeObj

    def addIsolatedRealEdge(self, edgeLabel):
        self.addEdge(Edge(edgeLabel, self.newVertex(), self.newVertex(),
                          "real"))
                                                   
    def initialPathSet(self):
        for startVertex in self._startVertices:
            self._paths[startVertex] = {}
            for endVertex in self._endVertices:
                self._paths[startVertex][endVertex] = []
    
    def cleanPathSet(self):
        for startVertex in self._startVertices:
            for endVertex in self._endVertices:
                self._paths[startVertex][endVertex] = []
                
    def addAssignment(self, assignObj):
        self._assignments[assignObj.label] = assignObj
     
    def addTLJunctions(self, junctionObj):
        self._junctions[junctionObj.label] = junctionObj
        
    def getJunction(self, junctionlabel):
        return self._junctions[junctionlabel]
        
    def addFlowVarianceMatrix(self, varianceObj):
        self._flowVarianceMatrices[varianceObj.label] = varianceObj
    
    def removeUTurnEdge(self, edge):
        outEdge = edge
        if outEdge.uturn != None:
            for link in self._edges.itervalues():
                if str(link.source) == str(outEdge.target) and str(link.target) == str(outEdge.source):
                    for edge1 in outEdge.target.outEdges:
                        for edge2 in link.source.inEdges:
                            if edge1 == edge2:
                                uTurnEdge = edge1

                    outEdge.target.outEdges.discard(uTurnEdge)
                    link.source.inEdges.discard(uTurnEdge)
                    
#    find the k shortest paths for each OD pair. The "k" is defined by users.
    def calcKPaths(self, verbose, newRoutes, KPaths, startVertices, endVertices, matrixPshort):
        if verbose:
            foutkpath = file('kpaths.xml', 'w')
            print >> foutkpath, """<?xml version="1.0"?>
<!-- generated on %s by $Id$ -->
<routes>"""
        start = -1
      
        for startVertex in startVertices:
            start += 1
            end = -1
            for vertex in self._vertices:
                vertex.preds = []
                vertex.wasUpdated = False
            startVertex.preds.append(Predecessor(None, None, 0))
            updatedVertices = [startVertex]

            while len(updatedVertices) > 0:
                vertex = updatedVertices.pop(0)
                vertex.wasUpdated = False
                for edge in vertex.outEdges:
                    if edge.target != startVertex and edge.target.update(KPaths, edge):
                        updatedVertices.append(edge.target)
    
            for endVertex in endVertices:
                end += 1
                ODPaths = self._paths[startVertex][endVertex]
                if str(startVertex) != str(endVertex) and matrixPshort[start][end] != 0.:
                    for startPred in endVertex.preds:
                        temppath = []
                        temppathcost = 0.
                        pred = startPred
                        vertex = endVertex
                        while vertex != startVertex:
                            if pred.edge.kind == "real":
                                temppath.append(pred.edge)
                            vertex = pred.edge.source
                            pred = pred.pred
                        
                        for edge in temppath:
                            temppathcost += edge.freeflowtime
                        
                        if len(ODPaths) > 0:
                            minpath = min(ODPaths, key=operator.attrgetter('freepathtime'))
                            if minpath.freepathtime*1.4 < temppathcost/3600.:
                                break
                            else:
                                newpath = Path()
                                newpath.usedcounts += 1
                                ODPaths.append(newpath)
                                newpath.source = startVertex
                                newpath.target = endVertex
                                temppath.reverse()
                                newpath.Edges = temppath
                                newpath.freepathtime = temppathcost/3600.
                                newpath.actpathtime = newpath.freepathtime
                                newRoutes += 1
                                if verbose:
                                    foutkpath.write('    <path id="%s" source="%s" target="%s" pathcost="%s">\n' %(newpath.label, newpath.source, newpath.target, newpath.actpathtime))  
                                    foutkpath.write('        <route>')
                                    for edge in newpath.Edges[1:-1]:
                                        foutkpath.write('%s ' %edge.label)
                                    foutkpath.write('</route>\n')
                                    foutkpath.write('    </path>\n')
                        else:
                            newpath = Path()
                            newpath.usedcounts += 1
                            ODPaths.append(newpath)
                            newpath.source = startVertex
                            newpath.target = endVertex
                            temppath.reverse()                           
                            newpath.Edges = temppath
                            newpath.freepathtime = temppathcost/3600.
                            newpath.actpathtime = newpath.freepathtime
                            newRoutes += 1     
                            if verbose:
                                foutkpath.write('    <path id="%s", source="%s", target="%s", path cost="%s">\n' %(newpath.label, newpath.source, newpath.target, newpath.actpathtime))  
                                foutkpath.write('        <route>')
                                for edge in newpath.Edges[1:-1]:
                                    foutkpath.write('%s ' %edge.label)
                                foutkpath.write('</route>\n')
                                foutkpath.write('    </path>\n')     
        if verbose:
            foutkpath.write('</routes>\n')
            foutkpath.close()
            
        return newRoutes

    def vehRelease(self, verbose, Parcontrol, departtime, CurrentMatrixSum):
        if verbose:
            print 'RandomVehRelease:', Parcontrol[(len(Parcontrol)-3)]
            print 'CurrentMatrixSum:', CurrentMatrixSum
        # generate the departure time for each vehicle uniform randomly
        if int(Parcontrol[(len(Parcontrol)-3)]) == 0:
            for veh in self._vehicles:                                                       
                if veh.depart == 0:
                    veh.depart = random.randint(departtime, departtime + 3600)
        else:
        # generate the departure time for each vehicle poisson randomly (based on a hourly matrix)
            random.shuffle(net._vehicles)
            beta = float(3600. / CurrentMatrixSum)
            releasetime = departtime
            for veh in self._vehicles:
                if veh.depart == 0.:
                    probability = random.random()
                    releasetime = releasetime + (-beta * (math.log(1.0 - probability)))
                    veh.depart = releasetime      
                        
    def printNet(self, foutnet):
        foutnet.write('Name\t Kind\t FrNode\t ToNode\t length\t MaxSpeed\t Lanes\t CR-Curve\t EstCap.\t Free-Flow TT\t Weight\t Connection\n')
        for edgeName, edgeObj in self._edges.iteritems():
            foutnet.write('%s\t %s\t %s\t %s\t %s\t %s\t %s\t %s\t %s\t %s\t %s\t %d\n' 
            %(edgeName, edgeObj.kind, edgeObj.source, edgeObj.target, edgeObj.length, 
              edgeObj.maxspeed, edgeObj.numberlane, edgeObj.CRcurve, edgeObj.estcapacity, edgeObj.freeflowtime, edgeObj.weight, edgeObj.connection))
     
# The class is for parsing the XML input file (network file). The data parsed is written into the net.
class NetworkReader(handler.ContentHandler):
    def __init__(self, net):
        self._net = net
        self._edge = ''
        self._maxSpeed = 0
        self._laneNumber = 0
        self._length = 0
        self._edgeObj = None
        self._junctionObj = None
        self._phaseObj = None
        self._chars = ''
        self._counter = 0

    def startElement(self, name, attrs):
        self._chars = ''
        if name == 'edge' and (not attrs.has_key('function') or attrs['function'] != 'internal'):
            self._edge = attrs['id']
            self._edgeObj = self._net.getEdge(self._edge)
            self._edgeObj.source.label = attrs['from']
            self._edgeObj.target.label = attrs['to']
            self._maxSpeed = 0
            self._laneNumber = 0
            self._length = 0
        elif name == 'tl-logic':
            self._junctionObj = TLJunction()
            self._counter = 0
        elif name == 'phase':
            self._newphase = Signalphase(float(attrs['duration']), attrs['phase'], attrs['brake'], attrs['yellow'])
            self._junctionObj.phases.append(self._newphase)
            self._counter += 1
            self._newphase.label = self._counter
        elif name == 'succ':
            self._edge = attrs['edge']
            self._edgeObj = self._net.getEdge(self._edge)
            self._edgeObj.junction = attrs['junction']
        elif name == 'succlane':
            if attrs.has_key('tl'):
                self._edgeObj.junction = attrs['tl']
                self._edgeObj.junctiontype = 'signalized'
                if attrs['dir'] == "r":
                    self._edgeObj.rightturn = attrs['linkno']
                elif attrs['dir'] == "s": 
                    self._edgeObj.straight = attrs['linkno']
                elif attrs['dir'] == "l": 
                    self._edgeObj.leftturn = attrs['linkno']
                elif attrs['dir'] == "u": 
                    self._edgeObj.uturn = attrs['linkno']
            else:
                self._edgeObj.junctiontype = 'prioritized'
                if attrs['dir'] == "r":
                    self._edgeObj.rightturn = attrs['state']
                elif attrs['dir'] == "s": 
                    self._edgeObj.straight = attrs['state']
                elif attrs['dir'] == "l": 
                    self._edgeObj.leftturn = attrs['state']
                elif attrs['dir'] == "t": 
                    self._edgeObj.uturn = attrs['state']
                    
        elif name == 'cedge' and self._edge != '':
             fromEdge = self._net.getEdge(self._edge)
             toEdge = self._net.getEdge(attrs['id'])
             newEdge = Edge(self._edge+"_"+attrs['id'], fromEdge.target, toEdge.source)
             self._net.addEdge(newEdge)
             fromEdge.finalizer = attrs['id']
        elif name == 'lane' and self._edge != '':
            self._maxSpeed = max(self._maxSpeed, float(attrs['maxspeed']))
            self._laneNumber = self._laneNumber + 1
            self._length = float(attrs['length'])
      
    def characters(self, content):
        self._chars += content

    def endElement(self, name):
        if name == 'edges':
            for edge in self._chars.split():
                self._net.addIsolatedRealEdge(edge)
            self._chars = ''
        elif name == 'edge':
            self._edgeObj.init(self._maxSpeed, self._length, self._laneNumber)
            self._edge = ''
        elif name == 'key':
            if self._junctionObj:
                self._junctionObj.label = self._chars
                self._net.addTLJunctions(self._junctionObj)
                self._chars = ''
        elif name == 'phaseno':
            self._junctionObj.phaseNum = int(self._chars)
            self._chars = ''
        elif name == 'tl-logic':
            self._junctionObj = None

# The class is for parsing the XML input file (districts). The data parsed is written into the net.
class DistrictsReader(handler.ContentHandler):
    def __init__(self, net):
        self._net = net
        self._StartDTIn = None
        self._StartDTOut = None
        self.I = 100

    def startElement(self, name, attrs):
        if name == 'district':
            self._StartDTIn = self._net.newVertex()
            self._StartDTIn.label = attrs['id']
            self._StartDTOut = self._net.newVertex()
            self._StartDTOut.label = self._StartDTIn.label
            self._net._startVertices.append(self._StartDTIn)
            self._net._endVertices.append(self._StartDTOut)
        elif name == 'dsink':
            sourcelink = self._net.getEdge(attrs['id'])
            self.I += 1
            conlink = self._StartDTOut.label + str(self.I)
            newEdge = Edge(conlink, sourcelink.target, self._StartDTOut, "real")
            self._net.addEdge(newEdge)
            newEdge.weight = attrs['weight']
            newEdge.connection = 1
        elif name == 'dsource':
            sinklink = self._net.getEdge(attrs['id'])
            self.I += 1
            conlink = self._StartDTIn.label + str(self.I)
            newEdge = Edge(conlink, self._StartDTIn, sinklink.source, "real")
            self._net.addEdge(newEdge)
            newEdge.weight = attrs['weight']
            newEdge.connection = 2

# The class is for parsing the XML input file (vehicle information). This class is used in the networkStatistics.py for
# calculating the gloabal network performances, e.g. avg. travel time and avg. travel speed.
class VehInformationReader(handler.ContentHandler):
    def __init__(self, vehList):
        self._vehList = vehList
        self._Vehicle = None
        self._routeString = ''
            
    def startElement(self, name, attrs):
        if name == 'tripinfo':
            self._Vehicle = Vehicle(attrs['id'])
            self._Vehicle.traveltime = float(attrs['duration'])
            self._Vehicle.travellength = float(attrs['routeLength'])
            self._Vehicle.departdelay = float(attrs['departDelay'])
            self._Vehicle.waittime = float(attrs['departDelay']) + float(attrs['waitSteps']) 
            self._vehList.append(self._Vehicle)

## This class is for parsing the additional/updated information about singal timing plans
class ExtraSignalInformationReader(handler.ContentHandler):
    def __init__(self, net):
        self._net = net
        self._junctionlabel = None
        self._phaseObj = None
        self._chars = ''
        self._counter = 0

    def startElement(self, name, attrs):
        self._chars = ''
        if name == 'tl-logic':
            self._counter = 0
        elif name == 'phase':
            self._counter += 1
            junction = self._net.getJunction(self._junctionlabel)
            junction.phaseNum = self._counter
            for phase in junction.phases[:]:
                if phase.label == str(self._counter):
                    phase.duration = float(attrs['duration'])
                    phase.green = attrs['phase'][::-1]
                    phase.brake = attrs['brake'][::-1]
                    phase.yellow= attrs['yellow'][::-1]
      
    def characters(self, content):
        self._chars += content

    def endElement(self, name):
        if name == 'key':
            self._junctionlabel = self._chars
            self._chars = ''
        elif name == 'tl-logic':
            self._junctionObj = None
            
class DetectedFlowsReader(handler.ContentHandler):
    def __init__(self, net):
        self._net = net
        self._junctionlabel = None
        self._phaseObj = None
        self._chars = ''
        self._counter = 0