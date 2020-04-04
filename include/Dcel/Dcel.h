/*
 * Dcel.h
 *
 *  Created on: Jun 29, 2016
 *      Author: jnavas
 */

#ifndef DCEL_H_
#define DCEL_H_

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <utility>
#include <vector>
#include "Edge.h"
#include "Face.h"
#include "Line.h"
#include "Set.h"
#include "Vertex.h"


/***********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#define     NO_UPDATE           	(-1)
#define 	EXTERNAL_FACE       	(0)


/***********************************************************************************************************************
* Class definition
***********************************************************************************************************************/
class Dcel
{
	/*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
	bool	created;
	bool	incremental;
	vector<Vertex>	vVertex;
	vector<Edge> 	vEdges;
	vector<Face> 	vFaces;

	/*******************************************************************************************************************
	* Private methods
	*******************************************************************************************************************/
	void quicksort(Vertex *origin, Vertex *list, int first, int last);
	int	 movePivot(Vertex *origin, Vertex *list, int first, int last);

    friend class DcelReader;
    friend class DcelWriter;

public:
	/*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
	Dcel() : created(false), incremental(false) {};
    explicit Dcel(vector<Point<TYPE>> &v);
	~Dcel() = default;

	/*------------------------------------------------------------------------
	  Public functions.
	------------------------------------------------------------------------*/
	// Add / Delete functions.
	void addVertex(const Point<TYPE> *p, int edge);
	void updateVertex(int edge_ID, int index);
	void updateVertex(Point<TYPE> *p, int index);
	void swapVertex(int index1, int index2);

	void addEdge(int origin, int	twin, int previou, int next, int face);
	void updateEdge(int origin, int twin, int previous, int next, int face, int index);

	void addFace(int face);
	void updateFace(int edge_ID, int index);

	// Check functions.
	bool isExternalEdge(int edgeIndex);
	bool hasNegativeVertex(int edgeID);
	int	 getCollinear(int pointIndex, int edgeID);

	//------------------------------------------------------------------------
	// Get/set functions.
	//------------------------------------------------------------------------
	Point<TYPE> *getRefPoint(int index) { return this->vVertex.at(index).getRefPoint(); };
	Edge *getRefEdge(int index) { return &this->vEdges.at(index); };
	Face *getRefFace(int index) { return &this->vFaces.at(index); };

	size_t getNVertex() const { return vEdges.size(); };
	size_t getNEdges()  const { return vEdges.size(); };
	size_t getNFaces()  const { return vFaces.size(); };

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
	void getEdgePoints(int edgeIndex, Point<TYPE> &origin, Point<TYPE> &dest);

	// Access to FACES fields.
	int  getFaceEdge(int faceId) { return vFaces.at(faceId).getEdge();};
	void setFaceEdge(int faceIndex, int v) { vFaces.at(faceIndex).setEdge(v);};
	bool imaginaryFace(int faceIndex);
	void getFaceVertices(int faceIndex, int *ids);
	void getFacePoints(int faceIndex, Point<TYPE> &p, Point<TYPE> &q, Point<TYPE> &r);

	// Set functions.
	void reset();

	// PENDING. Move to set? point?
	void sort();
	int  getIndexLowest(bool (*f)(Point<TYPE> *, Point<TYPE> *));
	int  getIndexHighest(bool (*f)(const Point<TYPE> *, const Point<TYPE> *));
	enum Turn_T returnTurn(const Point<TYPE> *p, int sourcePoint, int destPoint);
	double 		signedArea(int id1, int id2, int id3);

	// Figures? PENDING. Move to another module?
	bool 	getEdgeInserection(Line &line, int face, int &edgeId);
	bool 	findPath(Set<int> &extremeFaces, Line &line, vector<int> &vFacesId);

	bool operator==(const Dcel& other) const;
	Dcel & operator=(const Dcel &d) = default;
};

#endif /* DCEL_H_ */
