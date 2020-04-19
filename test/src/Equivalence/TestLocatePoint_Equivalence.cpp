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
#define NUM_POINTS_1000            (1000)
#define NUM_POINTS_10000           (10000)
#define NUM_POINTS_100000          (100000)

#define NUM_ITERATIONS_10           (10)
#define NUM_ITERATIONS_100          (100)
#define NUM_ITERATIONS_1000         (1000)


/***********************************************************************************************************************
* Test Class Definition
***********************************************************************************************************************/
namespace
{
    class TestLocatePoint_Equivalence : public ::testing::Test
    {

    protected:

        // You can remove any or all of the following functions if its body
        // is empty.
        // Constructor (called before each test case) - SetUp
        TestLocatePoint_Equivalence() = default;

        // Deconstructor (called after each test case) - TearDown
        ~TestLocatePoint_Equivalence() override = default;

    public:

        static void executeSubtest(size_t szNumPoints, size_t szNumIterations);
    };

    void TestLocatePoint_Equivalence::executeSubtest(size_t szNumPoints, size_t szNumIterations)
    {
        // Generate random points to find
        vector<Point<TYPE>> vPointsToFind;
        PointFactory::generateRandom(szNumIterations, vPointsToFind);

        // Execute test szNumIterations times
        for (size_t i=0; i<szNumIterations ; i++)
        {
            // Generate random points set
            vector<Point<TYPE>> vPoints;
            PointFactory::generateRandom(szNumPoints, vPoints);

            // Build Delaunay triangulation and Voronoi diagram
            auto *delaunay = new Delaunay(vPoints);
            bool isSuccess = delaunay->build();
            ASSERT_TRUE(isSuccess);

            // Build Voronoi diagram
            auto *voronoi = new Voronoi(delaunay->getRefDcel());
            isSuccess = voronoi->build();
            ASSERT_TRUE(isSuccess);

            // Find closest point in Delaunay triangulation
            int pointIndex=0;
            Point<TYPE> closestDelaunay;
            isSuccess = delaunay->findClosestPoint(vPointsToFind.at(i), voronoi, closestDelaunay, pointIndex);
            ASSERT_TRUE(isSuccess);

            // Build Star triangulation
            auto *starTriangulation = new StarTriangulation(vPoints);
            isSuccess = starTriangulation->build();
            ASSERT_TRUE(isSuccess);

            // Find closest point in Star triangulation
            Point<TYPE> closestStar;
            isSuccess = starTriangulation->findClosestPoint(vPointsToFind.at(i), nullptr, closestStar, pointIndex);
            ASSERT_TRUE(isSuccess);

            // Check both points are the same
            ASSERT_TRUE(closestStar == closestDelaunay);

            cout << "Test " << (i+1) << "/" << szNumIterations << endl;

            // Free resources
            delete starTriangulation;
            delete voronoi;
            delete delaunay;
        }
    }
}


/**
 * DESCRIPTION:
 *
 */
TEST_F(TestLocatePoint_Equivalence, Test_Num_Samples_100000_Iter_1000)
{
    executeSubtest(NUM_POINTS_100000, NUM_ITERATIONS_10);
}


/**
 * DESCRIPTION:
 *
 */
TEST_F(TestLocatePoint_Equivalence, Test_Num_Samples_10000_Iter_100)
{
    executeSubtest(NUM_POINTS_10000, NUM_ITERATIONS_100);
}


/**
 * DESCRIPTION:
 *
 */
TEST_F(TestLocatePoint_Equivalence, Test_Num_Samples_1000_Iter_10)
{
    executeSubtest(NUM_POINTS_1000, NUM_ITERATIONS_1000);
}
