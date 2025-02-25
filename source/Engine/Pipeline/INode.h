//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#pragma once

#include <list>

#include <boost/lockfree/spsc_queue.hpp>

#include <pluginterfaces/vst/ivstaudioprocessor.h>

namespace spectr {
namespace engine {

class INode {
	using AudioBuffer = Steinberg::Vst::ProcessData;

public:
	INode();
	virtual ~INode();

	void addInput(boost::lockfree::spsc_queue<AudioBuffer> *input)
	{
		m_inputQueues = input;
	}

	void addOutput(boost::lockfree::spsc_queue<AudioBuffer> *output)
	{
		m_outputQueues = output;
	}

	virtual void process() = 0;

protected:
	boost::lockfree::spsc_queue<AudioBuffer> *m_inputQueues;
	boost::lockfree::spsc_queue<AudioBuffer> *m_outputQueues;
};

} // namespace engine
} // namespace spectr
