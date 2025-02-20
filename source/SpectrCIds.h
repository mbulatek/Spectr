//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#pragma once

#include <pluginterfaces/base/funknown.h>
#include <pluginterfaces/vst/vsttypes.h>

namespace MaroVST {

static const Steinberg::FUID kSpectrProcessorUID(0x875965F5, 0x366558DF,
                                                 0xB4C01848, 0x017AA95C);
static const Steinberg::FUID kSpectrControllerUID(0xB751A80D, 0x8FF35C8A,
                                                  0xB9D4D9BF, 0x98B63E64);

#define SpectrVST3Category "Fx|Analyzer"

}  // namespace MaroVST
