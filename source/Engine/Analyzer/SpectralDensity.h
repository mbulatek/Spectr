//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#pragma once

#include <fftw3.h>
#include <pluginterfaces/vst/ivstaudioprocessor.h>

#include <Engine/Pipeline/INode.h>

namespace spectr {
namespace engine {

class SpectralDensity: public INode {
	using AudioBuffer = Steinberg::Vst::ProcessData;

	struct FFTWInput {
		double *buffer;
		int size;
		static constexpr int N = 4096;
	};

public:
	SpectralDensity();
	virtual ~SpectralDensity();

	void process() override;

private:
	FFTWInput m_fftwIn;
	fftw_complex *m_fftwOut;
	fftw_plan m_fftwPlan;
	static constexpr int FS = 44100;
};

} /* namespace engine */
} /* namespace spectr */
