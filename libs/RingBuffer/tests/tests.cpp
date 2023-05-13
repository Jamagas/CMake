#include <gtest/gtest.h>
#include "RingBuffer.hpp"

class RingBufferTest
:	public ::testing::Test
{
protected:
	void SetUp() override
	{
		ringBuffer = new RingBuffer(1024);
	}

	void TearDown() override
	{
		delete ringBuffer;
	}

	RingBuffer* ringBuffer = nullptr;
};

TEST_F (RingBufferTest, Create)
{
	ASSERT_NE(ringBuffer, nullptr); 
}

TEST_F (RingBufferTest, Empty_True)
{
	EXPECT_EQ(ringBuffer->empty(), true);
}

TEST_F (RingBufferTest, Empty_False)
{
	uint8_t data[4] = { 1, 2, 3, 4 };

	ringBuffer->push(data, sizeof(data));

	EXPECT_EQ(ringBuffer->empty(), false);
}

TEST_F (RingBufferTest, Full_True)
{
	uint8_t data[1024] = { 1, 2, 3, 4 };

	ringBuffer->push(data, sizeof(data));

	EXPECT_EQ(ringBuffer->full(), true);
}

TEST_F (RingBufferTest, Full_False)
{
	uint8_t data[4] = { 1, 2, 3, 4 };

	ringBuffer->push(data, sizeof(data));

	EXPECT_EQ(ringBuffer->full(), false);
}

TEST_F (RingBufferTest, Available_NonZero)
{
	uint8_t data[4] = { 1, 2, 3, 4 };

	ringBuffer->push(data, sizeof(data));

	EXPECT_EQ(ringBuffer->available(), 1024 - sizeof(data));
}

TEST_F (RingBufferTest, Available_Zero)
{
	uint8_t data[1024] = { 1, 2, 3, 4 };

	ringBuffer->push(data, sizeof(data));

	EXPECT_EQ(ringBuffer->available(), 0);
}

TEST_F (RingBufferTest, PushAndPop_SameData)
{
    uint8_t data[4] = { 1, 2, 3, 4 };
    uint8_t poppedData[4] = { 0 };

    ringBuffer->push(data, sizeof(data));
    ringBuffer->pop(poppedData, sizeof(poppedData));

    EXPECT_EQ(memcmp(data, poppedData, sizeof(data)), 0);
}

TEST_F (RingBufferTest, BoundaryConditions)
{
    uint8_t data[1024];
    uint8_t poppedData[1024] = { 0 };
    for (int i = 0; i < 1024; i++) data[i] = i % 256;

    EXPECT_TRUE(ringBuffer->push(data, sizeof(data)));
    EXPECT_EQ(ringBuffer->full(), true);

    EXPECT_TRUE(ringBuffer->pop(poppedData, sizeof(poppedData)));
    EXPECT_EQ(ringBuffer->empty(), true);
    EXPECT_EQ(memcmp(data, poppedData, sizeof(data)), 0);
}

TEST_F (RingBufferTest, Overcapacity)
{
    uint8_t data[1025];
    for (int i = 0; i < 1025; i++) data[i] = i % 256;

    EXPECT_FALSE(ringBuffer->push(data, sizeof(data)));
}

TEST_F (RingBufferTest, Underflow)
{
    uint8_t poppedData[1];

    EXPECT_FALSE(ringBuffer->pop(poppedData, sizeof(poppedData)));
}

TEST_F (RingBufferTest, BufferWrapAround)
{
    uint8_t data[512], secondData[512], poppedData[512];
    for (int i = 0; i < 512; i++) 
    {
        data[i] = i % 256;
        secondData[i] = (i + 128) % 256;
    }

    EXPECT_TRUE(ringBuffer->push(data, sizeof(data)));
    EXPECT_TRUE(ringBuffer->pop(poppedData, sizeof(poppedData)));
    EXPECT_EQ(memcmp(data, poppedData, sizeof(data)), 0);

    EXPECT_TRUE(ringBuffer->push(secondData, sizeof(secondData)));
    EXPECT_TRUE(ringBuffer->pop(poppedData, sizeof(poppedData)));
    EXPECT_EQ(memcmp(secondData, poppedData, sizeof(secondData)), 0);
}
