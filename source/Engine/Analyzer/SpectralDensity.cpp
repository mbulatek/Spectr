//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#include <Engine/Analyzer/SpectralDensity.h>
#include <cstdlib>
#include <map>

#include <Utils/Logger.h>

namespace spectr {
namespace engine {

//int SpectralDensity::FFTWInput::N = 4096;

SpectralDensity::SpectralDensity() {
	int fs = 42100;

	m_fftwIn.buffer  = (double*) fftw_malloc(sizeof(double) * FFTWInput::N);
	m_fftwIn.size = 0;
	m_fftwOut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (FFTWInput::N/2 + 1));

	// Plan generation for real numbers
	m_fftwPlan = fftw_plan_dft_r2c_1d(FFTWInput::N, m_fftwIn.buffer, m_fftwOut, FFTW_PATIENT);

	/*for (int i = 0; i < N; i++) {
		in[i] = sin(2.0 * M_PI * i / N);
	}*/
}

SpectralDensity::~SpectralDensity() {
	fftw_destroy_plan(m_fftwPlan);
	fftw_free(m_fftwIn.buffer);
	fftw_free(m_fftwOut);
}


void SpectralDensity::process() {
	AudioBuffer newBuffer;
	bool dataReady = false;
	int dataCount = 0;

	while (m_inputQueues[0].pop(newBuffer))
	{
		dataCount = newBuffer.numSamples;
		if (newBuffer.numSamples + m_fftwIn.size < FFTWInput::N) {
			dataCount = newBuffer.numSamples;
		}
		else {
			dataCount = FFTWInput::N - m_fftwIn.size;
			dataReady = true;
		}

		memcpy(m_fftwIn.buffer, newBuffer.inputs[0].channelBuffers64[0],
				dataCount * sizeof(Steinberg::Vst::Sample64));
	}

	if (dataReady)
	{
		fftw_execute(m_fftwPlan);

		// Copy rest of the data
		memcpy(m_fftwIn.buffer, newBuffer.inputs[0].channelBuffers64[0],
			   newBuffer.numSamples - dataCount * sizeof(Steinberg::Vst::Sample64));

		std::map<double, double> frequency_amplitude_map;

		// Amplitude calculation
		for (int i = 0; i < FFTWInput::N/2 + 1; ++i) {
			double frequency = i * static_cast<double>(FS) / FFTWInput::N;
			double amplitude = sqrt(m_fftwOut[i][0] * m_fftwOut[i][0] + m_fftwOut[i][1] * m_fftwOut[i][1]) / FFTWInput::N;
			frequency_amplitude_map[frequency] = amplitude;
		}


		for (const auto& entry : frequency_amplitude_map) {
			spdlog::debug(std::string("Frequency: ") + std::to_string(entry.first) + " Hz, Amplitude: " + std::to_string(entry.second));
		}
	}
}


} /* namespace engine */
} /* namespace spectr */
