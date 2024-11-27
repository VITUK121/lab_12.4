#include "pch.h"
#include "CppUnitTest.h"
#include "../lab_12.4/lab_12.4.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest124
{
	TEST_CLASS(UnitTest124)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Assert::AreEqual(calc_n(1000), 4);
		}
	};
}
