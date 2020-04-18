/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "TestSuiteReader.h"
#include "Logging.h"

#include <fstream>
#include <cstring>


const std::string TestSuiteReader::strStartTest = "<TEST>";
const std::string TestSuiteReader::strEndTest = "</TEST>";
const std::string TestSuiteReader::strDelimiter = "=";


/***********************************************************************************************************************
* Public method definitions
***********************************************************************************************************************/
bool TestSuiteReader::read(const string &strFileName, TestSuite &suite)
{
    bool isSuccess=false;		// Return value.

    try
    {
        // Open file.
        ifstream ifs(strFileName.c_str(), ios::in);
        if (ifs.is_open())
        {
            std::string line;
            TestData *test=nullptr;
            while (std::getline(ifs, line))
            {
                // New test
                if (line.substr(0, strStartTest.size()) == strStartTest)
                {
                    test = new TestData();
                }
                // End test
                else if (line.substr(0, strEndTest.size()) == strEndTest)
                {
                    suite.add(*test);
                    delete test;
                }
                // Test parameter
                else
                {
                    // Extract parameter
                    auto start = 0;
                    auto end = line.find(strDelimiter);
                    string strKey = line.substr(start, end - start);
                    string strValue = line.substr(end+1, line.length());

                    // Add parameter to test
                    test->add(strKey, strValue);
                }
            }

            // Close file.
            ifs.close();
            isSuccess = true;
        }
        else
        {
            // show message:
            std::cout << "Error opening file " << strFileName << std::endl;
            cerr << "Error: " << strerror(errno);
        }
    }
    catch (const ifstream::failure& e)
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error opening file: ");
        Logging::buildText(__FUNCTION__, __FILE__, strFileName);
        Logging::write(true, Error);
    }
    catch (bad_alloc &ex)
    {
        Logging::buildText(__FUNCTION__, __FILE__, "Error allocating memory");
        Logging::write( true, Error);
    }
    catch (exception &ex)
    {
        std::cout << ex.what();
    }

    return isSuccess;
}