#ifndef INCLUDE_GABRIEL_H_
#define INCLUDE_GABRIEL_H_

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Dcel.h"
#include "Voronoi.h"

#include <iostream>
using namespace std;


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class Gabriel
{
    /*******************************************************************************************************************
    * Class members
    *******************************************************************************************************************/
    vector<bool> vValidEdges;	// Valid edge array.
	Dcel 	dcel;				// DCELto check.
	Voronoi voronoi;			// Voronoi graph associated to DCEL.


    /*******************************************************************************************************************
    * Private methods
    *******************************************************************************************************************/
	void validateEdges(int edgeIndex);

protected:

    /*******************************************************************************************************************
    * Protected methods
    *******************************************************************************************************************/
    void set(int index) { this->vValidEdges.at(index) = true; };
    bool at(int index) const { return this->vValidEdges.at(index); };

	friend class GabrielIO;
public:

    /*******************************************************************************************************************
     * Public methods declaration
     *******************************************************************************************************************/
	Gabriel(Dcel &dcelIn, Voronoi &voronoiIn) : dcel(dcelIn), voronoi(voronoiIn), vValidEdges(dcelIn.getNumEdges()) {};
	~Gabriel() = default;

    Gabriel(const Gabriel &d)
    {
        if(this != &d)
        {
            this->dcel = d.dcel;
            this->voronoi = d.voronoi;
            this->vValidEdges = d.vValidEdges;
        }
    }

	bool isSet(int index) { return(this->vValidEdges.at(index)); };
    bool build();

    /*******************************************************************************************************************
    * Getters/Setters
    *******************************************************************************************************************/
	int  getSize() const { return this->vValidEdges.size(); };
	Dcel *getDcel() {return &this->dcel; };
};

#endif /* INCLUDE_GABRIEL_H_ */
