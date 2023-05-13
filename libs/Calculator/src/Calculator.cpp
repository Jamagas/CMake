#include "Calculator.hpp"

#include <stdexcept>

// Constructor
Calculator::Calculator()
{
}

// Destructor
Calculator::~Calculator()
{
}

// Addition
double Calculator::add(double a, double b)
{
	return a + b;
}

// Subtraction
double Calculator::subtract(double a, double b)
{
	return a - b;
}

// Multiplication
double Calculator::multiply(double a, double b)
{
	return a * b;
}

// Division
double Calculator::divide(double a, double b)
{
	// Check for division by zero
	if(b == 0)
	{
		throw std::invalid_argument("Division by zero is undefined.");
	}
	return a / b;
}
