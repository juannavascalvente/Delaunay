#ifndef INCLUDE_POLYGON_H_
#define INCLUDE_POLYGON_H_


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Line.h"
#include "Point.h"

#include <vector>
using namespace std;


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Polygon
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
	vector<Point<TYPE>> vPoints;

public:

    /*******************************************************************************************************************
    * Constructor/Destructor
    *******************************************************************************************************************/
	Polygon() = default;
	explicit Polygon(int n) : vPoints(n) {};
    explicit Polygon(vector<Point<TYPE>> &v) { vPoints = v; }
	virtual ~Polygon() = default;
    Polygon(const Polygon &p) { vPoints = p.vPoints; }

    /*******************************************************************************************************************
    * Public methods
    *******************************************************************************************************************/
	inline int getNElements() { return(vPoints.size()); };
    inline void getPoints(vector<Point<TYPE>> &vPointsOut) { vPointsOut = vPoints; };
	inline Point<TYPE> at(size_t szIdx) { return(vPoints.at(szIdx)); };
	inline void	add(Point<TYPE> &p) { vPoints.push_back(p); };
	inline void reset() { vPoints.clear(); };

    /***************************************************************************
    * Name: 	centroid
    * IN:		NONE
    * OUT:		center		polygon centroid.
    * RETURN:	NONE
    * GLOBAL:	NONE
    * Description: 	computes the centroid of the polygon.
    ***************************************************************************/
	void 	centroid(Point<TYPE> &center);

    /***************************************************************************
    * Name: 	getIntersections
    * IN:		line		line to check
    * OUT:		set			set of edges that intersect line.
    * RETURN:	true 		if intersect.
    * 			false		i.o.c.
    * GLOBAL:	NONE
    * Description: 	gets the set of edges(two maximum) that intersects the
    * 				polygon.
    ***************************************************************************/
	bool getIntersections(Line &line, vector<int> &intersection);

	/**
	 * @fn      getIntersections
	 * @brief   Gets the intersection points, if any, between line and polygon
	 *
	 * @param   line    (IN) Line to check intersection with polygon
	 * @param   vOut    (OUT) Vector that contains intersection points (if any)
	 */
    void getIntersections(Line &line, vector<Point<TYPE>> &vOut);

    /***************************************************************************
    * Name: 	isInternal
    * IN:		p			point to check
    * OUT:		NONE
    * RETURN:	true 		if point is interior to polygon.
    * 			false		i.o.c.
    * GLOBAL:	NONE
    * Description: 	Checks if the input point is interior to the polygon
    ***************************************************************************/
	bool	isInternal(Point<TYPE> &p);

    /***************************************************************************
    * Name: 	print
    * IN:		out			output stream
    * OUT:		NONE
    * RETURN:	NONE
    * GLOBAL:	NONE
    * Description: 	print the set of points.
    ***************************************************************************/
	void 	print(std::ostream& out);

    /***************************************************************************
    * Name: 	toStr
    * IN:		NONE
    * OUT:		NONE
    * RETURN:	string		set of points as text.
    * GLOBAL:	NONE
    * Description: 	convert to string the set of points
    ***************************************************************************/
	string 	toStr();
};

#endif /* INCLUDE_POLYLINE_H_ */
