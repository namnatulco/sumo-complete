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

import os, random, string, sys, datetime, math
from xml.sax import saxutils, make_parser, handler
from elements import Predecessor, Vertex, Edge, Vehicle, Path

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
        self._duavehicles = {}
        self._oneshotvehicles = {}
        self._incremvehicles = []
        self._clogitvehicles = []
        self._lohsevehicles = []
        self._assignments = {}
        
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
    
    def addAssignment(self, assignObj):
        self._assignments[assignObj.label] = assignObj
        
#    find the k shortest paths for each OD pair. The "k" is defined by users.
    def calcPaths(self, verbose, newRoutes, KPaths, startVertices, endVertices, matrixPshort):
        if verbose:
            foutkpath = file('kpaths.txt', 'w')
        start = -1
      
        for startVertex in startVertices:
            start += 1
            end = -1
            for vertex in self._vertices:
                vertex.preds = []
                vertex.wasUpdated = False
            startVertex.preds.append(Predecessor(None, None, 0))
            updatedVertices = [startVertex]
            if verbose:
                print 'updatedVertices:', updatedVertices

            while len(updatedVertices) > 0:
                vertex = updatedVertices.pop(0)
                vertex.wasUpdated = False
                for edge in vertex.outEdges:
                    if edge.target != startVertex and edge.target.update(KPaths, edge):
                        updatedVertices.append(edge.target)
    
            for endVertex in endVertices:
                end += 1
                if verbose:
                    print 'Number of the new Routes:', newRoutes
                if str(startVertex) != str(endVertex) and matrixPshort[start][end] != 0.:
                    for startPred in endVertex.preds:
                        newpath = Path()
                        self._paths[startVertex][endVertex].append(newpath)
                        newpath.source = startVertex
                        newpath.target = endVertex
                        
                        pred = startPred
                        vertex = endVertex
                        while vertex != startVertex:
                            if pred.edge.kind == "real":
                                newpath.Edges.append(pred.edge)
                            vertex = pred.edge.source
                            pred = pred.pred

                        newpath.Edges.reverse()    
                        if verbose:
                            foutkpath.write('\npathID:%s, source:%s, target:%s, Edges:' %(newpath.label, newpath.source, newpath.target))  
    
                        for edge in newpath.Edges:
                            if verbose:
                                foutkpath.write('%s, ' %(edge.label))
                            newpath.freepathtime += edge.freeflowtime
                            
                        newpath.actpathtime = newpath.freepathtime
                        if verbose:
                            foutkpath.write('Path cost:%s' %newpath.actpathtime) 
                    newRoutes += 1
        if verbose:
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
            print 'beta:', beta
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
        self._edgeString = ''
        self._edge = ''
        self._maxSpeed = 0
        self._laneNumber = 0
        self._length = 0
        self._edgeObj = None

    def startElement(self, name, attrs):
        if name == 'edges':
            self._edgeString = ' '
        elif name == 'edge' and (not attrs.has_key('function') or attrs['function'] != 'internal'):
            self._edge = attrs['id']
            self._edgeObj = self._net.getEdge(self._edge)
            self._edgeObj.source.label = attrs['from']
            self._edgeObj.target.label = attrs['to']
            self._maxSpeed = 0
            self._laneNumber = 0
            self._length = 0
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
        if self._edgeString != '':
            self._edgeString += content

    def endElement(self, name):
        if name == 'edges':
            for edge in self._edgeString.split():
                self._net.addIsolatedRealEdge(edge)
            self._edgeString = ''
        elif name == 'edge':
            self._edgeObj.init(self._maxSpeed, self._length, self._laneNumber)
            self._edge = ''

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
            self._Vehicle.waittime = float(attrs['departDelay']) + float(attrs['waitSteps']) 
            self._vehList.append(self._Vehicle)
