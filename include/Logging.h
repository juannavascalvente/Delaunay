/*
 * Logging.h
 *
 *  Created on: Jul 7, 2016
 *      Author: jnavas
 */

#ifndef INCLUDE_LOGGING_H_
#define INCLUDE_LOGGING_H_

#include <iostream>
#include <fstream>
using namespace std;

#include "defines.h"
#include "Point.h"

//****************************************************************************
// 							Data types definition
//****************************************************************************
// Type of information to print.
enum MessageCategory {Info=1, Warning=2, Error=3};

/****************************************************************************
// 						Logging CLASS DEFITNION
****************************************************************************/
class Logging
{
	//------------------------------------------------------------------------
	//  Attributes
	//------------------------------------------------------------------------
	//static Logging *instance;

public:
	/*------------------------------------------------------------------------
	  Constructor/Destructor.
	------------------------------------------------------------------------*/
	Logging() {};
	Logging( const string fileName, bool force);
	~Logging();

	/*------------------------------------------------------------------------
	  Public functions.
	------------------------------------------------------------------------*/
	//static void buildText( string func, string f, int n, ...);
	static void buildText( string func, string f, string text);
	static void buildText( string func, string f, int value);
	static void buildText( string func, string f, TYPE value);
#ifdef FLOAT_TYPE
	static void buildText( string func, string f, double value);
#endif
	static void buildText( string func, string f, Point<TYPE> *point);
	static void buildRange( string func, string f, int start, int end);
	static void write( bool print, enum MessageCategory category);

	//static Logging* getInstance();
};

#endif /* INCLUDE_LOGGING_H_ */
