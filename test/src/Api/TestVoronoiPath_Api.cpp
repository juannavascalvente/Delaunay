/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "pathLib.h"
#include "PointsReader.h"
#include "TestSuiteReader.h"

#include <gtest/gtest.h>
#include <iostream>


/***********************************************************************************************************************
* Test Class Definition
***********************************************************************************************************************/
namespace
{
    class TestVoronoiPath_Api : public ::testing::Test
    {
        string strTestFilename;
        string strTestPointsFolder;
        string strTestFolder;
    protected:

        // You can remove any or all of the following functions if its body
        // is empty.
        // Constructor (called before each test case) - SetUp
        TestVoronoiPath_Api()
        {
            string strBaseFolder = BASEFOLDER;
            strTestFolder = strBaseFolder + "/data/TestData/VoronoiPathGolden/";
            strTestPointsFolder = strBaseFolder + "/data/TestData/PointsGolden/";
            strTestFilename = strTestFolder + "testInfo.txt";
        }

    public:

        /**
         * @fn      checkGold
         * @brief   Reads a set of precomputed Voronoi diagrams, two points and computes the Voronoi path between both
         *          points. Then it compares the result with an already computed Voronoi path and checks if both are
         *          equals.
         */
        void checkGold();
    };

    void TestVoronoiPath_Api::checkGold()
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
            isSuccess = test.getValue(TEST_POINTS_SET_IN_FIELD, strFileName);
            string strPointsFilename = strTestPointsFolder + strFileName;
            ASSERT_TRUE(isSuccess);

            // Read points from input file
            vector<Point<TYPE>> vPoints;
            isSuccess = PointsReader::read(strPointsFilename, vPoints);
            ASSERT_TRUE(isSuccess);

            // Get extreme points input file
            isSuccess = test.getValue(TEST_EXTREME_POINTS_IN_FIELD, strFileName);
            string strExtremePointsFilename = strTestFolder + strFileName;
            ASSERT_TRUE(isSuccess);

            // Read extreme points from input file
            vector<Point<TYPE>> vExtremePoints;
            isSuccess = PointsReader::read(strExtremePointsFilename, vExtremePoints);
            ASSERT_TRUE(isSuccess);
            ASSERT_EQ(vExtremePoints.size(), 2);

            // Get Voronoi path file name
            isSuccess = test.getValue(TEST_POINTS_OUT_FIELD, strFileName);
            string strVoronoiFilename = strTestFolder + strFileName;
            ASSERT_TRUE(isSuccess);

            // Read Voronoi path
            vector<Point<TYPE>> vGoldenPathPoints;
            isSuccess = PointsReader::read(strVoronoiFilename, vGoldenPathPoints);
            ASSERT_TRUE(isSuccess);

            // Compute Voronoi path
            vector<FaceT> vFaces;
            isSuccess = getVoronoiPath(vExtremePoints[0], vExtremePoints[1], vPoints, vFaces);
            ASSERT_TRUE(isSuccess);

            // Extract faces points
            vector<Point<TYPE>> vPointsPath;
            for (const auto& face : vFaces)
            {
                for (auto point : face)
                {
                    vPointsPath.push_back(point);
                }
            }

            // Check Voronoi triangulations are equal
            ASSERT_TRUE(vPointsPath == vGoldenPathPoints);
        }
    }
}

/**
 * @brief   Reads a set of points, two points and computes the Voronoi path of the given set between both
 *          points. Then it compares the result with an already computed Voronoi path and checks if both are
 *          equals.
 */
TEST_F(TestVoronoiPath_Api, Test_Voronoi_Gold)
{
    checkGold();
}
