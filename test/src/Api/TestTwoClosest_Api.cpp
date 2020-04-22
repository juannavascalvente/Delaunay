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
    class TestTwoClosest_Api : public ::testing::Test
    {
        string strTestFilename;
        string strTestFolder;
        string strTestPointsFolder;
    protected:

        // You can remove any or all of the following functions if its body is empty.
        // Constructor (called before each test case) - SetUp
        TestTwoClosest_Api()
        {
            string strBaseFolder = BASEFOLDER;
            strTestFolder = strBaseFolder + "/data/TestData/TwoClosestGolden/";
            strTestPointsFolder = strBaseFolder + "/data/TestData/PointsGolden/";
            strTestFilename = strTestFolder + "testInfo.txt";
        }

    public:

        /**
         * @fn      checkGold
         * @brief   Reads a set of points and computes the two closest point in each set and checks if they are equal
         *          to the already computed two closest points
         */
        void checkGold();
    };

    void TestTwoClosest_Api::checkGold()
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
            isSuccess = test.getValue(TEST_POINTS_IN_FIELD, strFilename);
            string strPointsFilename = strTestPointsFolder + strFilename;
            ASSERT_TRUE(isSuccess);

            // Read points from input file
            vector<Point<TYPE>> vPoints;
            isSuccess = PointsReader::read(strPointsFilename, vPoints);
            ASSERT_TRUE(isSuccess);

            // Get two closest points file name
            isSuccess = test.getValue(TEST_TWO_POINTS_OUT_FIELD, strFilename);
            string strTwoClosestFilename = strTestFolder + strFilename;
            ASSERT_TRUE(isSuccess);

            // Read two closest points
            vector<Point<TYPE>> vOriginalPoints;
            isSuccess = PointsReader::read(strTwoClosestFilename, vOriginalPoints);
            ASSERT_TRUE(isSuccess);

            // Get two closest in set of points
            Point<TYPE> computedPoint1;
            Point<TYPE> computedPoint2;
            isSuccess =  get2ClosestPoints(vPoints, computedPoint1, computedPoint2);
            ASSERT_TRUE(isSuccess);

            // Check Delaunay triangulations are equal
            if (computedPoint1 == vOriginalPoints[0])
            {
                ASSERT_EQ(computedPoint2, vOriginalPoints[1]);
            }
            else
            {
                ASSERT_NE(computedPoint1, vOriginalPoints[1]);
                ASSERT_EQ(computedPoint2, vOriginalPoints[0]);
            }
        }
    }
}


/**
 * @brief   Reads a set of points and computes the two closest point in each set and checks if they are equal
 *          to the already computed two closest points
 */
TEST_F(TestTwoClosest_Api, Test_Delaunay_Gold)
{
    checkGold();
}
