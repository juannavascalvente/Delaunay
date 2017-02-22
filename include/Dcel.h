/*
 * Dcel.h
 *
 *  Created on: Jun 29, 2016
 *      Author: jnavas
 */

#ifndef DCEL_H_
#define DCEL_H_

#include "Edge.h"
#include "Face.h"
#include "Line.h"
#include "Set.h"
#include "Vertex.h"

//------------------------------------------------------------------------
// PUBLIC defines
//------------------------------------------------------------------------
#define     NO_UPDATE           	-1
#define 	EXTERNAL_FACE       	0

/****************************************************************************
// 								DCEL CLASS DEFITNION
****************************************************************************/
class Dcel
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	bool	created;
	bool	incremental;

	// Vertex data.
	int 	nVertex;
	int		sizeVertex;
	Vertex	*vertex;

	// Edges data.
	int 	nEdges;
	int		sizeEdges;
	Edge	*edges;

	// Faces data.
	int 	nFaces;
	int		sizeFaces;
	Face	*faces;

	// Max and min X and Y coordinates for all points in the DCEL set.
	TYPE	minX;
	TYPE	minY;
	TYPE	maxX;
	TYPE	maxY;

	//------------------------------------------------------------------------
	//  Private functions.
	//------------------------------------------------------------------------
	void clutter();
	void quicksort(Vertex *origin, Vertex *list, int first, int last);
	int	 movePivot(Vertex *origin, Vertex *list, int first, int last);
	void swapVertex(int index1, int index2);

	void drawPointsInfo();
	void drawEdgesInfo();
	void drawFacesInfo();

	bool read(string fileName);
	bool write(string fileName) const;
	bool readBinary(string fileName);
	bool writeBinary(string fileName)  const;

public:
	/*------------------------------------------------------------------------
	  Constructor/Destructor.
	------------------------------------------------------------------------*/
	Dcel();
	Dcel(int nPoints, int nEdges=INVALID, int nFaces=INVALID);
	~Dcel();

	/*------------------------------------------------------------------------
	  Public functions.
	------------------------------------------------------------------------*/
	// Add / Delete functions.
	void addVertex(const Point<TYPE> *p, const int edge);
	void addVertex(const Vertex *vertex);
	void updateVertex(int edge_ID, int index);
	void updateVertex(Point<TYPE> *p, int index);

	void addEdge(const Edge *edge);
	void addEdge(int origin, int	twin, int previou, int next, int face);
	void updateEdge(int origin, int twin, int previous, int next, int face, int index);

	void addFace(const Face *face);
	void addFace(const int face);
	void updateFace(int edge_ID, int index);

	// Check functions.
	bool isExternalEdge(int edgeIndex) const;
	bool hasNegativeVertex(int edgeID) const;
	void getEdgeVertices(int edgeID, int &index1, int &index2) const;
	int	 getCollinear(int pointIndex, int edgeID);

	//---------------------------------------------------------------------------
	// Get/set functions.
	//---------------------------------------------------------------------------
	inline Point<TYPE> *getRefPoint(int index) const {return(this->vertex[index].getRefPoint());};
	inline Edge *getRefEdge(int index) {return(&this->edges[index]);};
	inline Face *getRefFace(int index) {return(&this->faces[index]);};
	inline int getNVertex() const {return(this->nVertex);};
	inline int getNEdges() const{return(this->nEdges);};
	inline int getNFaces()  const{return(this->nFaces);};
	inline int getSizeVertex() const{return(this->sizeVertex);};
	inline int getSizeEdges() const {return(this->sizeEdges);};
	inline int getSizeFaces() const {return(this->sizeFaces);};

	// Access to POINTS fields.
	inline int getPointEdge(int pointIndex) {return(this->vertex[pointIndex].getOrigin());};
	//void getNeighbors(int pointIndex, bool &checked, Queue<int> &queue);

	// Access to EDGES fields.
	inline int getOrigin(int edgeIndex) const {return(this->edges[edgeIndex].getOrigin());};
	inline int getTwin(int edgeIndex) const {return(this->edges[edgeIndex].getTwin());};
	inline int getPrevious(int edgeIndex) const {return(this->edges[edgeIndex].getPrevious());};
	inline int getNext(int edgeIndex) const {return(this->edges[edgeIndex].getNext());};
	inline int getFace(int edgeIndex) const {return(this->edges[edgeIndex].getFace());};
	inline int getNextNeighbor(int edgeIndex) const {return(this->edges[this->edges[edgeIndex].getPrevious()-1].getTwin());};
	inline void setOrigin(int edgeIndex, int v) {this->edges[edgeIndex].setOrigin(v);};
	inline void setTwin(int edgeIndex, int v) {this->edges[edgeIndex].setTwin(v);};
	inline void setPrevious(int edgeIndex, int v) {this->edges[edgeIndex].setPrevious(v);};
	inline void setNext(int edgeIndex, int v) {this->edges[edgeIndex].setNext(v);};
	inline void setFace(int edgeIndex, int v) {this->edges[edgeIndex].setFace(v);};
	void getEdgePoints(int edgeIndex, Point<TYPE> &origin, Point<TYPE> &dest) const;

	// Access to FACES fields.
	inline int  getFaceEdge(int faceId) const {return(this->faces[faceId].getEdge());};
	inline void setFaceEdge(int faceIndex, int v) {this->faces[faceIndex].setEdge(v);};
	bool imaginaryFace(int faceIndex) const;
	bool isBottomMostFace(int faceIndex) const;
	void getBottomMostFaceNeighborFaces(int faceId, Set<int> &faces) const;
	inline bool isExternalFace(int faceIndex) const {return(faceIndex == EXTERNAL_FACE);};
	void getFaceVertices(int faceIndex, int *ids) const;
	void getFacePoints(int faceIndex, Point<TYPE> &p, Point<TYPE> &q, Point<TYPE> &r);

	// Set functions.
	void reset();
	void resize(int size, bool copy);		// PENDING Return false if FAILED?
	void invalidate();
	void shake();
	// PENDING. Move to set? point?
	bool generateRandom(int nPoints);
	bool generateClusters(int nPoints, int nClusters, TYPE radius);
	void sort();
	void 		setLowestFirst(bool (*f)(Point<TYPE> *, Point<TYPE> *));
	void 		setHighestFirst(bool (*f)(Point<TYPE> *, Point<TYPE> *));
	enum Turn_T returnTurn(Point<TYPE> *p, int sourcePoint, int destPoint);
	double 		signedArea(int id1, int id2, int id3);

	// Figures? PENDING. Move to another module?
	bool 	getEdgeInserection(Line &line, int face, int &edgeId);
	bool 	findPath(Set<int> &extremeFaces, Line &line, Set<int> &faces);

	// I/O functions.
	bool 	read(string fileName, bool isBinary);
	bool 	write(string fileName, bool isBinary) const;
	bool 	readPoints(string fileName);
	bool 	writePoints(string fileName, int nPoints) const;
	void 	print(std::ostream& out) const;

	bool operator==(const Dcel& other) const;
#ifdef TESTING
	int		test(int nPoints);
#endif
};

#endif /* DCEL_H_ */
