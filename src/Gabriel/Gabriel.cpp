/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Gabriel.h"
//#include "Line.h"
//#include "Point.h"


/***********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#ifdef DEBUG_GEOMETRICAL
#include "Logging.h"
//#define DEBUG_GABRIEL_CONSTRUCTOR
//#define DEBUG_GABRIEL_BUILD
//#define DEBUG_GABRIEL_VALIDATE_EDGES
//#define DEBUG_GABRIEL_RESIZE
//#define DEBUG_GABRIEL_READ
#endif


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
/***************************************************************************
* Name: 	build
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description:	builds the Gabriel graph using the DCEl attribute as the
* 				triangulation to check. For every edge in the triangulation
* 				it checks if the circle whose diameter is that edge
* 				surrounds any other point of the triangulation.
***************************************************************************/
bool Gabriel::build()
{
    int     edgeIndex=0;		// Loop counter.
    int		nEdges=0;			// Upper bound limit.
    Point<TYPE> *p1, *p2;		// Edge origin and destination points.
    Point<TYPE> q1, q2;		// Edge origin and destination points.
    int     face1=0, face2=0; 	// Faces that share edge.
    Line 	line1, line2;

    // Check all edges.
    nEdges = this->getSize();
#ifdef DEBUG_GABRIEL_BUILD
    Logging::buildText(__FUNCTION__, __FILE__, "Building Gabriel graph. # edges: ");
		Logging::buildText(__FUNCTION__, __FILE__, nEdges);
		Logging::write(true, Info);
#endif
    for (edgeIndex=0; edgeIndex<nEdges ;edgeIndex++)
    {
#ifdef DEBUG_GABRIEL_BUILD
        Logging::buildText(__FUNCTION__, __FILE__, "Edge ");
			Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
			Logging::buildText(__FUNCTION__, __FILE__, "/");
			Logging::buildText(__FUNCTION__, __FILE__, nEdges);
			Logging::write(true, Info);
#endif
        // Skip edges higher than its twin -> already checked.
        if (edgeIndex < this->dcel.getTwin(edgeIndex))
        {
            // Skip imaginary edges.
            if (!this->dcel.hasNegativeVertex(edgeIndex+1))
            {
#ifdef DEBUG_GABRIEL_BUILD
                Logging::buildText(__FUNCTION__, __FILE__, "Checking intersection for edge ");
					Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
					Logging::write(true, Info);
#endif
                // Get faces that share edge.
                face1 = this->dcel.getFace(edgeIndex);
                face2 = this->dcel.getFace(this->dcel.getTwin(edgeIndex)-1);

                // Get origin and destination point of triangulation segment.
                p1 = this->dcel.getRefPoint(this->dcel.getOrigin(edgeIndex)-1);
                p2 = this->dcel.getRefPoint(this->dcel.getOrigin(this->dcel.getTwin(edgeIndex)-1)-1);

                // Get origin and destination point of triangulation segment.
                this->voronoi.getCentre(face1-1, q1);
                this->voronoi.getCentre(face2-1, q2);

                // Create lines to check intersection.
                line1 = Line(*p1, *p2);
                line2 = Line(q1, q2);

                // Check if both segment intersect.
                if (line1.intersect(line2))
                {
#ifdef DEBUG_GABRIEL_BUILD
                    Logging::buildText(__FUNCTION__, __FILE__, "Adding edge ");
						Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
						Logging::write(true, Info);
#endif
                    // Set edge and its twin as valid Gabriel edges.
                    this->validateEdges(edgeIndex);
                }
#ifdef DEBUG_GABRIEL_BUILD
                else
					{
						Logging::buildText(__FUNCTION__, __FILE__, "Not intersection for edge ");
						Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
						Logging::write(true, Info);
					}
#endif
            }
#ifdef DEBUG_GABRIEL_BUILD
            else
				{
					Logging::buildText(__FUNCTION__, __FILE__, "Non real edge ");
					Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
					Logging::write(true, Info);
				}
#endif
        }
#ifdef DEBUG_GABRIEL_BUILD
        else
			{
				Logging::buildText(__FUNCTION__, __FILE__, "Skipping edge ");
				Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
				Logging::buildText(__FUNCTION__, __FILE__, " because twin edge already checked ");
				Logging::buildText(__FUNCTION__, __FILE__, this->dcel.getTwin(edgeIndex));
				Logging::write(true, Info);
			}
#endif
    }

    return true;
}


/***************************************************************************
* Name: 	validateEdges
* IN:		edgeIndex			edge index to set to true
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	sets to true the input edge and its twin.
***************************************************************************/
void Gabriel::validateEdges(int edgeIndex)
{
#ifdef DEBUG_GABRIEL_VALIDATE_EDGES
	Logging::buildText(__FUNCTION__, __FILE__, "Setting edge ");
	Logging::buildText(__FUNCTION__, __FILE__, edgeIndex+1);
	Logging::buildText(__FUNCTION__, __FILE__, " and its twin ");
	Logging::buildText(__FUNCTION__, __FILE__, this->dcel.getTwin(edgeIndex)-1);
	Logging::write(true, Info);
#endif
	this->set(edgeIndex);
	this->set(this->dcel.getTwin(edgeIndex)-1);
}
