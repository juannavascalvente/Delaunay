#include "Tester.h"
#include <gtest/gtest.h>
#include <stdio.h>
#include <math.h>


//------------------------------------------------------------------------
// Public functions.
//------------------------------------------------------------------------
int Tester::Execute(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
};

