//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#pragma once

#include <AudioRingBuffer.h>
#include <Utils/Logger.h>
#include <public.sdk/source/vst/vstaudioeffect.h>

#include <vector>

namespace spectr {

class SpectrProcessor : public Steinberg::Vst::AudioEffect {
 public:
  SpectrProcessor();
  ~SpectrProcessor() SMTG_OVERRIDE;

  //! \brief Create function
  static Steinberg::FUnknown* createInstance(void* /*context*/);

  //--- ---------------------------------------------------------------------
  // AudioEffect overrides:
  //--- ---------------------------------------------------------------------
  //! \brief Called at first after constructor
  Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown* context)
      SMTG_OVERRIDE;

  //! \brief Called at the end before destructor
  Steinberg::tresult PLUGIN_API terminate() SMTG_OVERRIDE;

  //! \brief Switch the Plug-in on/off
  Steinberg::tresult PLUGIN_API setActive(Steinberg::TBool state) SMTG_OVERRIDE;

  //! \brief Will be called before any process call
  Steinberg::tresult PLUGIN_API
  setupProcessing(Steinberg::Vst::ProcessSetup& newSetup) SMTG_OVERRIDE;

  Steinberg::tresult PLUGIN_API setProcessing(Steinberg::TBool state)
      SMTG_OVERRIDE;

  //! \brief Asks if a given sample size is supported see SymbolicSampleSizes
  Steinberg::tresult PLUGIN_API
  canProcessSampleSize(Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;

  //! \brief Here we go...the process call
  Steinberg::tresult PLUGIN_API process(Steinberg::Vst::ProcessData& data)
      SMTG_OVERRIDE;

  //! \brief For persistence
  Steinberg::tresult PLUGIN_API setState(Steinberg::IBStream* state)
      SMTG_OVERRIDE;
  Steinberg::tresult PLUGIN_API getState(Steinberg::IBStream* state)
      SMTG_OVERRIDE;

  bool getFFTData(AudioRingBuffer::SamplesBuffer& outData);

 private:
  AudioRingBuffer fftBuffer;
};

}  // namespace spectr
