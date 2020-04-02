//
// Created by delaunay on 1/4/20.
//

#ifndef DELAUNAY_STATUSMODEL_H
#define DELAUNAY_STATUSMODEL_H


/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/


/***********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#define FIRST_TIME_DEFAULT          (true)
#define IS_SET_DEFAULT              (false)
#define IS_TRIANGULATION_DEFAULT    (false)
#define IS_DELAUNAY_DEFAULT         (false)
#define IS_VORONOI_DEFAULT          (false)
#define IS_GABRIEL_DEFAULT          (false)


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Status
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    bool	hasFirstTime;		// First time execution flag.
    bool	hasSet;             // Set of points created or read flag.
    bool	hasTriangulation;	// Star triangulation created flag.
    bool	hasDelaunay;        // Delaunay triangulation created flag.
    bool	hasVoronoi;			// Voronoi diagram computed flag.
    bool	hasGabriel;			// Gabriel graph computed flag.

public:
    Status() : hasFirstTime(FIRST_TIME_DEFAULT), hasSet(IS_SET_DEFAULT), hasTriangulation(IS_TRIANGULATION_DEFAULT),
                    hasDelaunay(IS_DELAUNAY_DEFAULT),
                    hasVoronoi(IS_VORONOI_DEFAULT),
                    hasGabriel(IS_GABRIEL_DEFAULT) {};

    Status(bool isFirstTime, bool isSet, bool isTriangulation, bool isDelaunay, bool isVoronoi, bool isGabriel) :
            hasFirstTime(isFirstTime),
            hasSet(isSet),
            hasTriangulation(isTriangulation),
            hasDelaunay(isDelaunay),
            hasVoronoi(isVoronoi),
            hasGabriel(isGabriel) {};


    /**
     * @fn      reset
     * @brief   reset to default status
     */
    void reset()
    {
        hasFirstTime = FIRST_TIME_DEFAULT;
        hasSet = IS_SET_DEFAULT;
        hasTriangulation = IS_TRIANGULATION_DEFAULT;
        hasDelaunay = IS_DELAUNAY_DEFAULT;
        hasVoronoi = IS_VORONOI_DEFAULT;
        hasGabriel = IS_GABRIEL_DEFAULT;
    }


    /**
     * @fn      StatusModel
     * @brief   Set status to value in input parameter
     *
     * @param   statusIn        (IN) Values to set to
     */
    void set(const Status &statusIn) { (*this) = statusIn; }


    /*******************************************************************************************************************
    * Getters/Setters
    *******************************************************************************************************************/
    bool isFirstTime() const { return hasFirstTime; }
    bool isSet() const { return hasSet; }
    bool isTriangulation() const { return hasTriangulation;}
    bool isDelaunay() const { return hasDelaunay; }
    bool isVoronoi() const { return hasVoronoi; }
    bool isGabriel() const { return hasGabriel; }
    void setFirstTime(bool isFirstTimeIn) { hasFirstTime = isFirstTimeIn; }
    void setSet(bool isSetIn) { hasSet = isSetIn; }
    void setTriangulation(bool isTriangulationIn) { hasTriangulation = isTriangulationIn; }
    void setDelaunay(bool isDelaunayIn) { hasDelaunay = isDelaunayIn; }
    void setVoronoi(bool isVoronoiIn) { hasVoronoi = isVoronoiIn; }
    void setGabriel(bool isGabrielIn) { hasGabriel = isGabrielIn; }
};


#endif //DELAUNAY_STATUSMODEL_H
