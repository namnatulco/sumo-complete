#ifndef NBNodeShapeComputer_h
#define NBNodeShapeComputer_h

#include <utils/geom/Position2DVector.h>

class NBNode;
class NBEdge;

class NBNodeShapeComputer {
public:
    NBNodeShapeComputer(const NBNode &node);
    ~NBNodeShapeComputer();
    Position2DVector compute();

private:
    void addInternalGeometry();
    Position2DVector computeRealNodeShape();
    Position2DVector computeJoinSplitNodeShape();
    Position2DVector computeContinuationNodeShape();

    void addCCWPoint(Position2DVector &poly, NBEdge *e, double offset,
        double width);
    void addCWPoint(Position2DVector &poly, NBEdge *e, double offset,
        double width);

    bool isSimpleContinuation(const NBNode &n) const;

    struct NeighborCrossDesc {
        bool myUsedOppositeDirection; // !!!
        bool myAmValid;
        double myCrossingPosition;
        double myCrossingAngle;

    };

    typedef std::pair<NeighborCrossDesc, NeighborCrossDesc> EdgeCrossDef;

    typedef std::vector<EdgeCrossDef> EdgeCrossDefVector;

    EdgeCrossDef getEdgeNeighborCrossings(const EdgeVector::const_iterator &i);
    NeighborCrossDesc getNeighbor2Use(const EdgeCrossDefVector::iterator &j);
    NeighborCrossDesc buildCrossingDescription(
        const EdgeVector::const_iterator &i,
        const EdgeVector::const_iterator &oi,
        const Position2DVector &own_bound,
        const Position2DVector &opp_bound) const;


private:
    const NBNode &myNode;

};

#endif
