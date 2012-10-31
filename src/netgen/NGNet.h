/****************************************************************************/
/// @file    NGNet.h
/// @author  Markus Hartinger
/// @author  Daniel Krajzewicz
/// @author  Michael Behrisch
/// @date    Mar, 2003
/// @version $Id$
///
// The class storing the generated network
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.sourceforge.net/
// Copyright (C) 2001-2012 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This file is part of SUMO.
//   SUMO is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/
#ifndef NGNet_h
#define NGNet_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include "NGEdge.h"
#include "NGNode.h"


// ===========================================================================
// class declarations
// ===========================================================================
class NBNetBuilder;


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class NGNet
 * @brief The class storing the generated network
 *
 * An instance of this class stores both the edges and the nodes build during
 *  the generation of a network (using any type of generation algorithm).
 * These instances are later transformed into netbuild-structures using toNB().
 */
class NGNet {
public:
    /// @brief Constructor
    NGNet(NBNetBuilder& nb);


    /// @brief Destructor
    ~NGNet();


    /** @brief Returns the node at the given position
     *
     * Searches for a node with the given position within myNodeList.
     *  Returns the matching node, if one exists, or 0 otherwise.
     *
     * @param[in] xPos The x-position of the searched node
     * @param[in] yPos The y-position of the searched node
     * @return The node with the given position, or 0 if no such node exists
     */
    NGNode* findNode(int xPos, int yPos);


    /** @brief Returns the next free id
     *
     * Uses the value of myLastID to return a new (numeric) id. Increases
     *  myLastID.
     *
     * @return A new, unique numerical id
     */
    std::string getNextFreeID();


    /** @brief Returns the x-position resulting from the given radius and angle
     *
     * @param[in] radius The radius of the circle
     * @param[in] phi The angle the position is located at
     * @return The x-position at the described circle at angle phi
     */
    SUMOReal radialToX(SUMOReal radius, SUMOReal phi);


    /** @brief Returns the y-position resulting from the given radius and angle
     *
     * @param[in] radius The radius of the circle
     * @param[in] phi The angle the position is located at
     * @return The y-position at the described circle at angle phi
     */
    SUMOReal radialToY(SUMOReal radius, SUMOReal phi);


    /** @brief Creates a grid network
     *
     * Performs a souble-loop over numX, then numY. Builds NGNodes
     *  at the according positions and connects them using NGNet::connect.
     * Stores both the nodes and the edges within the internal container.
     *
     * The nodes get an id using &lt;RUNNING_X&gt;/&lt;RUNNING_Y&gt;. The ids
     *  of the links are set in NGNet::connect.
     *
     * @param[in] numX The number of nodes in x-direction
     * @param[in] numY The number of nodes in y-direction
     * @param[in] spaceX The space between nodes in x-direction
     * @param[in] spaceY The space between nodes in y-direction
     * @param[in] attachLength The length of streets attached at the border
     * @see NGNet::connect
     */
    void createChequerBoard(int numX, int numY, SUMOReal spaceX, SUMOReal spaceY, SUMOReal attachLength);


    /** @brief Creates a spider network
     *
     * Creates a spider web by going through all arms and then all circles in a loop.
     *  Builds the NGNodes at the positions obtained using radialToX and radialToY and
     *  connects them using NGNet::connect. Builds optionally a center node, and connects
     *  it, too.
     *
     * The nodes get an id using &lt;RUNNING_ARM_NUMBER&gt;/&lt;RUNNING_CIRCLE_NUMBER&gt;.
     *  The ids of the links are set in NGNet::connect.
     *
     * @param[in] numRadDiv The number of arms to build
     * @param[in] numCircles The number of circles to build
     * @param[in] spaceRad The distance between the circles
     * @param[in] hasCenter Information whether a center node shall be built
     * @see NGNet::connect
     * @todo consolidate the name of the center node
     */
    void createSpiderWeb(int numRadDiv, int numCircles, SUMOReal spaceRad, bool hasCenter);


    /** @brief Converts the stored network into its netbuilder-representation
     *
     * Goes through all stored nodes, first, converts them into their netbuilder
     *  representations using NGNode::buildNBNode, and stores the built NBNodes into the
     *  net builder myNetBuilder.
     *
     * Then, the method goes through all edges, converts them into their netbuilder
     *  representations using NGEdge::buildNBEdge, and stores the built NBEdges into the
     *  net builder myNetBuilder.
     *
     * If one of the nodes is controlled by a tls and the built logic could not be added
     *  to net builder's storage, a ProcessError is thrown. This in fact may only happen
     *  when two same ids occur, what is not possible.
     *
     * @exception ProcessError If a built tls logic could not be added (should never happen)
     * @todo Check whether throwing an exception is really necessary, here
     */
    void toNB() const;


    /** @brief Adds the given node to the network
     *
     * The node is added to myNodeList.
     *
     * @param[in] node The node to add
     */
    void add(NGNode* node);


    /** @brief Adds the given edge to the network
     *
     * The edge is added to myEdgeList.
     *
     * @param[in] edge The edge to add
     */
    void add(NGEdge* edge);


    /** @brief Returns the number of stored nodes
     *
     * @return The number of stored nodes
     */
    size_t nodeNo() const;


private:
    /** @brief Connects both nodes with two edges, one for each direction
     *
     * Builds one link for each direction and appends the links to myEdgeList.
     * The name of a link is as following: &lt;FROM_NODE_ID&gt;to&lt;TO_NODE_ID&gt;.
     *
     * @param[in] node1 The first node to connect
     * @param[in] node2 The second node to connect
     */
    void connect(NGNode* node1, NGNode* node2);


private:
    /// @brief The last ID given to node or link
    int myLastID;

    /// @brief The builder used to build NB*-structures
    NBNetBuilder& myNetBuilder;

    /// @brief The list of nodes
    NGNodeList myNodeList;

    /// @brief The list of links
    NGEdgeList myEdgeList;

private:
    /// @brief Invalidated copy constructor.
    NGNet(const NGNet&);

    /// @brief Invalidated assignment operator.
    NGNet& operator=(const NGNet&);

};


#endif

/****************************************************************************/
