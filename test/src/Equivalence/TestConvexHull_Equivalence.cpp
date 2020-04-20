/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <iostream>
#include <gtest/gtest.h>

#include "Delaunay.h"
#include "PointFactory.h"
#include "StarTriangulation.h"
#include "Voronoi.h"

using namespace std;


/***********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#define NUM_POINTS_1K            (1000)
#define NUM_POINTS_10K           (10000)
#define NUM_POINTS_100K          (100000)

#define NUM_ITERATIONS_10          (10)
#define NUM_ITERATIONS_100         (100)
#define NUM_ITERATIONS_1000        (1000)


/***********************************************************************************************************************
* Test Class Definition
***********************************************************************************************************************/
namespace
{
    class TestConvexHull_Equivalence : public ::testing::Test
    {

    protected:

        // You can remove any or all of the following functions if its body
        // is empty.
        // Constructor (called before each test case) - SetUp
        TestConvexHull_Equivalence() = default;

        // Deconstructor (called after each test case) - TearDown
        ~TestConvexHull_Equivalence() override = default;

    public:

        static void executeSubtest(size_t szNumPoints, size_t szNumIterations);
    };

    void TestConvexHull_Equivalence::executeSubtest(size_t szNumPoints, size_t szNumIterations)
    {
        // Execute test szNumIterations times
        for (size_t i=0; i<szNumIterations ; i++)
        {
            auto *hullStar = new Polygon();
            auto *hullDelaunay = new Polygon();

            // Generate random points set
            vector<Point<TYPE>> vPoints;
            PointFactory::generateRandom(szNumPoints, vPoints);

            // Build Delaunay triangulation and Voronoi diagram
            auto *delaunay = new Delaunay(vPoints);
            bool isSuccess = delaunay->build();
            ASSERT_TRUE(isSuccess);

            // Compute Delaunay triangulation convex hull
            isSuccess = delaunay->convexHull();
            delaunay->getConvexHull(*hullDelaunay);
            ASSERT_TRUE(isSuccess);

            // Build Star triangulation
            auto *starTriangulation = new StarTriangulation(vPoints);
            isSuccess = starTriangulation->build();
            ASSERT_TRUE(isSuccess);

            // Compute Star triangulation convex hull
            isSuccess = starTriangulation->convexHull();
            ASSERT_TRUE(isSuccess);
            starTriangulation->getConvexHull(*hullStar);

            // Check both polygons are the same
            cout << "Test " << (i+1) << "/" << szNumIterations << endl;
            ASSERT_TRUE(*hullStar == *hullDelaunay);

            // Free resources
            delete starTriangulation;
            delete delaunay;
            delete hullStar;
            delete hullDelaunay;
        }
    }
}


/**
 * DESCRIPTION:
 *
 */
TEST_F(TestConvexHull_Equivalence, Test_Num_Samples_100000_Iter_1000)
{
    executeSubtest(NUM_POINTS_100K, NUM_ITERATIONS_1000);
}


/**
 * DESCRIPTION:
 *
 */
TEST_F(TestConvexHull_Equivalence, Test_Num_Samples_10000_Iter_100)
{
    executeSubtest(NUM_POINTS_10K, NUM_ITERATIONS_100);
}


/**
 * DESCRIPTION:
 *
 */
TEST_F(TestConvexHull_Equivalence, Test_Num_Samples_1000_Iter_10)
{
    executeSubtest(NUM_POINTS_1K, NUM_ITERATIONS_10);
}
