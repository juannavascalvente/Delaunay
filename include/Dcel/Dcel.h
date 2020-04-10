#ifndef DCEL_H_
#define DCEL_H_

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DcelModel.h"
#include "Edge.h"
#include "Face.h"
#include "Line.h"
#include "Vertex.h"

#include <utility>
#include <vector>


/***********************************************************************************************************************
* Class definition
***********************************************************************************************************************/
class Dcel : public DcelModel
{
	/*******************************************************************************************************************
	* Private methods
	*******************************************************************************************************************/
	void quicksort(Vertex *origin, Vertex *list, int first, int last);
	int	 movePivot(Vertex *origin, Vertex *list, int first, int last);

public:
	/*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
    Dcel() = default;
    explicit Dcel(vector<Point<TYPE>> &v) : DcelModel(v) {};
	~Dcel() = default;

	// Check functions.
	bool isExternalEdge(int edgeIndex);
	bool hasNegativeVertex(int edgeID);
	int	 getCollinear(int pointIndex, int edgeID);

	// PENDING. Move to set? point?
	void sort();
	int  getIndexLowest(bool (*f)(Point<TYPE> *, Point<TYPE> *));
	int  getIndexHighest(bool (*f)(const Point<TYPE> *, const Point<TYPE> *));
	enum Turn_T returnTurn(const Point<TYPE> *p, int sourcePoint, int destPoint);
	double 		signedArea(int id1, int id2, int id3);

	// Figures? PENDING. Move to another module?
	bool 	getEdgeInserection(Line &line, int face, int &edgeId);
	bool 	findPath(vector<int> &vExtremeFaces, Line &line, vector<int> &vFacesId);

	bool isInsideFace(const Point<TYPE> &p, int faceId);

    /**
     * @fn                  imaginaryFace
     * @brief               Checks if any of the vertex of the face is imaginary (only incremental algorithms)
     *
     * @param faceIndex     (IN) Face id to check
     * @return
     */
    bool imaginaryFace(int faceIndex);

    /**
     * @fn              getPoints
     * @brief           Gets Dcel set of points
     *
     * @param vOut      (OUT) Points in dcel
     */
    void getPoints(vector<Point<TYPE>> &vOut);
};

#endif /* DCEL_H_ */
