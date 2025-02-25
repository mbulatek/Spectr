//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#pragma once

#include <SpectrEditor.h>
#include <base/source/fobject.h>
#include <base/source/fstring.h>
#include <pluginterfaces/base/fplatform.h>
#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <public.sdk/source/vst/vsteditcontroller.h>
#include <vstgui4/vstgui/vstgui.h>

namespace spectr {

class SpectrController : public Steinberg::Vst::EditControllerEx1 {
 public:
  SpectrController() = default;
  ~SpectrController() SMTG_OVERRIDE = default;

  //! \brief Create function
  static Steinberg::FUnknown* createInstance(void* /*context*/) {
    return (Steinberg::Vst::IEditController*)new SpectrController;
  }

  //! from IPluginBase
  Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown* context)
      SMTG_OVERRIDE;
  Steinberg::tresult PLUGIN_API terminate() SMTG_OVERRIDE;

  //! from EditController
  Steinberg::tresult PLUGIN_API setComponentState(Steinberg::IBStream* state)
      SMTG_OVERRIDE;
  Steinberg::IPlugView* PLUGIN_API createView(Steinberg::FIDString name)
      SMTG_OVERRIDE;
  Steinberg::tresult PLUGIN_API setState(Steinberg::IBStream* state)
      SMTG_OVERRIDE;
  Steinberg::tresult PLUGIN_API getState(Steinberg::IBStream* state)
      SMTG_OVERRIDE;

  //! Interface
  DEFINE_INTERFACES
  //! Here you can add more supported VST3 interfaces
  //! DEF_INTERFACE (Vst::IXXX)
  END_DEFINE_INTERFACES(EditController)
  DELEGATE_REFCOUNT(EditController)
};

}  // namespace spectr
