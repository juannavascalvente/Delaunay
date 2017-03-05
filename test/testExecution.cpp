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
	else
	{
		Logging::buildText(__FUNCTION__, __FILE__, "Unknown label parameter:");
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
	cout << "---------------------------------------------------" << endl;
	cout << "Test parameters" << endl;
	cout << "---------------------------------------------------" << endl;
	cout << "Number of points " << this->nPoints << endl;
	cout << "Delta points " << this->deltaPoints << endl;
	cout << "Number of steps " << this->nSteps << endl;
	cout << "Number of test  " << this->nTests << endl;
	cout << "Output folder " << this->outputFolder << endl;
	cout << "---------------------------------------------------" << endl;
}

/***************************************************************************
* Name: 	deallocate
* IN:		NONE
* OUT:		NONE
* RETURN:	NONE
* GLOBAL:	NONE
* Description: 	free test resources. The data allocated are the parameters
* 				and validators allocated in initParameters
***************************************************************************/
void TestExecution::deallocate()
{
	cout << "Deallocating TestExecution" << endl;
	// PENDING https://github.com/juannavascalvente/Delaunay/issues/2
}



