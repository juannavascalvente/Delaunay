#ifndef INCLUDE_CIRCLE_H_
#define INCLUDE_CIRCLE_H_


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Point.h"

#include <vector>
using namespace std;


/***********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#define NUM_POINTS_IN_CIRCLE            (3)


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Circle
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    vector<Point<TYPE>> vPoints;    // Points in circle.

	Point<TYPE> centre;			    // Circle centre.
	TYPE radius;				    // Circle radius.

    /*******************************************************************************************************************
    * Private class methods
    *******************************************************************************************************************/
	void computeCentre();

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor
	//------------------------------------------------------------------------
	Circle() : radius(0) {}
	Circle(Point<TYPE> *centre, TYPE radius);
	// TODO Circle must contain only 3 elements in vector: https://github.com/juannavascalvente/Delaunay/issues/58
	explicit Circle(vector<Point<TYPE>> &vPointsIn) : vPoints(vPointsIn)
    {
        // Initialize fields.
        computeCentre();
        this->radius = this->centre.distance(vPointsIn.at(0));
    };

    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
    /**
     * @fn      area
     * @brief   Computes circle area
     *
     * @return  circle area
     */
	inline double area() { return(PI*pow(this->radius,2)); };

    /**
     * @fn      perimeter
     * @brief   Computes circle perimeter
     *
     * @return  circle perimeter
     */
	inline double perimeter() { return(2*PI*this->radius); };

    /**
     * @fn      inCircle
     * @brief   Checks if input point falls into circle
     *
     * @param   p   (IN)    Point to check
     * @return  true if points falls into circle
     *          false otherwise
     */
	bool inCircle(Point<TYPE> &p);

    /*******************************************************************************************************************
    * Getter/Setter
    *******************************************************************************************************************/
	inline Point<TYPE>* getRefCentre() { return( &this->centre); };
	inline double getRadius() { return(this->radius); };
};

#endif /* INCLUDE_CIRCLE_H_ */
