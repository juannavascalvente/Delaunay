/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Delaunay.h"
#include "DelaunayIO.h"
#include "PointFactory.h"
#include "PointsReader.h"
#include "TriangulationFactory.h"
#include "triangulationLib.h"
#include "TestSuiteReader.h"

#include <gtest/gtest.h>
#include <iostream>


/***********************************************************************************************************************
* Test Class Definition
***********************************************************************************************************************/
namespace
{
    class TestDelaunay_Api : public ::testing::Test
    {
        string strTestFilename;
        string strTestFolder;
        string strTestPointsFolder;
    protected:

        // You can remove any or all of the following functions if its body
        // is empty.
        // Constructor (called before each test case) - SetUp
        TestDelaunay_Api()
        {
            string strBaseFolder = BASEFOLDER;
            strTestPointsFolder = strBaseFolder + "/data/TestData/PointsGolden/";
            strTestFolder = strBaseFolder + "/data/TestData/DelaunayGolden/";
            strTestFilename = strTestFolder + "testInfo.txt";
        }

    public:

        /**
         * @fn      executeTwice
         * @brief   Generates a set of random points and computes two Delaunay triangulations using that set of points.
         *          Then it compares both triangulations to be equal. The number of points is defined by szNumPoints
         *          and it is executed szNumIterations times
         *
         * @param   szNumPoints         (IN) Points set number of points
         * @param   szNumIterations     (IN) Number of times the triangulations are computed
         */
        static void executeTwice(size_t szNumPoints, size_t szNumIterations);

        /**
         * @fn      checkGold
         * @brief   Reads a set of precomputed Delaunay triangulation and using their set of points computes the
         *          Delaunay triangulation and checks both are equal, the computed and the precomputed triangulations
         */
        void checkGold();
    };


    void TestDelaunay_Api::executeTwice(size_t szNumPoints, size_t szNumIterations)
    {
        // Execute test szNumIterations times
        for (size_t i=0; i<szNumIterations ; i++)
        {
            // Two points set
            vector<Point<TYPE>> vPointsFirst;
            vector<Point<TYPE>> vPointsSecond;

            // Generate random points set
            PointFactory::generateRandom(szNumPoints, vPointsFirst);
            vPointsSecond = vPointsFirst;

            // Build first Delaunay triangulation
            bool isSuccess;
            auto *delaunayFirst = TriangulationFactory::createDelaunay(vPointsFirst, isSuccess);
            ASSERT_TRUE(isSuccess);

            // Build second Delaunay triangulation
            auto *delaunaySecond = TriangulationFactory::createDelaunay(vPointsSecond, isSuccess);
            ASSERT_TRUE(isSuccess);

            // Check Delaunay triangulations are equal
            ASSERT_TRUE(*delaunayFirst->getRefDcel() == *delaunaySecond->getRefDcel());

            // Free resources
            delete delaunaySecond;
            delete delaunayFirst;
        }
    }

    void TestDelaunay_Api::checkGold()
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
            string srFileName;
            isSuccess = test.getValue(TEST_POINTS_IN_FIELD, srFileName);
            string strPointsFilename = strTestPointsFolder + srFileName;
            ASSERT_TRUE(isSuccess);

            // Read points from input file
            vector<Point<TYPE>> vPoints;
            isSuccess = PointsReader::read(strPointsFilename, vPoints);
            ASSERT_TRUE(isSuccess);

            // Get Delaunay files
            isSuccess = test.getValue(TEST_DELAUNAY_OUT_FIELD, srFileName);
            string strDelaunayFilename = strTestFolder + srFileName;
            ASSERT_TRUE(isSuccess);
            isSuccess = test.getValue(TEST_DELAUNAY_GRAPH_OUT_FIELD, srFileName);
            string strGraphFilename = strTestFolder + srFileName;
            ASSERT_TRUE(isSuccess);

            // Read Delaunay from file
            Delaunay goldenDelaunay;
            isSuccess = DelaunayIO::read(strDelaunayFilename, strGraphFilename, goldenDelaunay);
            ASSERT_TRUE(isSuccess);

            // Build Delaunay using input points
            Dcel dcel;
            isSuccess = getDelaunay(vPoints, dcel);
            ASSERT_TRUE(isSuccess);

            // Check Delaunay triangulations are equal
            ASSERT_TRUE(*goldenDelaunay.getRefDcel() == dcel);
        }
    }
}


/**
 * @brief   Create two Delaunay triangulations using the same points set and compare the results are the same
 *          The number of points and number of times the Delaunay is computed changes for every execution
 *
 */
TEST_F(TestDelaunay_Api, Test_Delaunay_Twice)
{
    executeTwice(NUM_POINTS_3, NUM_ITERATIONS_1000);
    executeTwice(NUM_POINTS_10, NUM_ITERATIONS_100);
    executeTwice(NUM_POINTS_1K, NUM_ITERATIONS_10);
}

/**
 * @brief   Reads a set of precomputed Delaunay triangulation and using their set of points computes the Delaunay
 *          triangulation and checks both are equal, the computed and the precomputed triangulations
 */
TEST_F(TestDelaunay_Api, Test_Delaunay_Gold)
{
    checkGold();
}
