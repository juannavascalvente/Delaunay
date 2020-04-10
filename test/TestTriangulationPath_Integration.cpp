/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <gtest/gtest.h>

#include "PointFactory.h"
#include "PointsWriter.h"
#include "StarTriangulation.h"

using namespace std;


/***********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#define NUM_SAMPLES_5                   (5)
#define NUM_SAMPLES_100                 (100)
#define NUM_SAMPLES_1000                (1000)
#define NUM_SAMPLES_1000000             (1000000)

#define NUM_ITERATIONS_10               (10)
#define NUM_ITERATIONS_100              (100)
#define NUM_ITERATIONS_1000             (1000)
#define NUM_ITERATIONS_1000000          (1000000)


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

    protected:

        // Constructor (called before each test case) - SetUp
        TestTriangulationPath_Integration()
        {
            // Check if input point parameter provided by user.
            point.random();

            string strPointFileName = "Point.txt";
            vector<Point<TYPE>> vPointToLocate;
            vPointToLocate.push_back(point);
            PointsWriter::write(strPointFileName, vPointToLocate);
        };
        ~TestTriangulationPath_Integration() override = default;

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
            string strPointSetFileName = "PointSet.txt";
            PointsWriter::write(strPointSetFileName, vPoints);

            // Create triangulation
            auto *triangulation = new StarTriangulation(vPoints);
            triangulation->build();

            // Execute function to test
            isRunSuccess = triangulation->findFace(point, iFaceId);

            // Check result
            EXPECT_TRUE(isRunSuccess);
        }
    }

    /**
     * DESCRIPTION:
     *
     */
    TEST_F(TestTriangulationPath_Integration, Test_Num_Samples_5_Iterations_1000000)
    {
        executeSubtest(NUM_SAMPLES_5, NUM_ITERATIONS_1000000);
    }


    /**
     * DESCRIPTION:
     *
     */
    TEST_F(TestTriangulationPath_Integration, Test_Num_Samples_100_Iterations_1000)
    {
        executeSubtest(NUM_SAMPLES_100, NUM_ITERATIONS_1000);
    }

    /**
     * DESCRIPTION:
     *
     */
    TEST_F(TestTriangulationPath_Integration, Test_Num_Samples_1000_Iterations_100)
    {
        executeSubtest(NUM_SAMPLES_1000, NUM_ITERATIONS_100);
    }

    /**
     * DESCRIPTION:
     *
     */
    TEST_F(TestTriangulationPath_Integration, Test_Num_Samples_1000000_Iterations_10)
    {
        executeSubtest(NUM_SAMPLES_1000000, NUM_ITERATIONS_10);
    }
}
