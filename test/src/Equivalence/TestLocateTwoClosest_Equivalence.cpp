/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <iostream>
#include <gtest/gtest.h>

#include "Delaunay.h"
#include "PointFactory.h"
#include "StarTriangulation.h"

using namespace std;


/***********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#define NUM_POINTS_1000            (1000)
#define NUM_POINTS_10000           (10000)
#define NUM_POINTS_100000          (100000)

#define NUM_ITERATIONS_10          (10)
#define NUM_ITERATIONS_100         (100)
#define NUM_ITERATIONS_1000        (1000)


/***********************************************************************************************************************
* Test Class Definition
***********************************************************************************************************************/
namespace
{
    class TestLocateTwoClosest_Equivalence : public ::testing::Test
    {

    protected:

        // You can remove any or all of the following functions if its body
        // is empty.
        // Constructor (called before each test case) - SetUp
        TestLocateTwoClosest_Equivalence() = default;

        // Deconstructor (called after each test case) - TearDown
        ~TestLocateTwoClosest_Equivalence() override = default;

    public:

        static void executeSubtest(size_t szNumPoints, size_t szNumIterations);
    };

    void TestLocateTwoClosest_Equivalence::executeSubtest(size_t szNumPoints, size_t szNumIterations)
    {
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

            // Find two closest point in Delaunay triangulation
            Point<TYPE> closestDelaunayP1;
            Point<TYPE> closestDelaunayP2;
            isSuccess = delaunay->findTwoClosest(closestDelaunayP1, closestDelaunayP2);
            ASSERT_TRUE(isSuccess);

            // Build Star triangulation
            auto *starTriangulation = new StarTriangulation(vPoints);
            isSuccess = starTriangulation->build();
            ASSERT_TRUE(isSuccess);

            // Find closest point in Star triangulation
            Point<TYPE> closestStarP1;
            Point<TYPE> closestStarP2;
            isSuccess = starTriangulation->findTwoClosest(closestStarP1, closestStarP2);
            ASSERT_TRUE(isSuccess);

            // Check closest points are equal
            ASSERT_TRUE(((closestStarP1 == closestDelaunayP1) && (closestStarP2 == closestDelaunayP2)) ||
                                ((closestStarP2 == closestDelaunayP1) && (closestStarP1 == closestDelaunayP2)));
            cout << "Test " << (i+1) << "/" << szNumIterations << endl;

            // Free resources
            delete starTriangulation;
            delete delaunay;
        }
    }
}


/**
 * DESCRIPTION:
 *
 */
TEST_F(TestLocateTwoClosest_Equivalence, Test_Num_Samples_100000_Iter_1000)
{
    executeSubtest(NUM_POINTS_100000, NUM_ITERATIONS_10);
}


/**
 * DESCRIPTION:
 *
 */
TEST_F(TestLocateTwoClosest_Equivalence, Test_Num_Samples_10000_Iter_100)
{
    executeSubtest(NUM_POINTS_10000, NUM_ITERATIONS_100);
}


/**
 * DESCRIPTION:
 *
 */
TEST_F(TestLocateTwoClosest_Equivalence, Test_Num_Samples_1000_Iter_10)
{
    executeSubtest(NUM_POINTS_1000, NUM_ITERATIONS_1000);
}
