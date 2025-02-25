//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#pragma once

#include <vector>
#include <atomic>
#include <cstddef>

#include <Utils/Logger.h>

namespace spectr
{

class AudioRingBuffer
{
public:
	struct SamplesBuffer
	{
		float *buffer;
		int size;
		SamplesBuffer() noexcept :
			buffer(nullptr),
			size(0) {}
		SamplesBuffer(float *_buffer, int _size) noexcept :
			buffer(_buffer),
			size(_size) {}
	};


    AudioRingBuffer(size_t size) : buffer(size), head(0), tail(0), capacity(size) {}

    bool push(SamplesBuffer data)
    {
    	size_t nextHead = (head + 1) % capacity;
        if (nextHead == tail)
            return false;

        buffer[head] = data;
        head = nextHead;
        return true;
    }

    bool pop(SamplesBuffer &outData)
    {
        if (tail == head)
            return false;

        outData = buffer[tail];
        tail = (tail + 1) % capacity;
        return true;
    }

private:
    std::vector<SamplesBuffer> buffer;
    std::atomic<size_t> head;
    std::atomic<size_t> tail;
    size_t capacity;
};

} // namespace spectr
