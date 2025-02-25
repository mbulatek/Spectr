//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#include <SpectrCIds.h>
#include <SpectrController.h>
#include <Utils/Logger.h>
#include <vstgui4/vstgui/plugin-bindings/vst3editor.h>

using namespace Steinberg;

namespace spectr {

tresult PLUGIN_API SpectrController::initialize(FUnknown* context) {
  // Here the Plug-in will be instantiated

  //---do not forget to call parent ------
  tresult result = EditControllerEx1::initialize(context);
  if (result != kResultOk) {
    return result;
  }

  SetupLogger();

  // Here you could register some parameters

  return result;
}

tresult PLUGIN_API SpectrController::terminate() {
  // Here the Plug-in will be de-instantiated, last possibility to remove some
  // memory!

  //---do not forget to call parent ------
  return EditControllerEx1::terminate();
}

tresult PLUGIN_API SpectrController::setComponentState(IBStream* state) {
  // Here you get the state of the component (Processor part)
  if (!state)
    return kResultFalse;

  return kResultOk;
}

tresult PLUGIN_API SpectrController::setState(IBStream* state) {
  // Here you get the state of the controller

  return kResultTrue;
}

tresult PLUGIN_API SpectrController::getState(IBStream* state) {
  // Here you are asked to deliver the state of the controller (if needed)
  // Note: the real state of your plug-in is saved in the processor

  return kResultTrue;
}

IPlugView* PLUGIN_API SpectrController::createView(FIDString name) {
  // Here the Host wants to open your editor (if you have one)
  if (FIDStringsEqual(name, Vst::ViewType::kEditor)) {
    // create your editor here and return a IPlugView ptr of it
    auto* view = new SpectrEditor(this);
    view->setIdleRate(5);
    return view;
  }
  return nullptr;
}

}  // namespace spectr
