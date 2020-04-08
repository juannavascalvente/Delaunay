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
enum MessageCategory {Info=1, Warning=2, Error=3, Successful=4, Testing=5};

/****************************************************************************
// 						Logging CLASS DEFITNION
****************************************************************************/
class Logging
{
public:
	/*------------------------------------------------------------------------
	  Constructor/Destructor.
	------------------------------------------------------------------------*/
	Logging() = default;;
	Logging(const string& fileName, bool force);
	~Logging();

	/*------------------------------------------------------------------------
	  Public functions.
	------------------------------------------------------------------------*/
	//static void buildText(string func, string f, int n, ...);
	static void buildText(string func, string f, const string& text);
	static void buildText(string func, string f, int value);
    static void buildText(string func, string f, size_t szValue);
	static void buildText(string func, string f, TYPE value);
#ifdef FLOAT_TYPE
	static void buildText(string func, string f, double value);
#endif
	static void buildText(string func, string f, Point<TYPE> *point);
	static void buildRange(string func, string f, int start, int end);
	static void write(bool print, enum MessageCategory category);

	static void buildText(const string& text);
	static void buildText(int value);
};

#endif /* INCLUDE_LOGGING_H_ */
