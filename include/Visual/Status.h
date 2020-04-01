#ifndef VISUAL_STATUS_H_
#define VISUAL_STATUS_H_

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "StatusModel.h"


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Status
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    StatusModel status;

public:
    /*******************************************************************************************************************
    * Public methods declarations
    *******************************************************************************************************************/
	Status() = default;


	/**
	 * @fn      StatusModel
	 * @brief   Set status to value in input parameter
	 *
	 * @param   statusIn        (IN) Values to set to
	 */
    void set(const StatusModel &statusIn) { status = statusIn; }


    /**
     * @fn      reset
     * @brief   Reset status to its initial values
     */
	void reset() { status.reset();};


    /*******************************************************************************************************************
    * Getters/Setters
    *******************************************************************************************************************/
	inline bool isFirstTime() { return(status.isFirstTime());};
	inline bool isSetCreated() { return(status.isSet());};
	inline bool isTriangulationCreated() {return(status.isTriangulation());};
	inline bool isDelaunayCreated() { return(status.isDelaunay());};
	inline bool isVoronoiCreated() { return(status.isVoronoi());};
	inline bool isGabrielCreated() { return(status.isGabriel());};
	inline void setFirstTime(bool value) { status.setFirstTime(value);};
	inline void setSetCreated(bool value) { status.setSet(value); };
	inline void seTriangulationCreated(bool value) { status.setTriangulation(value); };
	inline void setDelaunayCreated(bool value) { status.setDelaunay(value); };
	inline void setVoronoiCreated(bool value) { status.setVoronoi(value); };
	inline void setGabrielCreated(bool value) { status.setGabriel(value);};
};

#endif /* VISUAL_STATUS_H_ */
