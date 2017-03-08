/*
 * Label.h
 *
 *  Created on: Mar 4, 2017
 *      Author: juan
 */

#ifndef TEST_LABEL_H_
#define TEST_LABEL_H_

#include <string>
using namespace std;

/****************************************************************************
// 							Data DEFITNION
****************************************************************************/
// Predefined labels.
#define NULL_LABEL				"NULL"
#define TEST_LABEL				"TEST"
#define TEST_TYPE_LABEL			"TEST_TYPE"
#define TYPE_N_POINTS_LABEL		"N_POINTS"
#define TYPE_N_TESTS_LABEL		"N_TESTS"
#define TYPE_DELTA_STEP_LABEL	"DELTA_STEP"
#define TYPE_N_STEPS_LABEL		"N_STEPS"
#define TEST_FILE_LABEL			"FILE"

enum LabelType {UNKNOWN_LABEL,
				BEGIN_LABEL,
				END_LABEL,
				DATA_LABEL};

/****************************************************************************
// 							Label CLASS DEFITNION
****************************************************************************/
class Label
{
	//------------------------------------------------------------------------
	// Attributes.
	//------------------------------------------------------------------------
	string 		name;
	string 		beginLabel;
	string 		endLabel;
	string 		value;
	bool		isValid;
	LabelType 	type;

	//------------------------------------------------------------------------
	// Private functions.
	//------------------------------------------------------------------------
	static LabelType extractType(string &line);
	void extractBegin(string &line);
	void extractData(string &line);
	void extractEnd(string &line);
	void setNameFromBeginLabel();
	void setNameFromEndLabel();
	void reset();
public:
	//------------------------------------------------------------------------
	// Constructor/Destructor.
	//------------------------------------------------------------------------
	Label() : name(NULL_LABEL), isValid(false), type(LabelType(0)) {};
	Label(string name, string value, LabelType type);
	Label(const Label& label) : name(label.name), \
								beginLabel(label.beginLabel), \
								endLabel(label.endLabel), \
								value(label.value), \
								isValid(label.isValid), \
								type(label.type) {};

	//------------------------------------------------------------------------
	// Get/Set functions.
	//------------------------------------------------------------------------
	const string& getName() const {return this->name;}
	const string& getValue() const {return this->value;}
	const string& getBeginLabel() const {return beginLabel;}
	const string& getEndLabel() const {return endLabel;}
	bool isBeginLabel() {return(this->type == BEGIN_LABEL);};
	bool isEndLabel() {return(this->type == END_LABEL);};
	bool isDataLabel() {return(this->type == DATA_LABEL);};

	//------------------------------------------------------------------------
	// Public functions.
	//------------------------------------------------------------------------
	static bool validFormat(string &line);
	void extractLabel(string &line);
	void print() const;
};

#endif /* TEST_LABEL_H_ */
