/*
 * testExecution.cpp
 *
 *  Created on: Mar 5, 2017
 *      Author: juan
 */

#include "testExecution.h"

/***************************************************************************
* Name: 	initParameters
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	registers the test parameters
***************************************************************************/
void TestExecution::initParameters()
{
	// Add common test parameters.
	Test::initParameters();

	// Add # points parameter.
	NumericValidator *nPointsDefinition = new NumericValidator(3, INT_MAX);
	ParameterInt *nPointsParameter = new ParameterInt(TYPE_N_POINTS_LABEL, nPointsDefinition);
	this->parameters.add(nPointsParameter);

	// Add # tests parameter.
	NumericValidator *nTestsDefinition = new NumericValidator(1, INT_MAX);
	ParameterInt *nTestsParameter = new ParameterInt(TYPE_N_TESTS_LABEL, nTestsDefinition);
	this->parameters.add(nTestsParameter);

	// Add delta # points parameter.
	NumericValidator *deltaPointsDefinition = new NumericValidator(1, 100);
	ParameterInt *deltaPointsParameter = new ParameterInt(TYPE_DELTA_STEP_LABEL, deltaPointsDefinition);
	this->parameters.add(deltaPointsParameter);

	// Add # steps parameter.
	NumericValidator *stepsDefinition = new NumericValidator(0, INT_MAX);
	ParameterInt *stepsParameter = new ParameterInt(TYPE_N_STEPS_LABEL, stepsDefinition);
	this->parameters.add(stepsParameter);
#ifdef DEBUG_TEST_DELAUNAY_BUILD_INIT
	// Print parameters data.
	this->printParameters();
#endif
}

/***************************************************************************
* Name: 	applyParameter
* IN:		parameter		parameter to update
* 			value			value to set
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	set a class attribute to "value"
***************************************************************************/
void TestExecution::applyParameter(Parameter *parameter, string value)
{
	string parameterName;
	int convertedValue=0;

	// Get parameter label.
	parameterName = parameter->getName();
	if (parameterName == TYPE_N_POINTS_LABEL)
	{
		// Convert string into integer.
		stringstream str(value);
		str >> convertedValue;
		this->setPoints(convertedValue);
	}
	else if (parameterName == TYPE_N_TESTS_LABEL)
	{
		// Convert string into integer.
		stringstream str(value);
		str >> convertedValue;

		this->setTests(convertedValue);
	}
	else if (parameterName == TYPE_DELTA_STEP_LABEL)
	{
		// Convert string into integer.
		stringstream str(value);
		str >> convertedValue;

		this->setDeltaPoints(convertedValue);
	}
	else if (parameterName == TYPE_N_STEPS_LABEL)
	{
		// Convert string into integer.
		stringstream str(value);
		str >> convertedValue;

		this->setSteps(convertedValue);
	}
	else if (parameterName == OUT_FOLDER_LABEL)
	{
		this->setOutFolder(value);
	}
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Unknown label parameter: ");
		Logging::buildText(__FUNCTION__, __FILE__, parameterName);
		Logging::write(true, Error);
	}
}

/***************************************************************************
* Name: 	printParameters
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	prints the test parameters.
***************************************************************************/
void TestExecution::printParameters()
{
#ifdef DEBUG_TEST_EXECUTION
	Logging::buildText("Number of points ");
	Logging::buildText(this->nPoints);
	Logging::write(true, Info);
	Logging::buildText("Delta points ");
	Logging::buildText(this->deltaPoints);
	Logging::write(true, Info);
	Logging::buildText("Number of steps ");
	Logging::buildText(this->nSteps);
	Logging::write(true, Info);
	Logging::buildText("Number of test ");
	Logging::buildText(this->nTests);
	Logging::write(true, Info);
	Logging::buildText("Output folder ");
	Logging::buildText(this->outFolder);
	Logging::write(true, Info);
#endif
}

/***************************************************************************
* Name: 	prepare
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	Executes any previous action before the test is executed.
* 				This operation are compute previous values, do previous
* 				checks, etc...
***************************************************************************/
bool TestExecution::prepare()
{
	bool success=true;			// Return value.

	// Initialize # tests and current test.
	this->testCounter = 1;
	this->totalTests = this->nSteps*this->nTests;

	return(success);
}


