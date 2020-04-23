/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "locateLib.h"
#include "PointsReader.h"
#include "TestSuiteReader.h"

#include <gtest/gtest.h>


/***********************************************************************************************************************
* Test Class Definition
***********************************************************************************************************************/
namespace
{
    class TestClosest_Api : public ::testing::Test
    {
        string strTestFilename;
        string strTestFolder;
        string strTestPointsFolder;
    protected:

        // You can remove any or all of the following functions if its body is empty.
        // Constructor (called before each test case) - SetUp
        TestClosest_Api()
        {
            string strBaseFolder = BASEFOLDER;
            strTestFolder = strBaseFolder + "/data/TestData/ClosestPointGolden/";
            strTestPointsFolder = strBaseFolder + "/data/TestData/PointsGolden/";
            strTestFilename = strTestFolder + "testInfo.txt";
        }

    public:

        /**
         * @fn      checkGold
         * @brief   Reads a set of points and an input point and computes the closest point in the points set
         */
        void checkGold();
    };

    void TestClosest_Api::checkGold()
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
            string strPointsFilename = strTestPointsFolder + strFilename;
            ASSERT_TRUE(isSuccess);

            // Read points from input file
            vector<Point<TYPE>> vPoints;
            isSuccess = PointsReader::read(strPointsFilename, vPoints);
            ASSERT_TRUE(isSuccess);

            // Get input point file name
            isSuccess = test.getValue(TEST_SINGLE_POINT_IN_FIELD, strFilename);
            string strPointFilename = strTestFolder + strFilename;
            ASSERT_TRUE(isSuccess);

            // Read input point
            vector<Point<TYPE>> vPointLocate;
            isSuccess = PointsReader::read(strPointFilename, vPointLocate);
            ASSERT_TRUE(isSuccess);

            // Get closest point file name
            isSuccess = test.getValue(TEST_POINTS_OUT_FIELD, strFilename);
            string strTwoClosestFilename = strTestFolder + strFilename;
            ASSERT_TRUE(isSuccess);

            // Read closest point
            vector<Point<TYPE>> vGoldenClosestPoints;
            isSuccess = PointsReader::read(strTwoClosestFilename, vGoldenClosestPoints);
            ASSERT_TRUE(isSuccess);

            // Compute closest point
            Point<TYPE> computedClosest;
            isSuccess = getClosestPoint(vPointLocate[0], vPoints, computedClosest);
            ASSERT_TRUE(isSuccess);

            // Check points are equal
            ASSERT_EQ(computedClosest, vGoldenClosestPoints[0]);
        }
    }
}


/**
 * @brief   Reads a set of points and an input point and computes the closest point in the points set
 */
TEST_F(TestClosest_Api, Test_Closest_Gold)
{
    checkGold();
}
