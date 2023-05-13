#include <gtest/gtest.h>
#include "Calculator.hpp"

class CalculatorTest
:	public ::testing::Test
{
protected:
	void SetUp() override
	{
		calc = new Calculator();
	}

	void TearDown() override
	{
		delete calc;
	}

	Calculator* calc = nullptr;
};

TEST_F (CalculatorTest, Add) {
	EXPECT_EQ(calc->add(4, 5), 9);
}

TEST_F (CalculatorTest, Subtract) {
	EXPECT_EQ(calc->subtract(4, 9), -5);
}

TEST_F (CalculatorTest, Multiply) {
	EXPECT_EQ(calc->multiply(3, -7), -21);
}

TEST_F (CalculatorTest, Divide) {
	EXPECT_EQ(calc->divide(1024, 512), 2);
}

TEST_F (CalculatorTest, DivideByZero) {
	ASSERT_THROW(calc->divide(4, 0), std::invalid_argument);
}
