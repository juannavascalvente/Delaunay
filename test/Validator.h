/*
 * DefinitionParameter.h
 *
 *  Created on: Feb 23, 2017
 *      Author: juan
 */

#ifndef TEST_VALIDATOR_H_
#define TEST_VALIDATOR_H_

#include "defines.h"
#include "Point.h"

#include <climits>
#include <string>
#include <stdio.h>
using namespace std;

const string nullString("NULL");

/****************************************************************************
//					DefinitionParameter CLASS DEFITNION
****************************************************************************/
template<class T>
class Validator
{
protected:
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	T 		value;

	virtual T convert(string text) = 0;
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	Validator() {};
	virtual ~Validator() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	virtual bool isValid(string text) = 0;
	virtual void print() {
		cout << "ERROR: Calling base method print" << endl;
	}
};

/****************************************************************************
//						FileParameter CLASS DEFITNION
****************************************************************************/
class FileValidator : public Validator<string>
{
	string convert(string text) {return(text);};
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	FileValidator() {};
	~FileValidator() {};

	bool isValid(string text);
	void print() {};
};

/****************************************************************************
//					NumericValidator CLASS DEFITNION
****************************************************************************/
class NumericValidator : public Validator<int>
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	int 	min;
	int 	max;

	int  convert(string text);
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	NumericValidator() : min(0), max(0) {};
	NumericValidator(int min, int max) : min(min), max(max) {};
	~NumericValidator() {};

	//------------------------------------------------------------------------
	//  Public functions.
	//------------------------------------------------------------------------
	bool isValid(string text);
	void print();
};



#endif /* TEST_VALIDATOR_H_ */
