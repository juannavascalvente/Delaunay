/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <gtest/gtest.h>

#include "PointFactory.h"
#include "PointsWriter.h"
#include "StarTriangulation.h"

#include <cstdio>
using namespace std;


/***********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#define NUM_ELEMS_10                  (10)
#define NUM_ELEMS_100                 (100)
#define NUM_ELEMS_1000                (1000)
#define NUM_ELEMS_10000               (10000)
#define NUM_ELEMS_100000              (100000)
#define NUM_ELEMS_1000000             (1000000)


/***********************************************************************************************************************
* Test Class Definition
***********************************************************************************************************************/
namespace
{

    class TestTriangulationPath_Integration : public ::testing::Test
    {
        int         iFaceId{};               // Output face
        vector<Point<TYPE>> vPoints;        // Points in set
        Point<TYPE> point;                  // Path origin point

        // Log information file names
        string strPointFileName = "Point.txt";
        string strPointSetFileName = "PointSet.txt";

    protected:

        // Constructor (called before each test case) - SetUp
        TestTriangulationPath_Integration()
        {
            // Check if input point parameter provided by user.
            point.random();

            vector<Point<TYPE>> vPointToLocate;
            vPointToLocate.push_back(point);
            PointsWriter::write(strPointFileName, vPointToLocate);
        };
        ~TestTriangulationPath_Integration() override
        {
            remove(strPointFileName.c_str());
        };

    public:
        void executeSubtest(size_t szNumPoints, size_t szNumIterations);
    };


    void TestTriangulationPath_Integration::executeSubtest(size_t szNumPoints, size_t szNumIterations)
    {
        // Execute this test szNumIterations times
        for (size_t i=0; i<szNumIterations ; i++)
        {
            // Create set of points
            bool isRunSuccess;
            PointFactory::generateRandom(szNumPoints, vPoints);

            // Write data before test is executed
            PointsWriter::write(strPointSetFileName, vPoints);

            // Create triangulation
            auto *triangulation = new StarTriangulation(vPoints);
            triangulation->build();

            // Execute function to test
            isRunSuccess = triangulation->findFace(point, iFaceId);

            // Check result
            EXPECT_TRUE(isRunSuccess);
            cout << "Success " << szNumPoints << " points. Iter: " << (i+1) << "/" << szNumIterations << endl;

            // Free resources
            delete triangulation;
            remove(strPointSetFileName.c_str());
        }
    }

    /**
     * DESCRIPTION:
     *
     */
    TEST_F(TestTriangulationPath_Integration, Test_NUM_ELEMS_10_NUM_ELEMS_1000000)
    {
        executeSubtest(NUM_ELEMS_10, NUM_ELEMS_1000000);
    }

    /**
     * DESCRIPTION:
     *
     */
    TEST_F(TestTriangulationPath_Integration, Test_NUM_ELEMS_100_NUM_ELEMS_100000)
    {
        executeSubtest(NUM_ELEMS_100, NUM_ELEMS_100000);
    }

    /**
     * DESCRIPTION:
     *
     */
    TEST_F(TestTriangulationPath_Integration, Test_NUM_ELEMS_1000_NUM_ELEMS_10000)
    {
        executeSubtest(NUM_ELEMS_1000, NUM_ELEMS_10000);
    }

    /**
     * DESCRIPTION:
     *
     */
    TEST_F(TestTriangulationPath_Integration, Test_NUM_ELEMS_10000_NUM_ELEMS_1000)
    {
        executeSubtest(NUM_ELEMS_10000, NUM_ELEMS_1000);
    }

    /**
     * DESCRIPTION:
     *
     */
    TEST_F(TestTriangulationPath_Integration, Test_NUM_ELEMS_100000_NUM_ELEMS_100)
    {
        executeSubtest(NUM_ELEMS_100000, NUM_ELEMS_100);
    }

    /**
     * DESCRIPTION:
     *
     */
    TEST_F(TestTriangulationPath_Integration, Test_NUM_ELEMS_1000000_NUM_ELEMS_10)
    {
        executeSubtest(NUM_ELEMS_1000000, NUM_ELEMS_10);
    }
}
