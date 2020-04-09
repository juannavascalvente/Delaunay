#ifndef INCLUDE_VORONOI_H_
#define INCLUDE_VORONOI_H_

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Dcel.h"
#include "Line.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Voronoi
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
	Dcel 	triangulation;		// Reference to triangulation DCEL data.
	Dcel 	dcel;				// Reference to Voronoi DCEL data.
	bool    isBuilt;

    /*******************************************************************************************************************
    * Private methods declarations
    *******************************************************************************************************************/
	void computeCircumcentres(bool isIncremental);
	void buildArea(int pointIndex);
	Point<TYPE> computeExtremeVoronoi(int edgeIndex, Point<TYPE> &centre);
	bool edgeExists(Edge &edge);
	bool isBottomMostFace(int faceId);

	friend class VoronoiIO;

public:
    /*******************************************************************************************************************
    * Public methods declarations
    *******************************************************************************************************************/
	Voronoi() : isBuilt(false) {};
	explicit Voronoi(Dcel &t) : triangulation(t), isBuilt(false) {};
	~Voronoi() = default;

    Voronoi(const Voronoi &d)
    {
        this->isBuilt = d.isBuilt;
        this->dcel = d.dcel;
        this->triangulation = d.triangulation;
    }

	/**
	 * @fn  reset
	 * @brief   Resets voronoi dcel (but keeps triangulation dcel)
	 */
	void 	reset();

	/**
	 * @fn                      build
	 * @brief                   Builds Voronoi diagram associated to triangulation
	 * @param isIncremental     Flags if triangulation was built using Incremental algorithm
	 * @return                  true if build successfully
	 *                          false otherwise
	 */
	bool 	build(bool isIncremental);

	/**
	 * @fn              isInnerToArea
	 * @brief           checks if the input point is inner to areaId Voronoi area.
	 *
	 * @param p         (IN) Point to check
	 * @param areaId    (IN) Face id to check
	 * @return          true if point is interior to face
	 *                  false otherwise
	 */
	bool 	isInnerToArea(const Point<TYPE> &p, int areaId);

    /*******************************************************************************************************************
    * Getters/Setters
    *******************************************************************************************************************/
    bool isValid() const { return isBuilt; }
    void setValid(bool isValue) { isBuilt = isValue; }
    inline Dcel* getRefDcel() { return(&this->dcel); }

    /**
     * @fn              getCentre
     * @brief           Returns the centre of the input area id.
     * @param areaId    (IN) Area whose center point is returned
     * @param centre    (OUT) Area center
     */
    void 		 getCentre(int areaId, Point<TYPE> &centre) { centre = *this->dcel.getRefPoint(areaId); }
};

#endif /* INCLUDE_VORONOI_H_ */
