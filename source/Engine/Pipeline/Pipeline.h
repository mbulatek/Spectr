//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#pragma once

#include <pluginterfaces/vst/ivstaudioprocessor.h>
#include <boost/lockfree/spsc_queue.hpp>
#include <Engine/Pipeline/INode.h>

namespace spectr {
namespace engine {

class Pipeline {
	using AudioBuffer = Steinberg::Vst::AudioBusBuffers;

public:
	Pipeline();
	virtual ~Pipeline();

	void setInputQueue(boost::lockfree::spsc_queue<AudioBuffer> *inputQueue);
	void setOutputQueue();

	void pushNode(std::unique_ptr<INode> node);

private:
	boost::lockfree::spsc_queue<AudioBuffer> *inputQueue;
	boost::lockfree::spsc_queue<AudioBuffer> *outputQueue;
};

} // namespace engine
} // namespace spectr
