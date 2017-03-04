/*
 * Parameter.cpp
 *
 *  Created on: Feb 23, 2017
 *      Author: juan
 */
#include "Parameter.h"


void Parameter::print()
{
	cout << "----------------------------------" << endl;
	cout << "Parameter values" << endl;
	cout << "----------------------------------" << endl;
	cout << "Name: " << this->name << endl;
}

void ParameterInt::print()
{
	Parameter::print();
	this->validator->print();
}
