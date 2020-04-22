/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "figuresLib.h"
#include "PointFactory.h"
#include "PointsReader.h"
#include "TestSuiteReader.h"
#include "VoronoiFactory.h"
#include "VoronoiIO.h"

#include <gtest/gtest.h>
#include <iostream>


/***********************************************************************************************************************
* Test Class Definition
***********************************************************************************************************************/
namespace
{
    class TestVoronoi_Api : public ::testing::Test
    {
        string strTestFilename;
        string strTestFolder;
    protected:

        // You can remove any or all of the following functions if its body
        // is empty.
        // Constructor (called before each test case) - SetUp
        TestVoronoi_Api()
        {
            string strBaseFolder = BASEFOLDER;
            strTestFolder = strBaseFolder + "/data/TestData/VoronoiGolden/";
            strTestFilename = strTestFolder + "testInfo.txt";
        }

    public:

        /**
         * @fn      executeTwice
         * @brief   Generates a set of random points and computes two Voronoi triangulations using that set of points.
         *          Then it compares both triangulations to be equal. The number of points is defined by szNumPoints
         *          and it is executed szNumIterations times
         *
         * @param   szNumPoints         (IN) Points set number of points
         * @param   szNumIterations     (IN) Number of times the triangulations are computed
         */
        static void executeTwice(size_t szNumPoints, size_t szNumIterations);

        /**
         * @fn      checkGold
         * @brief   Reads a set of precomputed Voronoi triangulation and using their set of points computes the
         *          Voronoi triangulation and checks both are equal, the computed and the precomputed triangulations
         */
        void checkGold();
    };


    void TestVoronoi_Api::executeTwice(size_t szNumPoints, size_t szNumIterations)
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

            // Build first Voronoi triangulation
            bool isSuccess;
            Dcel dcelOut1;
            isSuccess = getVoronoi(vPointsFirst, dcelOut1);
            ASSERT_TRUE(isSuccess);

            // Build second Voronoi triangulation
            Dcel dcelOut2;
            isSuccess = getVoronoi(vPointsSecond, dcelOut2);
            ASSERT_TRUE(isSuccess);

            // Check Voronoi triangulations are equal
            ASSERT_TRUE(dcelOut1 == dcelOut2);
        }
    }

    void TestVoronoi_Api::checkGold()
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
            string strFileName;
            isSuccess = test.getValue(TEST_POINTS_IN_FIELD, strFileName);
            string strPointsFilename = strTestFolder + strFileName;
            ASSERT_TRUE(isSuccess);

            // Read points from input file
            vector<Point<TYPE>> vPoints;
            isSuccess = PointsReader::read(strPointsFilename, vPoints);
            ASSERT_TRUE(isSuccess);

            // Get Voronoi files
            isSuccess = test.getValue(TEST_VORONOI_OUT_FIELD, strFileName);
            string strVoronoiFilename = strTestFolder + strFileName;
            ASSERT_TRUE(isSuccess);

            // Read Voronoi from file
            Voronoi goldenVoronoi;
            isSuccess = VoronoiIO::read(strVoronoiFilename, goldenVoronoi);
            ASSERT_TRUE(isSuccess);

            // Build Voronoi using input points
            Dcel dcelOut;
            isSuccess = getVoronoi(vPoints, dcelOut);
            ASSERT_TRUE(isSuccess);

            // Check Voronoi triangulations are equal
            ASSERT_TRUE(*goldenVoronoi.getRefDcel() == dcelOut);
        }
    }
}


/**
 * @brief   Create two Voronoi diagrams using the same points set and compare the results are the same
 *          The number of points and number of times the Voronoi is computed changes for every execution
 *
 */
TEST_F(TestVoronoi_Api, Test_Voronoi_Twice)
{
    executeTwice(NUM_POINTS_3, NUM_ITERATIONS_1000);
    executeTwice(NUM_POINTS_10, NUM_ITERATIONS_100);
    executeTwice(NUM_POINTS_1K, NUM_ITERATIONS_10);
}

/**
 * @brief   Reads a set of precomputed Voronoi diagrams and using their set of points computes the Voronoi
 *          diagram and checks both are equal, the computed and the precomputed Voronoi diagrams
 */
TEST_F(TestVoronoi_Api, Test_Voronoi_Gold)
{
    checkGold();
}
