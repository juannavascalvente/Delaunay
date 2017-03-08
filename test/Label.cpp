/*
 * Label.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: juan
 */

#include "Label.h"
#include <regex.h>
#include <iostream>

//#define LABEL_EXTRACT_TYPE
//#define LABEL_EXTRACT_LABEL

Label::Label(string name, string value, LabelType type)
{
	this->name = name;
	this->type = type;
	switch (type)
	{
		case BEGIN_LABEL:
		{
			this->beginLabel = "<" + name + ">";
			break;
		}
		case END_LABEL:
		{
			this->endLabel = "</" + name + ">";
			break;
		}
		case DATA_LABEL:
		{
			this->beginLabel = "<" + name + ">";
			this->value = value;
			this->endLabel = "</" + name + ">";
			break;
		}
		default:
		{
			break;
		}
	}
	// PENDING ADD VALIDATION LABEL TO CHECK BEGIN == END
	isValid = true;
}

bool Label::validFormat(string &line)
{
	LabelType type; 		// Return value.

	type = Label::extractType(line);

	return (type != UNKNOWN_LABEL);
}

LabelType Label::extractType(string &line)
{
	LabelType type; 		// Return value.
	regex_t beginRegex;
	regex_t dataRegex;
	regex_t endRegex;

	regcomp(&beginRegex, "<.*>$", 0);
	regcomp(&endRegex, "</.*>$", 0);
	regcomp(&dataRegex, "<.*>.*</.*.>$", 0);

	// Execute regular expression.
	if (!regexec(&dataRegex, line.c_str(), 0, NULL, 0))
	{
#ifdef LABEL_EXTRACT_TYPE
		cout << "Label type is DATA_LABEL" << endl;
#endif
		type = DATA_LABEL;
	}
	else if (!regexec(&endRegex, line.c_str(), 0, NULL, 0))
	{
#ifdef LABEL_EXTRACT_TYPE
		cout << "Label type is END_LABEL" << endl;
#endif
		type = END_LABEL;
	}
	else if (!regexec(&beginRegex, line.c_str(), 0, NULL, 0))
	{
#ifdef LABEL_EXTRACT_TYPE
		cout << "Label type is BEGIN_LABEL" << endl;
#endif
		type = BEGIN_LABEL;
	}
	else
	{
#ifdef LABEL_EXTRACT_TYPE
		cout << "Label type is UNKNOWN_LABEL" << endl;
#endif
		type = UNKNOWN_LABEL;
	}

	// Deallocate regular expressions.
	regfree(&beginRegex);
	regfree(&endRegex);
	regfree(&dataRegex);

	return(type);
}

void Label::extractLabel(string &line)
{
	// Reset label data.
	this->reset();

	this->isValid = true;

	// Get label type.
	this->type = this->extractType(line);
	switch (this->type)
	{
		// Extract BEGIN label only.
		case BEGIN_LABEL:
		{
#ifdef LABEL_EXTRACT_LABEL
			cout << "Extracting BEGIN_LABEL" << endl;
#endif
			this->extractBegin(line);
			this->setNameFromBeginLabel();
			break;
		}
		// Extract END label only.
		case END_LABEL:
		{
#ifdef LABEL_EXTRACT_LABEL
			cout << "Extracting END_LABEL" << endl;
#endif
			this->extractEnd(line);
			this->setNameFromEndLabel();
			break;
		}
		// Extract BEGIN, data and END label.
		case DATA_LABEL:
		{
#ifdef LABEL_EXTRACT_LABEL
			cout << "Extracting DATA_LABEL" << endl;
#endif
			this->extractBegin(line);
			this->extractData(line);
			this->extractEnd(line);
			this->setNameFromBeginLabel();
			break;
		}
		// ERROR -> Unknwon label. PENDING RAISE EXCEPTION?
		default:
		{
			cout << "Error unknown label type " << this->type << endl;
			this->isValid = false;
			break;
		}
	}
}

void Label::print() const
{
	// Print label attributes.
	cout << "Name: " << this->name << endl;
	cout << "Begin label: " << this->beginLabel << endl;
	cout << "End label: " << this->endLabel << endl;
	cout << "Value: " << this->value << endl;
	cout << "Status: " << this->isValid << endl;
	cout << "Type: " << this->type << endl;
}


void Label::extractBegin(string &line)
{
	size_t position=0;		// Character position.

	// Copy characters until first ">" character.
	position = line.find(">");
	if (position != std::string::npos)
	{
		this->beginLabel = line.substr(0,position+1);
	}
}

void Label::extractData(string &line)
{
	int length=0;			// String length.
	size_t position=0;		// Character position.
	size_t positionEnd=0;	// Character position.
	string remainingText;

	length = line.length();
	position = line.find(">");
	if (position != std::string::npos)
	{
		remainingText = line.substr(position, length-1);
		positionEnd = remainingText.find("<");
		this->value = remainingText.substr(1, positionEnd-1);
	}
}

void Label::extractEnd(string &line)
{
	int length=0;			// String length.
	size_t position=0;		// Character position.

	// Copy characters until first ">" character.
	length = line.length();
	position = line.find("</");
	if (position != std::string::npos)
	{
		this->endLabel = line.substr(position,length);
	}
}

void Label::setNameFromBeginLabel()
{
	int length=0;			// String length.

	length = this->beginLabel.length();
	this->name = this->beginLabel.substr(1,length-2);
}

void Label::setNameFromEndLabel()
{
	int length=0;			// String length.

	length = this->endLabel.length();
	this->name = this->endLabel.substr(2,length-3);
}

void Label::reset()
{
	// Reset label data.
	this->name = "";
	this->beginLabel = "";
	this->endLabel = "";
	this->value = "";
	this->isValid = false;
	this->type = UNKNOWN_LABEL;
}
