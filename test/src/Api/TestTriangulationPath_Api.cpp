/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "pathLib.h"
#include "PointsReader.h"
#include "TestSuiteReader.h"

#include <gtest/gtest.h>


/***********************************************************************************************************************
* Test Class Definition
***********************************************************************************************************************/
namespace
{
    class TestTriangulationPath_Api : public ::testing::Test
    {
        string strTestFilename;
        string strTestPointsFolder;
        string strTestFolder;
    protected:

        // You can remove any or all of the following functions if its body is empty.
        // Constructor (called before each test case) - SetUp
        TestTriangulationPath_Api()
        {
            string strBaseFolder = BASEFOLDER;
            strTestFolder = strBaseFolder + "/data/TestData/TriangulationPathGolden/";
            strTestPointsFolder = strBaseFolder + "/data/TestData/PointsGolden/";
            strTestFilename = strTestFolder + "testInfo.txt";
        }

    public:

        /**
         * @fn      checkGold
         * @brief   Reads a set of precomputed triangulation path and their associated set of points. For each
         *          set of points it reads two additional points and computes the triangulation path between both
         *          points and compares it with the triangulation read.
         */
        void checkGold();
    };

    void TestTriangulationPath_Api::checkGold()
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
            ASSERT_TRUE(vPoints.size() >= 3);

            // Get extreme points input file
            isSuccess = test.getValue(TEST_EXTREME_POINTS_IN_FIELD, strFilename);
            string strExtremePointsFilename = strTestFolder + strFilename;
            ASSERT_TRUE(isSuccess);

            // Read extreme points from input file
            vector<Point<TYPE>> vExtremePoints;
            isSuccess = PointsReader::read(strExtremePointsFilename, vExtremePoints);
            ASSERT_TRUE(isSuccess);
            ASSERT_EQ(vExtremePoints.size(), 2);

            // Get triangulation path file name
            isSuccess = test.getValue(TEST_POINTS_OUT_FIELD, strFilename);
            string strPathFilename = strTestFolder + strFilename;
            ASSERT_TRUE(isSuccess);

            // Read triangulation path
            vector<Point<TYPE>> vPointsComputedPath;
            isSuccess = PointsReader::read(strPathFilename, vPointsComputedPath);
            ASSERT_TRUE(isSuccess);

            // Compute triangulation pah
            vector<Point<TYPE>> vPointsPath;
            isSuccess = getTriangulationPath(vExtremePoints[0], vExtremePoints[1], vPoints, vPointsPath);
            ASSERT_TRUE(isSuccess);

            // Check Delaunay triangulations are equal
            ASSERT_TRUE(vPointsComputedPath == vPointsPath);
        }
    }
}

/**
 * @brief   Reads a set of precomputed triangulation path and their associated set of points. For each
 *          set of points it reads two additional points and computes the triangulation path between both
 *          points and compares it with the triangulation read.
 */
TEST_F(TestTriangulationPath_Api, Test_Delaunay_Gold)
{
    checkGold();
}
