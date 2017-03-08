/*
 * Parameter.h
 *
 *  Created on: Feb 23, 2017
 *      Author: juan
 */

#ifndef TEST_PARAMETER_H_
#define TEST_PARAMETER_H_

#include "Label.h"
#include "Validator.h"
#include <regex.h>


class Parameter
{
protected:
	//------------------------------------------------------------------------
	// Attributes.
	//------------------------------------------------------------------------
	string name;

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	Parameter() {};
	Parameter(string name) : name(name) {};
	virtual ~Parameter(){};

	//------------------------------------------------------------------------
	// Get/Set functions.
	//------------------------------------------------------------------------
	const string& getName() const {return name;}
	virtual bool isValid(string value) {cout<< "DEFAULT"<<endl;return(false);};
	virtual void deleteValidator() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	void print();
};

class ParameterInt : public Parameter
{
	//------------------------------------------------------------------------
	// Attributes.
	//------------------------------------------------------------------------
	NumericValidator *validator;

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	ParameterInt() : Parameter(NULL_LABEL), validator(NULL) {};
	ParameterInt(string name, NumericValidator *val) : 	Parameter(name), \
														validator(val) {};
	~ParameterInt(){};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	bool isValid(string value) {return(validator->isValid(value));};
	void print();
	void deleteValidator() {delete validator;};
};

class ParameterFile : public Parameter
{
	//------------------------------------------------------------------------
	// Attributes.
	//------------------------------------------------------------------------
	FileValidator *validator;

public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	ParameterFile() : Parameter(NULL_LABEL), validator(NULL) {};
	ParameterFile(string name, FileValidator *val) : Parameter(name), \
													 validator(val) {};
	~ParameterFile(){};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	bool isValid(string value) {return(validator->isValid(value));};
	void print();
	void deleteValidator() {delete validator;};
};


#endif /* TEST_PARAMETER_H_ */
