/*
 * testCompare.h
 *
 *  Created on: Mar 5, 2017
 *      Author: juan
 */

#ifndef TEST_TESTCOMPARE_H_
#define TEST_TESTCOMPARE_H_

#define DEFAULT_FILE_LIST_COMPARE		"fileList.txt"
#define DEFAULT_N_FILES_COMPARE			50

#include "Dcel.h"
#include "test.h"

/****************************************************************************
// 						TestCompare CLASS DEFITNION
****************************************************************************/
class TestCompare : public Test
{
protected:
	//------------------------------------------------------------------------
	// Attributes.
	//------------------------------------------------------------------------
	string filesNamesFile;
	Set<string> filesList;

	//------------------------------------------------------------------------
	//  Private functions.
	//------------------------------------------------------------------------
	void initParameters();
	void printParameters();
	void applyParameter(Parameter *parameter, string value);
	void dump(string dcelFileName, Dcel &dcel);
	void deallocate();
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	TestCompare(string logFile, string outFolder, bool printData) :
					Test(logFile, outFolder, printData), \
					filesNamesFile(DEFAULT_FILE_LIST_COMPARE), \
					filesList(DEFAULT_N_FILES_COMPARE) {};
	~TestCompare() {};

	//------------------------------------------------------------------------
	//  Get/Set functions.
	//------------------------------------------------------------------------
	void setFileNamesFile(string value) {this->filesNamesFile = value;};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	bool prepare();
	void main() {};
};



#endif /* TEST_TESTCOMPARE_H_ */
