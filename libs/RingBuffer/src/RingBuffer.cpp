#include "RingBuffer.hpp"

#include <cstring>

RingBuffer::RingBuffer(size_t size)
: m_capacity(size)
{
	m_buffer = new uint8_t[size];
}

RingBuffer::~RingBuffer()
{
	delete[] m_buffer;
}

bool RingBuffer::push(uint8_t* data, size_t size)
{
	if (! data || ! size || full())
		return false;

	if (size > available())
		return false;

	// If the data does not wrap around the end of the buffer,
	// we can simply copy it into the buffer
	// at the position pointed to by the head.
	if (m_head + size <= m_capacity)
	{
		memcpy(m_buffer + m_head, data, size);
	}
	else
	{
		// If the data does wrap around the end of the buffer,
		// we need to split it into two parts
		// and copy each part into the buffer separately.
		size_t firstPart = m_capacity - m_head;

		// The first part is copied into the space
		memcpy(m_buffer + m_head, data, firstPart);

		// remaining at the end of the buffer, and the second part
		// is copied from the start of the buffer.
		memcpy(m_buffer, data + firstPart, size - firstPart);
	}

	// Move the head forward by the size of the data,
	// wrapping around to the start of the buffer if necessary.
	m_head = (m_head + size) % m_capacity;

	if (m_head == m_tail)
		m_isFull = true;

	// If we've made it to this point, then we have successfully
	// pushed the data into the buffer.
	return true;
}

bool RingBuffer::pop(uint8_t* data, size_t size)
{
	if (! data || ! size || empty())
		return false;

	if (size > this->size())
		return false;

	// If the data does not wrap around the end of the buffer,
	// we can simply copy it from the buffer
	// at the position pointed to by the tail.
	if (m_tail + size <= m_capacity)
	{
		memcpy(data, m_buffer + m_tail, size);
	}
	else
	{
		// If the data does wrap around the end of the buffer,
		// we need to split it into two parts
		// and copy each part from the buffer separately.
		size_t firstPart = m_capacity - m_tail;

		// The first part is copied from the space
		memcpy(data, m_buffer + m_tail, firstPart);

		// remaining at the end of the buffer, and the second part is
		// copied from the start of the buffer.
		memcpy(data + firstPart, m_buffer, size - firstPart);
	}

	// Move the tail forward by the size of the data, wrapping around to the start of the buffer if necessary.
	m_tail = (m_tail + size) % m_capacity;
	m_isFull = false;

	// If we've made it to this point, then we have successfully popped the data from the buffer.
	return true;
}

size_t RingBuffer::size() const
{
	if (m_head == m_tail)
	{
		return m_isFull ? m_capacity : 0;
	}
	else if (m_head > m_tail)
	{
		return m_head - m_tail;
	}

	// m_tail > m_head
	return m_capacity - m_tail + m_head;
}

size_t RingBuffer::available() const
{
	if (m_head == m_tail)
	{
		return m_isFull ? 0 : m_capacity;
	}
	else if (m_head > m_tail)
	{
		return m_capacity - (m_head - m_tail);
	}

	// m_tail > m_head
	return m_tail - m_head - 1;
}

bool RingBuffer::empty() const
{
	return (! full() && (m_head == m_tail));
}

bool RingBuffer::full() const
{
	return m_isFull;
}
