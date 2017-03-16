/*
 * test.cpp
 *
 *  Created on: Feb 19, 2017
 *      Author: juan
 */
#include "test.h"
#include <stdlib.h>

//#define DEBUG_TEST_READ
//#define DEBUG_TEST_GET_PARAM_INDEX
//#define DEBUG_TEST_PARSE_PARAMETERS

enum READ_TEST_PROCESS	{READ_HEADER_STATUS,
						 READ_TYPE_STATUS,
						 READ_PARAMETER_STATUS,
						 UNKOWN_STATUS};

Test::~Test()
{
	int i=0;			// Loop counter.
	Parameter* param;	// Pointer to generic parameter.

	// Deallocate parameters set.
	for (i=0; i<this->parameters.getNElements() ;i++)
	{
		param = *this->parameters.at(i);
		param->deleteValidator();
		delete param;
	}
}

bool Test::getParameterIndex(Label &label, int &index)
{
	bool found=false;				// Return value.
	Parameter *currentParameter;
	string parameterName;

#ifdef DEBUG_TEST_GET_PARAM_INDEX
	cout << "Get index for label " << label.getName() << endl;
#endif

	// Search until label found or end of list
	index = 0;
	found = false;
#ifdef DEBUG_TEST_GET_PARAM_INDEX
	cout << "Tester: getParameterIndex array length " << this->parameters.getNElements() << endl;
#endif
	while (!found && (index < this->parameters.getNElements()))
	{
		currentParameter = *this->parameters.at(index);
		parameterName = currentParameter->getName();
#ifdef DEBUG_TEST_GET_PARAM_INDEX
		cout << "Tester: getParameterIndex. Compare labels " << label.getName() << " and " << parameterName << endl;
#endif
		if (parameterName.compare(label.getName()) == 0)
		{
#ifdef DEBUG_TEST_GET_PARAM_INDEX
			cout << "Tester: getParameterIndex. Label found at index " << index << endl;
#endif
			found = true;
		}
		else
		{
			index++;
#ifdef DEBUG_TEST_GET_PARAM_INDEX
			cout << "Tester: getParameterIndex. Checking next index " << index << endl;
#endif
		}
	}

	return (found);
}

void Test::parseParameters(Set<Label> &labels)
{
	int	i=0;			// Loop counter.
	int	index=0;		// Position in array.
	Label *currentLabel;
	Parameter *currentParameter;
#ifdef DEBUG_TEST_PARSE_PARAMETERS
	cout << "Number of labels is " << labels.getNElements() << endl;
#endif

	// Apply all labels
	for (i=0; i<labels.getNElements() ;i++)
	{
		// Get current label.
		currentLabel = labels.at(i);
#ifdef DEBUG_TEST_PARSE_PARAMETERS
		cout << "Label number " << (i+1) << endl;
		currentLabel->print();
#endif

		// Check if current label is in the labels set.
		if (this->getParameterIndex(*currentLabel, index))
		{
#ifdef DEBUG_TEST_PARSE_PARAMETERS
			cout << "Parameter index " << (index+1) << endl;
#endif
			// Check if parameter is valid.
			currentParameter = *this->parameters.at(index);
			if (currentParameter->isValid(currentLabel->getValue()))
			{
				// Apply value.
				this->applyParameter(currentParameter, currentLabel->getValue());
#ifdef DEBUG_TEST_PARSE_PARAMETERS
				cout << "is valid parameter at position " << i << endl;
#endif
			}
			else
			{
#ifdef DEBUG_TEST_PARSE_PARAMETERS
				cout << "Not valid value " << currentLabel->getValue() << \
						" in label " << currentLabel->getName() << endl;
#endif
			}
		}
		else
		{
			cout << "Label " << currentLabel->getName() << \
					" not found in parameters list" << endl;
		}
	}
}

/***************************************************************************
* Name: 	removeExistingFiles
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: removes all files in output folder.
***************************************************************************/
void Test::removeExistingFiles()
{
	string command = "exec rm -r " + this->outFolder + "*.txt";
	system(command.c_str());
}


/***************************************************************************
* Name: 	getTypeTest
* IN:		testName
* OUT:		NONE
* RETURN:	test type 		if the test is valid.
* GLOBAL:	NONE
* Description: Checks if the input string matches any test name. If so then
* 				it returns the enum value for such test. Otherwise it
* 				returns the UNKNOWN test value.
***************************************************************************/
TestType Test::getTypeTest(string testName)
{
	TestType type=TestType(0);		// Return value.

	// Test set.
	if (testName.compare(TEST_SET_NAME) == 0)
	{
		type = TEST_SET;
	}
	// Test dcel.
	else if (testName.compare(TEST_DCEL_NAME) == 0)
	{
		type = TEST_DCEL;
	}
	// Test Delaunay triangulation.
	else if (testName.compare(TEST_DELAUNAY_NAME) == 0)
	{
		type = TEST_DELAUNAY;
	}
	// Compare already built Delaunay.
	else if (testName.compare(TEST_DELAUNAY_COMPARE_NAME) == 0)
	{
		type = TEST_DELAUNAY_COMPARE;
	}
	// Test build Voronoi areas.
	else if (testName.compare(TEST_VORONOI_NAME) == 0)
	{
		type = TEST_VORONOI_BUILD;
	}
	// Compare built and already computed Voronoi areas.
	else if (testName.compare(TEST_VORONOI_COMPARE_NAME) == 0)
	{
		type = TEST_VORONOI_COMPARE;
	}
	// Compare computed paths and new executions.
	else if (testName.compare(TEST_PATH_DELAUNAY_NAME) == 0)
	{
		type = TEST_PATH_DELAUNAY;
	}
	// Compare computed paths and new executions.
	else if (testName.compare(TEST_PATH_DELAUNAY_COMPARE_NAME) == 0)
	{
		type = TEST_PATH_DELAUNAY_COMPARE;
	}
	// Compare computed paths and new executions.
	else if (testName.compare(TEST_PATH_VORONOI_NAME) == 0)
	{
		type = TEST_PATH_VORONOI;
	}
	// Compare computed paths and new executions.
	else if (testName.compare(TEST_PATH_VORONOI_COMPARE_NAME) == 0)
	{
		type = TEST_PATH_VORONOI_COMPARE;
	}
	else
	{
		cout << "Error: Unknown test name: " << testName << endl;
	}

	return(type);
}

bool Test::read(ifstream &ifs, Set<Label> &labels, TestType &testType)
{
	bool 	eof=false;				// End of file flag.
	bool 	endOfTest=false;		// End of test flag.
	string	line;					// File line text.
	Label 	label;					// Current label.
	int		status=READ_HEADER_STATUS;	// Read process status.

	// Read until end of file or end of test.
	while (!eof && !endOfTest)
	{
		// Read line.
		eof = !getline(ifs, line);
		if (!eof)
		{
#ifdef DEBUG_TEST_READ
			cout << "Line read is: " << line << endl;
#endif
			// Check if label has valid format.
			if (Label::validFormat(line))
			{
				// Extract label data.
				label.extractLabel(line);
#ifdef DEBUG_TEST_READ
				cout << "Current status is " << status << endl;
				cout << "Label has a valid format. Label data:" << endl;
				label.print();
#endif
				// Check current read process tatus.
				switch(status)
				{
					// Expecting TEST label.
					case READ_HEADER_STATUS:
					{
						if (label.isBeginLabel())
						{
							if (label.getName().compare(TEST_LABEL) == 0)
							{
#ifdef DEBUG_TEST_READ
								cout << "Change from HEADER to TYPE" << endl;
#endif
								status = READ_TYPE_STATUS;
							}
						}
						else
						{
							cout << "Expected TEST label and found: ";
							if (label.isBeginLabel())
							{
								cout << label.getBeginLabel() << endl;
							}
							else
							{
								cout << label.getEndLabel() << endl;
							}
						}
						break;
					}
					// Expecting TYPE label.
					case READ_TYPE_STATUS:
					{
						if (label.isDataLabel())
						{
							if (label.getName().compare(TEST_TYPE_LABEL) == 0)
							{
#ifdef DEBUG_TEST_READ
								cout << "Change from TYPE to PARAMETERS" << endl;
#endif
								status = READ_PARAMETER_STATUS;
								testType = Test::getTypeTest(label.getValue());
							}
						}
						// Error. Test type not valid.
						else
						{
#ifdef DEBUG_TEST_READ
							cout << "Invalid test type " << label.getName() << endl;
#endif
							testType = UNKNOWN_TEST;
							endOfTest = true;
						}
						break;
					}
					// Expecting parameter or end of test label.
					case READ_PARAMETER_STATUS:
					{
						// Check if it is a parameter.
						if (label.isDataLabel())
						{
#ifdef DEBUG_TEST_READ
							cout << "Still in PARAMETERS" << endl;
#endif
							labels.add(label);
						}
						// Check if it is end test label.
						else if (label.isEndLabel())
						{
							if (label.getName().compare(TEST_LABEL) == 0)
							{
#ifdef DEBUG_TEST_READ
								cout << "END test label" << endl;
#endif
								endOfTest = true;
							}
						}
						// Error unknown label. Skip this line.
						else
						{
							cout << "Error: unexpected label " << label.getName() << endl;
							cout << "Expecting parameter or end of test" << endl;
						}
						break;
					}
					default:
					{
						// PENDING THROW EXCEPTION?
						cout << "Error: unknown state " << status << " reading test" << endl;
						break;
					}
				}
			}
		}
	}

	return(eof);
}

void Test::init(Set<Label> &labels)
{
	this->initParameters();
	this->parseParameters(labels);
	this->removeExistingFiles();
}

void Test::run()
{
	this->prepare();
	this->main();
	this->write();
}

void Test::write()
{
	Logging::buildText(__FUNCTION__, __FILE__, \
			"----------------------------------------------\n");
	Logging::buildText(__FUNCTION__, __FILE__, "Test summary\n");
	Logging::buildText(__FUNCTION__, __FILE__, this->totalTests-this->nTestFailed);
	Logging::buildText(__FUNCTION__, __FILE__, "/");
	Logging::buildText(__FUNCTION__, __FILE__, this->totalTests);
	Logging::buildText(__FUNCTION__, __FILE__,  \
			"\n----------------------------------------------");
	if (this->nTestFailed == 0)
	{
		Logging::write(true, Successful);
	}
	else
	{
		Logging::write(true, Error);
	}
}

void Test::finish()
{

}

