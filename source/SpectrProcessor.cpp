//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#include <AudioRingBuffer.h>
#include <SpectrCIds.h>
#include <SpectrProcessor.h>
#include <UI/SpectrogramView.h>
#include <Utils/Logger.h>
#include <base/source/fstreamer.h>
#include <pluginterfaces/vst/ivstparameterchanges.h>

#include <cstdlib>
#include <string>

using namespace Steinberg;

namespace spectr {

static SpectrogramViewCreator spectrogramViewCreator;

Steinberg::FUnknown* SpectrProcessor::createInstance(void* /*context*/) {
  auto instance = new SpectrProcessor;

  spectrogramViewCreator.setProcessor(instance);

  return (Steinberg::Vst::IAudioProcessor*)instance;
}

SpectrProcessor::SpectrProcessor() : fftBuffer(1024) {
  //--- set the wanted controller for our processor
  setControllerClass(kSpectrControllerUID);
}

//------------------------------------------------------------------------
SpectrProcessor::~SpectrProcessor() {}

//------------------------------------------------------------------------
tresult PLUGIN_API SpectrProcessor::initialize(FUnknown* context) {
  // Here the Plug-in will be instantiated

  //---always initialize the parent-------
  tresult result = AudioEffect::initialize(context);
  // if everything Ok, continue
  if (result != kResultOk) {
    return result;
  }

  //--- create Audio IO ------
  addAudioInput(STR16("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
  addAudioOutput(STR16("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

  /* If you don't need an event bus, you can remove the next line */
  addEventInput(STR16("Event In"), 1);

  return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SpectrProcessor::terminate() {
  // Here the Plug-in will be de-instantiated, last possibility to remove some
  // memory!

  //---do not forget to call parent ------
  return AudioEffect::terminate();
}

//------------------------------------------------------------------------
tresult PLUGIN_API SpectrProcessor::setActive(TBool state) {
  //--- called when the Plug-in is enable/disable (On/Off) -----
  spdlog::debug(state ? "Activating (setActive: ON)"
                      : "Deactivating (setActive: OFF)");

  return AudioEffect::setActive(state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API SpectrProcessor::process(Vst::ProcessData& data) {
  //--- First : Read inputs parameter changes-----------

  /*if (data.inputParameterChanges)
  {
    int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
    for (int32 index = 0; index < numParamsChanged; index++)
    {
      if (auto* paramQueue = data.inputParameterChanges->getParameterData
  (index))
      {
        Vst::ParamValue value;
        int32 sampleOffset;
        int32 numPoints = paramQueue->getPointCount ();
        switch (paramQueue->getParameterId ())
        {
        }
      }
    }
  }*/

  spdlog::trace("Processing... ");

  if (data.numInputs == 0 || data.numOutputs == 0) {
    spdlog::debug("No input/output");
    return kResultOk;
  }

  if (data.numSamples > 0) {
    //--- ------------------------------------------
    // here as example a default implementation where we try to copy the inputs
    // to the outputs: if less input than outputs then clear outputs
    //--- ------------------------------------------

    int32 minBus = std::min(data.numInputs, data.numOutputs);

    for (int32 i = 0; i < minBus; i++) {
      int32 minChan =
          std::min(data.inputs[i].numChannels, data.outputs[i].numChannels);
      for (int32 c = 0; c < minChan; c++) {
        if (c == 0) {
          float* fftFrame =
              (float*)malloc(data.numSamples * sizeof(Vst::Sample32));
          if (fftFrame != NULL) {
            spdlog::trace("Pushing into ring buffer");
            memcpy(fftFrame, data.inputs[i].channelBuffers32[c],
                   data.numSamples * sizeof(Vst::Sample32));
            fftBuffer.push(
                AudioRingBuffer::SamplesBuffer(fftFrame, data.numSamples));
          } else {
            spdlog::error("Data allocation error");
          }
        }

        // do not need to be copied if the buffers are the same
        if (data.outputs[i].channelBuffers32[c] !=
            data.inputs[i].channelBuffers32[c]) {
          memcpy(data.outputs[i].channelBuffers32[c],
                 data.inputs[i].channelBuffers32[c],
                 data.numSamples * sizeof(Vst::Sample32));
        }
      }
      data.outputs[i].silenceFlags = data.inputs[i].silenceFlags;

      // clear the remaining output buffers
      for (int32 c = minChan; c < data.outputs[i].numChannels; c++) {
        // clear output buffers
        memset(data.outputs[i].channelBuffers32[c], 0,
               data.numSamples * sizeof(Vst::Sample32));

        // inform the host that this channel is silent
        data.outputs[i].silenceFlags |= ((uint64)1 << c);
      }
    }
    // clear the remaining output buffers
    for (int32 i = minBus; i < data.numOutputs; i++) {
      // clear output buffers
      for (int32 c = 0; c < data.outputs[i].numChannels; c++) {
        memset(data.outputs[i].channelBuffers32[c], 0,
               data.numSamples * sizeof(Vst::Sample32));
      }
      // inform the host that this bus is silent
      data.outputs[i].silenceFlags =
          ((uint64)1 << data.outputs[i].numChannels) - 1;
    }
  }

  return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API
SpectrProcessor::setupProcessing(Vst::ProcessSetup& newSetup) {
  //--- called before any processing ----
  spdlog::debug("Setup processing...");
  newSetup.processMode = Vst::kRealtime;
  return AudioEffect::setupProcessing(newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API
SpectrProcessor::canProcessSampleSize(int32 symbolicSampleSize) {
  // by default kSample32 is supported
  if (symbolicSampleSize == Vst::kSample32)
    return kResultTrue;

  // disable the following comment if your processing support kSample64
  if (symbolicSampleSize == Vst::kSample64) {
    spdlog::debug("Processing 64bit sample size");
    return kResultTrue;
  }

  return kResultFalse;
}

tresult PLUGIN_API SpectrProcessor::setProcessing(Steinberg::TBool state) {
  spdlog::debug(state ? "Processing ON" : "Processing OFF");

  return Steinberg::kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SpectrProcessor::setState(IBStream* state) {
  // called when we load a preset, the model has to be reloaded
  IBStreamer streamer(state, kLittleEndian);

  return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SpectrProcessor::getState(IBStream* state) {
  // here we need to save the model
  IBStreamer streamer(state, kLittleEndian);

  return kResultOk;
}

bool SpectrProcessor::getFFTData(AudioRingBuffer::SamplesBuffer& outData) {
  return fftBuffer.pop(outData);
}

//------------------------------------------------------------------------
}  // namespace spectr
