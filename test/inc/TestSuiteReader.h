#ifndef DELAUNAY_TESTSUITEREADER_H
#define DELAUNAY_TESTSUITEREADER_H

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include "TestSuite.h"

#include <string>
using namespace std;


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
class TestSuiteReader
{
    // Start end test flags
    static const std::string strStartTest;
    static const std::string strEndTest;

    static const std::string strDelimiter;
public:
    /*******************************************************************************************************************
    * Public method definitions
    *******************************************************************************************************************/
    static bool read(const string &strFileName, TestSuite &suite);
};


#endif //DELAUNAY_TESTSUITEREADER_H
