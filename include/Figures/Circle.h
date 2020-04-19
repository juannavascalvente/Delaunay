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
    /*******************************************************************************************************************
    * Public class methods
    *******************************************************************************************************************/
	Circle(Point<TYPE> *centre, TYPE radius);
	explicit Circle(vector<Point<TYPE>> &vPointsIn) : vPoints(vPointsIn)
    {
	    // Check exactly 3 points are used to create circle
	    if (vPointsIn.size() != NUM_POINTS_IN_CIRCLE)
        {
            throw std::runtime_error("Circle must be created using exactly 3 points\n");
        }

        // Initialize fields
        computeCentre();
        this->radius = this->centre.distance(vPointsIn.at(0));
    };

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
