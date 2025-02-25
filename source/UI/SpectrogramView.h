//------------------------------------------------------------------------
// Copyright(c) 2025 My Plug-in Company.
//------------------------------------------------------------------------

#pragma once

#include <AudioRingBuffer.h>
#include <SpectrProcessor.h>
#include <Utils/Logger.h>
#include <base/source/fdebug.h>
#include <vstgui4/vstgui/lib/cview.h>
#include <vstgui4/vstgui/lib/vstguibase.h>
#include <vstgui4/vstgui/uidescription/iuidescription.h>
#include <vstgui4/vstgui/uidescription/iviewcreator.h>
#include <vstgui4/vstgui/uidescription/uiattributes.h>
#include <vstgui4/vstgui/uidescription/uiviewfactory.h>
#include <vstgui4/vstgui/vstgui.h>

#include <cstdlib>

namespace spectr {
class SpectrogramView : public VSTGUI::CView {
 public:
  SpectrogramView(const VSTGUI::CRect& size)
      : CView(size), m_processor(nullptr) {
    setWantsIdle(true);
    previousPoint = VSTGUI::CPoint(0, getViewSize().getHeight() / 2);
  }

  void setProcessor(SpectrProcessor* processor) { m_processor = processor; }

  VSTGUI::CPoint previousPoint;

  void draw(VSTGUI::CDrawContext* pContext) override {
    pContext->setFillColor(VSTGUI::CColor(0, 82, 73, 92));
    pContext->drawRect(getViewSize(), VSTGUI::CDrawStyle::kDrawFilled);

    VSTGUI::CRect bounds = getViewSize();
    VSTGUI::CPoint center(bounds.getCenter());
    pContext->setFrameColor(VSTGUI::CColor(0, 0, 0));

    pContext->setLineWidth(2);
    VSTGUI::CPoint left(bounds.left, bounds.top + bounds.getHeight() / 2);
    VSTGUI::CPoint right(bounds.right, bounds.top + bounds.getHeight() / 2);
    pContext->drawLine(left, right);

    if (m_fftData.size != 0) {
      spdlog::trace("Drawing data: size=" + std::to_string(m_fftData.size));
    }

    for (size_t i = 0; i < m_fftData.size; ++i) {
      static float widthStep = getViewSize().getWidth() / m_fftData.size;
      /*float height = m_fftData.buffer[i] * getViewSize().getHeight();
      VSTGUI::CRect bar(i * widthStep, getViewSize().bottom - height, (i + 1) *
      widthStep, getViewSize().bottom);

      pContext->setFillColor(VSTGUI::CColor(255, 0, 0));
      pContext->drawRect(bar, VSTGUI::kDrawFilled);*/

      float height = m_fftData.buffer[i] * getViewSize().getHeight() / 2;
      VSTGUI::CPoint currentPoint(i * widthStep,
                                  bounds.top + bounds.getHeight() / 2 + height);

      pContext->setFillColor(VSTGUI::CColor(255, 0, 0));
      pContext->setFrameColor(VSTGUI::CColor(255, 0, 0));
      pContext->drawLine(previousPoint, currentPoint);
      previousPoint = currentPoint;
    }
    m_fftData.size = 0;
    if (m_fftData.buffer != nullptr)
      free(m_fftData.buffer);
    m_fftData.buffer = nullptr;

    setDirty(false);
  }

  void onIdle() override {
    updateFFT();
    CView::onIdle();
  }

  void updateFFT() {
    AudioRingBuffer::SamplesBuffer lastData;
    if (m_processor && m_processor->getFFTData(lastData)) {
      while (m_processor->getFFTData(m_fftData)) {
        free(lastData.buffer);
        lastData.buffer = 0;
        lastData = m_fftData;
      }
      m_fftData = lastData;
      setDirty(true);
    }
  }

 private:
  SpectrProcessor* m_processor;
  AudioRingBuffer::SamplesBuffer m_fftData;
};

class SpectrogramViewCreator : public VSTGUI::ViewCreatorAdapter {
  SpectrProcessor* m_processor;

 public:
  void setProcessor(SpectrProcessor* processor) { m_processor = processor; }

  // register this class with the view factory
  SpectrogramViewCreator() : m_processor(nullptr) {
    VSTGUI::UIViewFactory::registerViewCreator(*this);
  }

  // return an unique name here
  VSTGUI::IdStringPtr getViewName() const { return "SpectrogramView"; }

  // return the name here from where your custom view inherites.
  // Your view automatically supports the attributes from it.
  VSTGUI::IdStringPtr getBaseViewName() const { return "CView"; }

  // create your view here.
  // Note you don't need to apply attributes here as the apply method will be
  // called with this new view
  VSTGUI::CView* create(const VSTGUI::UIAttributes& attributes,
                        const VSTGUI::IUIDescription* description) const {
    auto instance = new SpectrogramView(VSTGUI::CRect(0, 0, 200, 200));
    instance->setProcessor(m_processor);
    return instance;
  }

  // apply custom attributes to your view
  bool apply(VSTGUI::CView* view, const VSTGUI::UIAttributes& attributes,
             const VSTGUI::IUIDescription* description) const {
    SpectrogramView* spectrogramView = dynamic_cast<SpectrogramView*>(view);
    if (spectrogramView == 0)
      return false;
    /*int32_t value;
    if (attributes.getIntegerAttribute("my-custom-attribute", value))
      spectrogramView->setCustomAttribute(value);*/
    return true;
  }

  // add your custom attributes to the list
  bool getAttributeNames(StringList& attributeNames) const {
    attributeNames.emplace_back("my-custom-attribute");
    return true;
  }

  // return the type of your custom attributes
  AttrType getAttributeType(const std::string& attributeName) const {
    if (attributeName == "my-custom-attribute")
      return kIntegerType;
    return kUnknownType;
  }

  // return the string value of the custom attributes of the view
  bool getAttributeValue(VSTGUI::CView* view, const string& attributeName,
                         string& stringValue,
                         const VSTGUI::IUIDescription* desc) const {
    SpectrogramView* spectrogramView = dynamic_cast<SpectrogramView*>(view);
    if (spectrogramView == 0)
      return false;
    /*if (attributeName == "my-custom-attribute")
    {
      stringValue =
    VSTGUI::UIAttributes::integerToString(spectrogramView->getCustomAttribute());
      return true;
    }*/
    return false;
  }
};

}  // namespace spectr
