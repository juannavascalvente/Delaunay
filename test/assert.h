/*
 * assert.h
 *
 *  Created on: Dec 8, 2016
 *      Author: juan
 */

#ifndef CODE_TEST_ASSERT_H_
#define CODE_TEST_ASSERT_H_

class Assert
{

public:

	Assert() {};
	~Assert() {};

	static bool assertEqual(int value1, int value2);
	static bool assertNotEqual(int value1, int value2);
};

#endif /* CODE_TEST_ASSERT_H_ */
