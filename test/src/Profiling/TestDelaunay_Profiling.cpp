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
         * @fn      execute
         * @brief   Generates a set of random points and computes the Delaunay triangulations using that set of points.
         *          The number of points is defined by szNumPoints and it is executed szNumIterations times
         *
         * @param   szNumPoints         (IN) Points set number of points
         * @param   szNumIterations     (IN) Number of times the triangulation is computed
         */
        static void execute(size_t szNumPoints, size_t szNumIterations, string strFileName);

        /**
         * @fn      writeReport
         * @brief   Writes a report to a file. The report contains one execution time per line and the last two lines
         *          are total and average tim
         *
         * @param   strFileName     (IN) Output file name
         * @param   vTimes          (IN) Vector that contains execution times
         */
        static void writeReport(const string& strFileName, const vector<std::chrono::duration<double>>& vTimes);
    };


    void TestDelaunay_Profiling::execute(size_t szNumPoints, size_t szNumIterations, string strFileName)
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
* @brief   Computes Delaunay triangulation 100 times using 1K points and writes a report
 */
TEST_F(TestDelaunay_Profiling, Test_Delaunay_1K_100)
{
    string strFileName="Delaunay_1K_100.txt";
    vector<std::chrono::duration<double>> vTimes;
    cout << strFileName << "..." << endl;
    execute(NUM_POINTS_1K, NUM_ITERATIONS_10, strFileName);
    cout << strFileName << "...Done" << endl;
}

/**
 * @brief   Computes Delaunay triangulation 100 times using 10K points and writes a report
 */
TEST_F(TestDelaunay_Profiling, Test_Delaunay_10K_100)
{
    string strFileName="Delaunay_10K_100.txt";
    vector<std::chrono::duration<double>> vTimes;
    cout << strFileName << "..." << endl;
    execute(NUM_POINTS_10K, NUM_ITERATIONS_10, strFileName);
    cout << strFileName << "...Done" << endl;
}

/**
* @brief   Computes Delaunay triangulation 100 times using 100K points and writes a report
 */
TEST_F(TestDelaunay_Profiling, Test_Delaunay_100K_100)
{
    string strFileName="Delaunay_100K_100.txt";
    vector<std::chrono::duration<double>> vTimes;
    cout << strFileName << "..." << endl;
    execute(NUM_POINTS_100K, NUM_ITERATIONS_10, strFileName);
    cout << strFileName << "...Done" << endl;
}

/**
* @brief   Computes Delaunay triangulation 100 times using 1M points and writes a report
 */
TEST_F(TestDelaunay_Profiling, Test_Delaunay_1M_100)
{
    string strFileName="Delaunay_1M_100.txt";
    vector<std::chrono::duration<double>> vTimes;
    cout << strFileName << "..." << endl;
    execute(NUM_POINTS_1M, NUM_ITERATIONS_10, strFileName);
    cout << strFileName << "...Done" << endl;
}

