/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "FigureIO.h"
#include "figuresLib.h"
#include "PointFactory.h"
#include "PointsReader.h"
#include "TriangulationFactory.h"
#include "TestSuiteReader.h"

#include <gtest/gtest.h>


/***********************************************************************************************************************
* Test Class Definition
***********************************************************************************************************************/
namespace
{
    class TestConvexHull_Api : public ::testing::Test
    {
        string strTestFilename;
        string strTestFolder;
    protected:

        // You can remove any or all of the following functions if its body is empty.
        // Constructor (called before each test case) - SetUp
        TestConvexHull_Api()
        {
            string strBaseFolder = BASEFOLDER;
            strTestFolder = strBaseFolder + "/data/TestData/ConvexHullGolden/";
            strTestFilename = strTestFolder + "testInfo.txt";
        }

    public:

        /**
         * @fn      executeTwice
         * @brief   Generates a set of random points and computes the convex hull using a star and a Delaunay
         *          triangulations. Then it compares both convex hull are equal. The number of points is defined by
         *          szNumPoints and it is executed szNumIterations times
         *
         * @param   szNumPoints         (IN) Points set number of points
         * @param   szNumIterations     (IN) Number of times the convex hull are computed
         */
        static void executeTwice(size_t szNumPoints, size_t szNumIterations);

        /**
         * @fn      checkGold
         * @brief   Reads a set of precomputed convex hulls, computes again for each one the convex hull and checks
         *          both are equal, the read convex hull and the computed
         */
        void checkGold();
    };


    void TestConvexHull_Api::executeTwice(size_t szNumPoints, size_t szNumIterations)
    {
        // Execute test szNumIterations times
        for (size_t i=0; i<szNumIterations ; i++)
        {
            // Two points set
            vector<Point<TYPE>> vPointsFirst;
            vector<Point<TYPE>> vPointsFirstConvexHull;
            vector<Point<TYPE>> vPointsSecond;
            vector<Point<TYPE>> vPointsSecondConvexHull;

            // Generate random points set
            PointFactory::generateRandom(szNumPoints, vPointsFirst);
            vPointsSecond = vPointsFirst;

            // Build first convex hull
            bool isSuccess;
            isSuccess = getConvexHull(vPointsFirst, vPointsFirstConvexHull);
            ASSERT_TRUE(isSuccess);

            // Build first convex hull
            isSuccess = getConvexHull(vPointsSecond, vPointsSecondConvexHull);
            ASSERT_TRUE(isSuccess);

            // Check convex hulls are equal
            ASSERT_TRUE(vPointsFirstConvexHull == vPointsSecondConvexHull);
        }
    }

    void TestConvexHull_Api::checkGold()
    {
        // Read test suite
        TestSuite suite;
        bool isSuccess = TestSuiteReader::read(strTestFilename, suite);
        ASSERT_TRUE(isSuccess);

        cout << "Executing test suite " << suite.getName() << endl;
        for (size_t szIdx=0; szIdx<suite.getNumTests() ; szIdx++)
        {
            TestData test = suite.getTest(szIdx);

            // Get test name
            string strTestName;
            isSuccess = test.getValue(TEST_NAME_FIELD, strTestName);
            ASSERT_TRUE(isSuccess);
            cout << "Test " << (szIdx+1) << "/" << suite.getNumTests() << ". Name:\t" << strTestName << endl;

            // Get points file name
            string strFilename;
            isSuccess = test.getValue(TEST_POINTS_SET_IN_FIELD, strFilename);
            string strPointsFilename = strTestFolder + strFilename;
            ASSERT_TRUE(isSuccess);

            // Read points from input file
            vector<Point<TYPE>> vPoints;
            isSuccess = PointsReader::read(strPointsFilename, vPoints);
            ASSERT_TRUE(isSuccess);

            // Get convex hull file name
            isSuccess = test.getValue(TEST_CONVEX_HULL_OUT_FIELD, strFilename);
            string strHullFilename = strTestFolder + strFilename;
            ASSERT_TRUE(isSuccess);

            // Read convex hull
            Polygon readHull;
            isSuccess = FigureIO::read(strHullFilename, readHull);
            ASSERT_TRUE(isSuccess);
            vector<Point<TYPE>> vPointsConvexHullOriginal;
            readHull.getPoints(vPointsConvexHullOriginal);

            // Build Delaunay triangulation
            // Build first convex hull
            vector<Point<TYPE>> vPointsConvexHull;
            isSuccess = getConvexHull(vPoints, vPointsConvexHull);
            ASSERT_TRUE(isSuccess);

            // Check Delaunay triangulations are equal
            ASSERT_TRUE(vPointsConvexHull == vPointsConvexHullOriginal);
        }
    }
}


/**
 * @brief   Create two Delaunay triangulations using the same points set and compare the results are the same
 *          The number of points and number of times the Delaunay is computed changes for every execution
 *
 */
TEST_F(TestConvexHull_Api, Test_Delaunay_Twice)
{
    executeTwice(NUM_POINTS_3, NUM_ITERATIONS_1000);
    executeTwice(NUM_POINTS_10, NUM_ITERATIONS_100);
    executeTwice(NUM_POINTS_1K, NUM_ITERATIONS_10);
}

/**
 * @brief   Reads a set of precomputed Delaunay triangulation and using their set of points computes the Delaunay
 *          triangulation and checks both are equal, the computed and the precomputed triangulations
 */
TEST_F(TestConvexHull_Api, Test_Delaunay_Gold)
{
    checkGold();
}
