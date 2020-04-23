/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "locateLib.h"
#include "PointsReader.h"
#include "Polygon.h"
#include "TestSuiteReader.h"

#include <gtest/gtest.h>


/***********************************************************************************************************************
* Test Class Definition
***********************************************************************************************************************/
namespace
{
    class TestLocateFace_Api : public ::testing::Test
    {
        string strTestFilename;
        string strTestFolder;
        string strTestPointsFolder;
    protected:

        // You can remove any or all of the following functions if its body is empty.
        // Constructor (called before each test case) - SetUp
        TestLocateFace_Api()
        {
            string strBaseFolder = BASEFOLDER;
            strTestFolder = strBaseFolder + "/data/TestData/LocateFaceGolden/";
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

    void TestLocateFace_Api::checkGold()
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

            // Get point file name
            string strFilename;
            isSuccess = test.getValue(TEST_SINGLE_POINT_IN_FIELD, strFilename);
            string strPointFaceFilename = strTestFolder + strFilename;
            ASSERT_TRUE(isSuccess);

            // Get point to locate
            vector<Point<TYPE>> vPointLocate;
            isSuccess = PointsReader::read(strPointFaceFilename, vPointLocate);
            ASSERT_TRUE(isSuccess);

            // Get points file name
            isSuccess = test.getValue(TEST_POINTS_SET_IN_FIELD, strFilename);
            string strPointsFilename = strTestPointsFolder + strFilename;
            ASSERT_TRUE(isSuccess);

            // Read points from input file
            vector<Point<TYPE>> vPoints;
            isSuccess = PointsReader::read(strPointsFilename, vPoints);
            ASSERT_TRUE(isSuccess);

            // Get face points file name
            isSuccess = test.getValue(TEST_POINTS_OUT_FIELD, strFilename);
            string strTwoClosestFilename = strTestFolder + strFilename;
            ASSERT_TRUE(isSuccess);

            // Read face points
            vector<Point<TYPE>> vOriginalPoints;
            isSuccess = PointsReader::read(strTwoClosestFilename, vOriginalPoints);
            ASSERT_TRUE(isSuccess);

            // Get two closest in set of points
            vector<Point<TYPE>> vComputedPoints;
            isSuccess = getPointFace(vPointLocate.at(0), vPoints, vComputedPoints);
            ASSERT_TRUE(isSuccess);

            // Check face points triangulations are equal
            Polygon originalPolygon(vOriginalPoints);
            Polygon computedPolygon(vComputedPoints);
            ASSERT_TRUE(originalPolygon == computedPolygon);
        }
    }
}


/**
 * @brief   Reads a set of points and computes the two closest point in each set and checks if they are equal
 *          to the already computed two closest points
 */
TEST_F(TestLocateFace_Api, Test_Delaunay_Gold)
{
    checkGold();
}
