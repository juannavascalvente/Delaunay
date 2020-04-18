/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Delaunay.h"
#include "DelaunayIO.h"
#include "PointFactory.h"
#include "PointsReader.h"
#include "TestSuiteReader.h"

#include <gtest/gtest.h>
#include <iostream>

/***********************************************************************************************************************
* Defines
***********************************************************************************************************************/



/***********************************************************************************************************************
* Test Class Definition
***********************************************************************************************************************/
namespace
{
    class TestDelaunay_Api : public ::testing::Test
    {
        string strTestFilename;
        string strTestFolder;
    protected:

        // You can remove any or all of the following functions if its body
        // is empty.
        // Constructor (called before each test case) - SetUp
        TestDelaunay_Api()
        {
            string strBaseFolder = BASEFOLDER;
            strTestFolder = strBaseFolder + "/data/TestData/DelaunayGolden/";
            strTestFilename = strTestFolder + "testInfo.txt";
        }

        // Deconstructor (called after each test case) - TearDown
        ~TestDelaunay_Api() override = default;

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
        void executeTwice(size_t szNumPoints, size_t szNumIterations);
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
            auto *delaunayFirst = new Delaunay(vPointsFirst);
            bool isSuccess = delaunayFirst->build();
            ASSERT_TRUE(isSuccess);

            // Build second Delaunay triangulation
            auto *delaunaySecond = new Delaunay(vPointsSecond);
            isSuccess = delaunaySecond->build();
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
            string strPointsFilename;
            isSuccess = test.getValue(TEST_POINTS_IN_FIELD, strPointsFilename);
            strPointsFilename = strTestFolder + strPointsFilename;
            ASSERT_TRUE(isSuccess);

            // Read points from input file
            vector<Point<TYPE>> vPoints;
            isSuccess = PointsReader::read(strPointsFilename, vPoints);
            ASSERT_TRUE(isSuccess);

            // Get Delaunay files
            string strDelaunayFilename;
            isSuccess = test.getValue(TEST_DELAUNAY_OUT_FIELD, strDelaunayFilename);
            strDelaunayFilename = strTestFolder + strDelaunayFilename;
            ASSERT_TRUE(isSuccess);
            string strGraphFilename;
            isSuccess = test.getValue(TEST_DELAUNAY_GRAPH_OUT_FIELD, strGraphFilename);
            strGraphFilename = strTestFolder + strGraphFilename;
            ASSERT_TRUE(isSuccess);

            // Read Delaunay from file
            Delaunay goldenDelaunay;
            isSuccess = DelaunayIO::read(strDelaunayFilename, strGraphFilename, goldenDelaunay);
            ASSERT_TRUE(isSuccess);

            // Build Delaunay using input points
            auto *delaunay = new Delaunay(vPoints);
            isSuccess = delaunay->build();
            ASSERT_TRUE(isSuccess);

            // Check Delaunay triangulations are equal
            ASSERT_TRUE(*goldenDelaunay.getRefDcel() == *delaunay->getRefDcel());

            // Free resources
            delete delaunay;
        }
    }
}


/**
 * @brief   Create two Delaunay triangulations using the same points set and compare the results are the same
 *          The points set contains 1K points and it is executed 10 times
 *
 */
//TEST_F(TestDelaunay_Api, Test_Delaunay_Twice)
//{
//    executeTwice(NUM_POINTS_3, NUM_ITERATIONS_1000);
//    executeTwice(NUM_POINTS_10, NUM_ITERATIONS_100);
//    executeTwice(NUM_POINTS_1000, NUM_ITERATIONS_10);
//}

TEST_F(TestDelaunay_Api, Test_Delaunay_Gold)
{
    checkGold();
}
