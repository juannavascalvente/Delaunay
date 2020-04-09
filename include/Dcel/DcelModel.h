#ifndef DELAUNAY_DCELMODEL_H
#define DELAUNAY_DCELMODEL_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Edge.h"
#include "Face.h"
#include "Vertex.h"


/***********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#define 	EXTERNAL_FACE       	(0)
#define     NO_UPDATE           	(-1)


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class DcelModel
{
protected:
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    vector<Vertex>	vVertex;
    vector<Edge> 	vEdges;
    vector<Face> 	vFaces;

    friend class DcelWriter;

public:
    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
    DcelModel() = default;
    explicit DcelModel(vector<Point<TYPE>> &v)
    {
        for (auto point : v)
        {
            Vertex vertex(INVALID, point);
            vVertex.push_back(vertex);
        }
    };
    ~DcelModel() = default;
    DcelModel & operator=(const DcelModel &d) = default;

    Point<TYPE> *getRefPoint(int index) { return this->vVertex.at(index).getRefPoint(); };
    Edge *getRefEdge(int index) { return &this->vEdges.at(index); };
    Face *getRefFace(int index) { return &this->vFaces.at(index); };

    size_t getNumVertex() const { return vVertex.size(); };
    size_t getNumEdges()  const { return vEdges.size(); };
    size_t getNumFaces()  const { return vFaces.size(); };

    // Access to POINTS fields.
    int getPointEdge(int pointIndex) {return(this->vVertex.at(pointIndex).getOrigin());};

    // Access to EDGES fields.
    int getOrigin(int edgeIndex) {return(this->vEdges.at(edgeIndex).getOrigin());};
    int getTwin(int edgeIndex) {return(this->vEdges.at(edgeIndex).getTwin());};
    int getPrevious(int edgeIndex) {return(this->vEdges.at(edgeIndex).getPrevious());};
    int getNext(int edgeIndex) {return(this->vEdges.at(edgeIndex).getNext());};
    int getFace(int edgeIndex) {return(this->vEdges.at(edgeIndex).getFace());};
    void setOrigin(int edgeIndex, int v) {this->vEdges.at(edgeIndex).setOrigin(v);};
    void setTwin(int edgeIndex, int v) {this->vEdges.at(edgeIndex).setTwin(v);};
    void setPrevious(int edgeIndex, int v) {this->vEdges.at(edgeIndex).setPrevious(v);};
    void setNext(int edgeIndex, int v) {this->vEdges.at(edgeIndex).setNext(v);};
    void setFace(int edgeIndex, int v) {this->vEdges.at(edgeIndex).setFace(v);};

    // Add / Delete functions.
    void addVertex(const Point<TYPE> *p, int edge);
    void addVertex(const Vertex &v);
    void updateVertex(int edge_ID, int index);
    void updateVertex(Point<TYPE> *p, int index);
    void swapVertex(int index1, int index2);
    void addEdge(int origin, int twin, int previou, int next, int face);
    void addEdge(Edge &edge);
    void updateEdge(int origin, int twin, int previous, int next, int face, int index);
    void addFace(int face);
    void addFace(Face &f);
    void updateFace(int edge_ID, int index);


    /**
     * @fn                  getEdgePoints
     * @brief               Gets origin and destination points of a given edge
     *
     * @param edgeIndex     (IN) Edge index to get
     * @param origin        (OUT) Origin edge point
     * @param dest          (OUT) Destination edge point
     */
    void getEdgePoints(int edgeIndex, Point<TYPE> &origin, Point<TYPE> &dest);

    // Access to FACES fields.
    int  getFaceEdge(int faceId) { return vFaces.at(faceId).getEdge();};
    void setFaceEdge(int faceIndex, int v) { vFaces.at(faceIndex).setEdge(v);};

    /**
     * @fn                  getFaceVertices
     * @brief               Get the face identifiers of the "faceIndex" face.
     *
     * @param faceIndex     (IN) Face whose vertex id are returned
     * @param ids           (OUT) Face vertex
     */
    void getFaceVertices(int faceIndex, int *ids);

    /**
     * @fn                  getFacePoints
     * @brief               Get points coordinates of a given face
     *
     * @param faceIndex     (IN)    Face whose points are returned
     * @param p             (OUT)   Point 1
     * @param q             (OUT)   Point 2
     * @param r             (OUT)   Point 3
     */
    void getFacePoints(int faceIndex, Point<TYPE> &p, Point<TYPE> &q, Point<TYPE> &r);

    /**
     * @fn              ==
     * @brief           Compares two dcel
     * @param other     Dcel to compare with
     * @return          true if both dcel are equal
     *                  false otherwise
     */
    bool operator==(const DcelModel& other) const;

    /**
     * @fn              reset
     * @brief           Resets dcel data
     */
    void reset(bool isResetPoints = true);
};


#endif //DELAUNAY_DCELMODEL_H
