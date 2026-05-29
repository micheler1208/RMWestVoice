/*
  ==============================================================================

    OscComponent.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomSlider.h"

//==============================================================================
/*
*/

class OscComponent  : public juce::Component
{
public:
    OscComponent(juce::AudioProcessorValueTreeState& apvts);
    ~OscComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox waveSelector;
    CustomSlider mixSlider;
    CustomSlider detuneSlider;
    juce::Label mixLabel;
    juce::Label detuneLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> detuneAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
