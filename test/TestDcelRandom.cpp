/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <iostream>
#include <gtest/gtest.h>

#include "Dcel.h"
#include "DcelReader.h"
#include "Delaunay.h"

using namespace std;


/***********************************************************************************************************************
* Defines
***********************************************************************************************************************/
#define NUM_TESTS                   3


/***********************************************************************************************************************
* Test Class Definition
***********************************************************************************************************************/
namespace
{
    class TestDcelRandom : public ::testing::Test
    {
        std::vector<string> vSubtestInputs;
        std::vector<string> vSubtestExpected;

        string strTestInfoFileName = "testInfo.xml";
        string strFolders[NUM_TESTS];
        size_t szTestCnt;

        bool readTestData(size_t szIndex);

    protected:

        // You can remove any or all of the following functions if its body
        // is empty.
        // Constructor (called before each test case) - SetUp
        TestDcelRandom()
        {
            // Initialize data.
            strFolders[0] = "/home/delaunay/Projects/Git/Delaunay/tmp/output/Random/NumSamples_10/";
            strFolders[1] = "/home/delaunay/Projects/Git/Delaunay/tmp/output/Random/NumSamples_100/";
            strFolders[2] = "/home/delaunay/Projects/Git/Delaunay/tmp/output/Random/NumSamples_10000/";
        }

        // Deconstructor (called after each test case) - TearDown
        ~TestDcelRandom() override
        {
        }

        void SetUp() override
        {
            // Reset sub test data.
            vSubtestInputs.clear();
            vSubtestExpected.clear();

            // Read test data.
            ASSERT_TRUE(readTestData(szTestCnt));
            szTestCnt++;
        }

        void TearDown() override
        {
        }

    public:
        void executeSubtest();
    };


    bool TestDcelRandom::readTestData(size_t szIndex)
    {
        bool isSuccess=true;        // Return value

        // Build test info file path.
        string strPath = strFolders[szIndex] + strTestInfoFileName;

        // Open file
        ifstream ifs;
        ifs.open(strPath);

        // Check file is open
        if (ifs.is_open())
        {
            string strInput;
            string strExpected;

            while (ifs >> strInput >> strExpected)
            {
                // Add sub test.
                vSubtestInputs.push_back(strFolders[szIndex] + strInput);
                vSubtestExpected.push_back(strFolders[szIndex] + strExpected);
            }

            // Close file
            ifs.close();
        }
        else
        {
            cerr << "Error opening test info file name " << strPath << endl;
            isSuccess = false;
        }

        return isSuccess;
    }

    void TestDcelRandom::executeSubtest()
    {
        Dcel dcelComputed;      // DCEL computed by test
        Dcel dcelExpected;      // DCEL expected
        Delaunay delaunay;		// Delaunay data.

        auto iterInputs = vSubtestInputs.begin();
        auto iterExpected = vSubtestExpected.begin();

        // Sub tests iterator.
        for (size_t i=0; i<vSubtestInputs.size() ; i++)
        {
            dcelComputed.reset();
            dcelExpected.reset();

            // Read input flat points file.
            bool isSuccess;
            isSuccess = DcelReader::readPoints((*iterInputs), true, dcelComputed);
            ASSERT_TRUE(isSuccess);

            // Read expected DCEL file.
            isSuccess = DcelReader::read((*iterExpected), false, dcelExpected);
            ASSERT_TRUE(isSuccess);

            // Compute Delaunay triangulation.
            delaunay.reset();
            delaunay.setDCEL(&dcelComputed);
            isSuccess = delaunay.incremental();
            ASSERT_TRUE(isSuccess);

            // Compare dcel.
            isSuccess = (dcelComputed == dcelExpected);
            if (!isSuccess)
            {
                cout << "Failed Delaunay comparison in file " << (*iterInputs) << endl;
                ASSERT_TRUE(isSuccess);
            }

            // Next elements
            iterInputs++;
            iterExpected++;
        }
    }
}


/**
 * DESCRIPTION:
 *
 */
TEST_F(TestDcelRandom, Test_Num_Samples_10)
{
    executeSubtest();
}


/**
 * DESCRIPTION:
 *
 */
TEST_F(TestDcelRandom, Test_Num_Samples_100)
{
    executeSubtest();
}


/**
 * DESCRIPTION:
 *
 */
TEST_F(TestDcelRandom, Test_Num_Samples_10000)
{
    executeSubtest();
}
