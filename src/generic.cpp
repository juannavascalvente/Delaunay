/*
 * generic.cpp
 *
 *  Created on: Mar 5, 2017
 *      Author: juan
 */

#include "generic.h"
#include <sstream>


///***************************************************************************
//* Name: trim
//* IN:		str			string to trim
//* OUT:	NONE
//* RETURN:	trimmed input string
//* GLOBAL:	NONE
//* Description: remove leading blanks from the input string
//***************************************************************************/
std::string trim(std::string const& str)
{
    std::string word;
    std::stringstream stream(str);
    stream >> word;

    return word;
}


