/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DcelGenerator.h"

#include <random>


/***********************************************************************************************************************
* Public methods definitions
***********************************************************************************************************************/
bool DcelGenerator::generateRandom(size_t szNumPoints, vector<Point<TYPE>> &vPoints)
{
    // Number of points must be higher than 0
    if (szNumPoints == 0)
    {
        std::cout << __func__ << ".Number of points must be higher than 0 and it is " <<  szNumPoints << std::endl;
        return false;
    }

    // Generate points
    srand48((int) time(nullptr));

    // Generate set of points
    Point<TYPE> p;
    for (std::size_t  i=0; i<szNumPoints ;i++)
    {
        p.random();
        vPoints.push_back(p);
    }

    return true;
}


bool DcelGenerator::generateClusters(size_t szNumPoints, size_t szNumClusters, TYPE radius, vector<Point<TYPE>> &vPoints)
{
    // Number of points must be higher than 0
    if (szNumPoints == 0)
    {
        std::cout << __func__ << ".Number of points must be higher than 0 and it is " <<  szNumPoints << std::endl;
        return false;
    }

    // Number of points must be higher than 0
    if (szNumClusters == 0)
    {
        std::cout << __func__ << ".Number of clusters must be higher than 0 and it is " <<  szNumPoints << std::endl;
        return false;
    }

    // Number of points must be higher than 0
    if (radius <= 0.0)
    {
        std::cout << __func__ << ".Radius clusters must be higher than 0 and it is " <<  radius << std::endl;
        return false;
    }

    // Initialize output
    vPoints.clear();

    srand48((int) time(nullptr));

    // Get # elements per cluster and remaining number of points to add to last cluster
    size_t szNumElemsCluster = szNumPoints / szNumClusters;

    // Generate clusters centers.
    Point<TYPE> p;
    for (size_t i=0; i<szNumClusters ;i++)
    {
        // Generate new point.
        p.random();

        // Insert new point
        vPoints.push_back(p);

        // Generate cluster points based on p point and shifting it a maximum distance of the radius
        for (size_t j = 0; j < szNumElemsCluster - 1; j++)
        {
            // Create point using p as base point and shake it a maximum distance of radius
            Point<TYPE> q = p;
            q.shake(radius);

            // Insert new point
            vPoints.push_back(q);
        }
    }

    // Add remaining elements to last cluster
    size_t szNumRemaining = szNumPoints % szNumClusters;
    for (size_t i=0; i<szNumRemaining ; i++)
    {
        // Create point using p as base point and shake it a maximum distance of radius
        Point<TYPE> q = p;
        q.shake(radius);

        // Insert new point
        vPoints.push_back(q);
    }

    return true;
}


/**********************************************************************************************************************/

bool DcelGenerator::generateRandom(int nPoints, Dcel &dcel)
{
    bool	generated=true;	// Return value.
    int		i=0;			// Loop counter.
    Point<TYPE> p;			// Temporary point.

    // Check # points to generate is positive.
    if (nPoints > 0)
    {
#ifdef DEBUG_GENERATE_RANDOM_DCEL
        Logging::buildText(__FUNCTION__, __FILE__, "# points is to generate is ");
		Logging::buildText(__FUNCTION__, __FILE__, nPoints);
		Logging::write( true, Info);
#endif
        // Resize DCEL if new number of points is higher.
        if (nPoints > dcel.getSizeVertex())
        {
#ifdef DEBUG_GENERATE_RANDOM_DCEL
            Logging::buildText(__FUNCTION__, __FILE__, "Resizing set of points.");
			Logging::write( true, Info);
#endif
            dcel.resize(nPoints, false);
        }
            // Otherwise just reset the # of elements.
        else
        {
#ifdef DEBUG_GENERATE_RANDOM_DCEL
            Logging::buildText(__FUNCTION__, __FILE__, "Reseting set of points.");
			Logging::write( true, Info);
#endif
            dcel.reset();
        }

        // Create seed.
        srand48((int) time(nullptr));

        // Generate new set of points.
        for (i=0; i<nPoints ;i++)
        {
            // Generate random points.
            p.random();
            dcel.addVertex(&p, INVALID);
        }
#ifdef DEBUG_GENERATE_RANDOM_DCEL
        Logging::buildText(__FUNCTION__, __FILE__, "# points generated.");
		Logging::buildText(__FUNCTION__, __FILE__, dcel.getNVertex());
		Logging::write( true, Info);
#endif
    }
    else
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Number of points to generate must be positive and is ");
        Logging::buildText(__FUNCTION__, __FILE__, nPoints);
        Logging::write(true, Error);
        generated = false;
    }

    return(generated);
}


bool DcelGenerator::generateClusters(int nPoints, int nClusters, TYPE radius, Dcel &dcel)
{
    bool		generated=true;			// Return value.
    int			i=0, j=0;				// Loop counters.
    int			nElementsCluster=0;		// # points per cluster.
    Point<TYPE> p, q;					// Temporary points.

#ifdef DEBUG_GENERATE_CLUSTER
    Logging::buildText(__FUNCTION__, __FILE__, "Generating ");
	Logging::buildText(__FUNCTION__, __FILE__, nPoints);
	Logging::buildText(__FUNCTION__, __FILE__, " points in ");
	Logging::buildText(__FUNCTION__, __FILE__, nClusters);
	Logging::buildText(__FUNCTION__, __FILE__, " clusters with radius ");
	Logging::buildText(__FUNCTION__, __FILE__, radius);
	Logging::write(true, Info);
#endif

    // Resize DCEL
    dcel.resize(nPoints, false);

    // Create seed.
    srand48((int) time(nullptr));

    // Get # elements per cluster.
    nElementsCluster = nPoints / nClusters;

    // Generate clusters centers.
    for (i=0; i<nClusters ;i++)
    {
        // Generate new point.
        p.random();

        // Insert new point.
        dcel.addVertex(&p, INVALID);

#ifdef DEBUG_GENERATE_CLUSTER
        Logging::buildText(__FUNCTION__, __FILE__, "Generating cluster ");
    	Logging::buildText(__FUNCTION__, __FILE__, i+1);
    	Logging::buildText(__FUNCTION__, __FILE__, " whose seed is ");
    	Logging::buildText(__FUNCTION__, __FILE__, p.toStr());
    	Logging::write(true, Info);
#endif

        // Add points center in current seed.
        for (j=0; j<nElementsCluster-1 ;j++)
        {
            q = p;

            // Generate random number between 0 and 3.
            random_device rd;
            mt19937 mt(rd());
            uniform_real_distribution<double> dist(0.0, 3.0);

            // Move point in random direction.
            int move = (int) dist(mt);
            if (move == 0)
            {
                q.shift((TYPE) (-drand48())*radius, (TYPE) (-drand48())*radius);
            }
            else if (move == 1)
            {
                q.shift((TYPE) (-drand48())*radius, (TYPE) drand48()*radius);
            }
            else if (move == 2)
            {
                q.shift((TYPE) drand48()*radius, (TYPE) (-drand48())*radius);
            }
            else
            {
                q.shift((TYPE) drand48()*radius, (TYPE) drand48()*radius);
            }

            // Insert new point.
            dcel.addVertex(&q, INVALID);
#ifdef DEBUG_GENERATE_CLUSTER
            Logging::buildText(__FUNCTION__, __FILE__, "Generating point ");
        	Logging::buildText(__FUNCTION__, __FILE__, j+1);
        	Logging::buildText(__FUNCTION__, __FILE__, " ");
        	Logging::buildText(__FUNCTION__, __FILE__, p.toStr());
        	Logging::write(true, Info);
#endif
        }

        // Check if it is last cluster.
        if (i == (nClusters-1))
        {
            // Add elements randomly until nPoints reached.
            for (i=dcel.getNVertex(); i<nPoints ;i++)
            {
                q = p;

                // Generate random number between 0 and 3.
                random_device rd;
                mt19937 mt(rd());
                uniform_real_distribution<double> dist(0.0, 3.0);

                // Move point in random direction.
                int move = (int) dist(mt);
                if (move == 0)
                {
                    q.shift((TYPE) (-drand48())*radius, (TYPE) (-drand48())*radius);
                }
                else if (move == 1)
                {
                    q.shift((TYPE) (-drand48())*radius, (TYPE) drand48()*radius);
                }
                else if (move == 2)
                {
                    q.shift((TYPE) drand48()*radius, (TYPE) (-drand48())*radius);
                }
                else
                {
                    q.shift((TYPE) drand48()*radius, (TYPE) drand48()*radius);
                }

                // Insert new point.
                dcel.addVertex(&q, INVALID);
            }
        }
    }

    // Clutter set of points.
    DcelGenerator::clutter(dcel);

    // PENDING return value never false???
    return(generated);
}


/***********************************************************************************************************************
* Private methods definitions
***********************************************************************************************************************/
/***************************************************************************
* Name: 	clutter
* IN:		N/A
* OUT:		N/A
* IN/OUT:	N/A
* RETURN:	N/A
* Description: clutters the set of point of the DCEL.
***************************************************************************/
void DcelGenerator::clutter(Dcel &dcel)
{
    int	i=0;					// Loop counter.
    int	index1=0, index2=0;		// Array indexes.

    // Generate random number between 0 and 3.
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> dist(0.0, dcel.getNVertex());

    // Loop set of points.
    for (i=0; i<dcel.getNVertex() ;i++)
    {
        // Generate random indexes to swap.
        index1 = (int) dist(mt);
        index2 = (int) dist(mt);

        // Swap elements.
        dcel.swapVertex(index1, index2);
    }
}
