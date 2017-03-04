/*
 * DefinitionParameter.cpp
 *
 *  Created on: Feb 23, 2017
 *      Author: juan
 */

#include <Validator.h>


/****************************************************************************
//					IntegerValidator CLASS implementation
****************************************************************************/
int NumericValidator::convert(string text)
{
	int value=0;		// Return value.

	// Convert string into integer.
	stringstream str(text);
	str >> value;

	return(value);
};

bool NumericValidator::isValid(string text)
{
	int value=0;		// Return value.

	// Get integer value.
	value = this->convert(text);

	//cout << "Min " << this->min << " and Max is " << this->max << " and value is " << this->value << endl;
	return ((value >= this->min) && (value <= this->max));
};

void NumericValidator::print()
{
	cout << "Integer definition" << endl;
	cout << "Min value " << this->min << endl;
	cout << "Max value " << this->max << endl;
}

/****************************************************************************
//					FileValidator CLASS implementation
****************************************************************************/
bool FileValidator::isValid(string text)
{
	bool success=false;	// Return value.
	ifstream ifs;		// File stream.

	// Open file.
	ifs.open(text.c_str(), ios::in);

	// Check file is opened.
	if (ifs.is_open())
	{
		success = true;

		// Close file.
		ifs.close();
	}

	return(success);
};

