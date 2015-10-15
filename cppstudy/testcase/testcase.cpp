
#include "testcase.h"
#include <iostream>
using namesapce std;

#define TEST_ASSERT(e) do{if(!(e))throw "Test Error!!!";}while(0)
#define TEST_CASE()                             \
		extern void TEST_CASE##NAME()           \
		namesapce unittest                      \
		{                                       \
			class TEST_RUNNER_##NAME            \
			{                                   \
			public:                             \
				TEST_RUNNER_##NAME(arguments)   \
				{                               \
					TEST_CASE();                \
				}                               \
			}TEST_CASE_##INSTANCE;              \
		}                                       \
void TEST_CASE()
{
	//TEST_ASSERT()
}

int main()
{
	return 0;
}