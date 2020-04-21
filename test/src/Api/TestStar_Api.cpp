/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "DcelReader.h"
#include "DelaunayIO.h"
#include "PointFactory.h"
#include "PointsReader.h"
#include "TriangulationFactory.h"
#include "TestSuiteReader.h"

#include <gtest/gtest.h>
#include <iostream>


/***********************************************************************************************************************
* Test Class Definition
***********************************************************************************************************************/
namespace
{
    class TestStar_Api : public ::testing::Test
    {
        string strTestFilename;
        string strTestFolder;
        string strTestPointsFolder;
    protected:

        // You can remove any or all of the following functions if its body
        // is empty.
        // Constructor (called before each test case) - SetUp
        TestStar_Api()
        {
            string strBaseFolder = BASEFOLDER;
            strTestFolder = strBaseFolder + "/data/TestData/StarGolden/";
            strTestPointsFolder = strBaseFolder + "/data/TestData/PointsGolden/";
            strTestFilename = strTestFolder + "testInfo.txt";
        }

    public:

        /**
         * @fn      executeTwice
         * @brief   Generates a set of random points and computes two Star triangulations using that set of points.
         *          Then it compares both triangulations to be equal. The number of points is defined by szNumPoints
         *          and it is executed szNumIterations times
         *
         * @param   szNumPoints         (IN) Points set number of points
         * @param   szNumIterations     (IN) Number of times the triangulations are computed
         */
        static void executeTwice(size_t szNumPoints, size_t szNumIterations);

        /**
         * @fn      checkGold
         * @brief   Reads a set of precomputed Star triangulation and using their set of points computes the
         *          Star triangulation and checks both are equal, the computed and the precomputed triangulations
         */
        void checkGold();
    };


    void TestStar_Api::executeTwice(size_t szNumPoints, size_t szNumIterations)
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

            // Build first Star triangulation
            bool isSuccess;
            auto *starFirst = TriangulationFactory::createStar(vPointsFirst, isSuccess);
            ASSERT_TRUE(isSuccess);

            // Build second Star triangulation
            auto *starSecond = TriangulationFactory::createStar(vPointsSecond, isSuccess);
            ASSERT_TRUE(isSuccess);

            // Check Star triangulations are equal
            ASSERT_TRUE(*starFirst->getRefDcel() == *starSecond->getRefDcel());

            // Free resources
            delete starFirst;
            delete starSecond;
        }
    }

    void TestStar_Api::checkGold()
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

            // Get Star file
            isSuccess = test.getValue(TEST_STAR_OUT_FIELD, srFileName);
            string strStarFilename = strTestFolder + srFileName;
            ASSERT_TRUE(isSuccess);

            // Read Star from file
            auto *goldenStar = new StarTriangulation();
            isSuccess = DcelReader::read(strStarFilename, *goldenStar->getRefDcel());
            ASSERT_TRUE(isSuccess);

            // Build Star using input points
            auto *computedStar = TriangulationFactory::createStar(vPoints, isSuccess);
            ASSERT_TRUE(isSuccess);

            // Check Star triangulations are equal
            ASSERT_TRUE(*goldenStar->getRefDcel() == *computedStar->getRefDcel());

            // Free resources
            delete computedStar;
            delete goldenStar;
        }
    }
}


/**
 * @brief   Create two Star triangulations using the same points set and compare the results are the same
 *          The number of points and number of times the Star is computed changes for every execution
 *
 */
TEST_F(TestStar_Api, Test_Star_Twice)
{
    executeTwice(NUM_POINTS_3, NUM_ITERATIONS_1000);
    executeTwice(NUM_POINTS_10, NUM_ITERATIONS_100);
    executeTwice(NUM_POINTS_1K, NUM_ITERATIONS_10);
}

/**
 * @brief   Reads a set of precomputed Star triangulation and using their set of points computes the Star
 *          triangulation and checks both are equal, the computed and the precomputed triangulations
 */
TEST_F(TestStar_Api, Test_Star_Gold)
{
    checkGold();
}
