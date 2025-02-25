//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#include <UI/SpectrogramView.h>
#include <Utils/Logger.h>
#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <vstgui4/vstgui/lib/vstguiinit.h>
#include <vstgui4/vstgui/plugin-bindings/vst3editor.h>
#include <vstgui4/vstgui/uidescription/iviewcreator.h>
#include <vstgui4/vstgui/uidescription/iviewfactory.h>
#include <vstgui4/vstgui/vstgui.h>

namespace spectr {

class SpectrEditor : public VSTGUI::VST3Editor {
 public:
  SpectrEditor(Steinberg::Vst::EditControllerEx1* controller)
      : VSTGUI::VST3Editor(controller, "view", "myplugineditor.uidesc") {}

  void onIdle() {
    spdlog::trace("SpectrEditor::onIdle()");
    if (getFrame()) {
      getFrame()->invalidRect(VSTGUI::CRect(0, 0, 400, 300));
    }
  }
};

}  // namespace spectr
