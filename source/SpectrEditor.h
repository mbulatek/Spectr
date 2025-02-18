#include "pluginterfaces/vst/ivsteditcontroller.h"

#include "vstgui4/vstgui/vstgui.h"
#include "vstgui4/vstgui/lib/vstguiinit.h"
#include "vstgui4/vstgui/uidescription/iviewcreator.h"
#include "vstgui4/vstgui/uidescription/iviewfactory.h"
#include "vstgui4/vstgui/plugin-bindings/vst3editor.h"

#include "UI/CSpectrogramView.h"
#include "Utils/Logger.h"

namespace MaroVST {

class SpectrEditor : public VSTGUI::VST3Editor {
public:
    SpectrEditor(Steinberg::Vst::EditControllerEx1 *controller)
        : VSTGUI::VST3Editor(controller, "view", "myplugineditor.uidesc")
    {}

    void onIdle()
    {
    	Logger::logToFile("SpectrEditor::onIdle()");
        if (getFrame())
        {
            getFrame()->invalidRect(VSTGUI::CRect(0, 0, 400, 300));
        }

        //VSTGUI::VST3Editor::onIdle();
    }

/*protected:
    void SpectrEditor::onRegisterCustomView(VSTGUI::IViewFactory* factory)
    {
        factory->registerViewCreator([]() -> VSTGUI::IViewCreator* {
            class SpectrogramViewCreator : public VSTGUI::CustomViewCreator<CSpectrogramView>
            {
            public:
                SpectrogramViewCreator() : CustomViewCreator("CSpectrogramView") {}
            };
            return new SpectrogramViewCreator();
            });
    }*/
};

} // namespace MaroVST
