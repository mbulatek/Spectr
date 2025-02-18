//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#pragma once

#include <vector>

#include "public.sdk/source/vst/vstaudioeffect.h"

#include "AudioRingBuffer.h"

namespace MaroVST {

class SpectrProcessor : public Steinberg::Vst::AudioEffect
{
public:
	SpectrProcessor ();
	~SpectrProcessor () SMTG_OVERRIDE;

    // Create function
	static Steinberg::FUnknown* createInstance(void* /*context*/);

	//--- ---------------------------------------------------------------------
	// AudioEffect overrides:
	//--- ---------------------------------------------------------------------
	/** Called at first after constructor */
	Steinberg::tresult PLUGIN_API initialize (Steinberg::FUnknown* context) SMTG_OVERRIDE;
	
	/** Called at the end before destructor */
	Steinberg::tresult PLUGIN_API terminate () SMTG_OVERRIDE;
	
	/** Switch the Plug-in on/off */
	Steinberg::tresult PLUGIN_API setActive (Steinberg::TBool state) SMTG_OVERRIDE;

	/** Will be called before any process call */
	Steinberg::tresult PLUGIN_API setupProcessing (Steinberg::Vst::ProcessSetup& newSetup) SMTG_OVERRIDE;
	
	Steinberg::tresult PLUGIN_API SpectrProcessor::setProcessing(Steinberg::TBool state) SMTG_OVERRIDE
	{
	    Logger::logToFile(state ? "SpectrProcessor: Processing ON" : "SpectrProcessor: Processing OFF");

	    return Steinberg::kResultOk;
	}

	/** Asks if a given sample size is supported see SymbolicSampleSizes. */
	Steinberg::tresult PLUGIN_API canProcessSampleSize (Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;

	/** Here we go...the process call */
	Steinberg::tresult PLUGIN_API process (Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;
		
	/** For persistence */
	Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;

	bool getFFTData(AudioRingBuffer::SamplesBuffer &outData);

private:
	AudioRingBuffer fftBuffer;
};

} // namespace MaroVST
