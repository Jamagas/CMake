#include "AppConfig.hpp"

#include "Calculator.hpp"
#include "RingBuffer.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
	std::cout << argv[0] << " Version "
		<< App_VERSION_MAJOR << "."
		<< App_VERSION_MINOR << std::endl;

#if defined (USE_CALCULATOR) && (USE_CALCULATOR > 0)
	Calculator calc;

	std::cout << "5 + 4 = " << calc.add(5, 4) << std::endl;
#endif

#if defined (USE_RINGBUFFER) && (USE_RINGBUFFER > 0)
	RingBuffer ringBuffer(10);

	for (uint8_t i = 0; i < 10; ++i)
	{
		if (! ringBuffer.push(&i, sizeof(i)))
		{
			std::cout << "Failed to push to ringbuffer at " << i << std::endl;
			exit(1);
		}
	}

	std::cout << "Ring buffer contents: ";
	for (uint8_t i = 0; i < 10; ++i)
	{
		uint8_t data;
		if (! ringBuffer.pop(&data, sizeof(data)))
		{
			std::cout << "Failed to push to ringbuffer at " << i << std::endl;
			exit(1);
		}
		std::cout << data << ", ";
	}

	std::cout << std::endl;
#endif

	return 0;
}