/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "Delaunay.h"
#include "DelaunayIO.h"
#include "PointFactory.h"
#include "PointsReader.h"
#include "TriangulationFactory.h"
#include "TestSuiteReader.h"

#include <chrono>
#include <gtest/gtest.h>
#include <iostream>


/***********************************************************************************************************************
* Test Class Definition
***********************************************************************************************************************/
namespace
{
    class TestDelaunay_Profiling : public ::testing::Test
    {
    public:

        /**
         * @fn      executeTwice
         * @brief   Generates a set of random points and computes the Delaunay triangulations using that set of points.
         *          The number of points is defined by szNumPoints and it is executed szNumIterations times
         *
         * @param   szNumPoints         (IN) Points set number of points
         * @param   szNumIterations     (IN) Number of times the triangulation is computed
         */
        static void executeTwice(size_t szNumPoints, size_t szNumIterations, string strFileName);

        static void writeReport(const string& strFileName, const vector<std::chrono::duration<double>>& vTimes);
    };


    void TestDelaunay_Profiling::executeTwice(size_t szNumPoints, size_t szNumIterations, string strFileName)
    {
        vector<std::chrono::duration<double>> vTimes;

        // Execute test szNumIterations times
        for (size_t i=0; i<szNumIterations ; i++)
        {
            // Two points set
            vector<Point<TYPE>> vPoints;

            // Generate random points set
            PointFactory::generateRandom(szNumPoints, vPoints);

            // Get init time
            auto start = std::chrono::steady_clock::now();

            // Build first Delaunay triangulation
            bool isSuccess;
            Delaunay *delaunay = TriangulationFactory::createDelaunay(vPoints, isSuccess);
            ASSERT_TRUE(isSuccess);

            // Get end time and time elapsed
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            vTimes.push_back(elapsed);

            // Free resources
            delete delaunay;
        }

        // Write report
        writeReport(strFileName, vTimes);
    }

    void TestDelaunay_Profiling::writeReport(const string& strFileName, const vector<std::chrono::duration<double>>& vTimes)
    {
        // Open file.
        ofstream ofs(strFileName.c_str(), ios::out);
        if (ofs.is_open())
        {
            double total=0.0;
            for (auto time : vTimes)
            {
                ofs << time.count() << endl;
                total += time.count();
            }
            ofs << "Total: " << total << endl;
            ofs << "Avg: " << (total / vTimes.size()) << endl;

            // Close file.
            ofs.close();
        }
    }
}


/**
 * @brief   Computes Delaunay triangulation using different subset of points (range from 1K to 1M. Each one is executed
 *          100 times and the average is computed
 */
TEST_F(TestDelaunay_Profiling, Test_Delaunay)
{
    vector<std::chrono::duration<double>> vTimes;
    executeTwice(NUM_POINTS_1K, NUM_ITERATIONS_10, "Delaunay_1K_100.txt");
    cout << "Delaunay_1K_100.txt...Done" << endl;
    executeTwice(NUM_POINTS_10K, NUM_ITERATIONS_100, "Delaunay_10K_100.txt");
    cout << "Delaunay_10K_100.txt...Done" << endl;
    executeTwice(NUM_POINTS_100K, NUM_ITERATIONS_100, "Delaunay_100K_100.txt");
    cout << "Delaunay_100K_100.txt...Done" << endl;
    executeTwice(NUM_POINTS_1M, NUM_ITERATIONS_100, "Delaunay_1M_100.txt");
    cout << "Delaunay_1M_100.txt...Done" << endl;
}
