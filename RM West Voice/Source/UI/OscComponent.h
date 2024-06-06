/*
  ==============================================================================

    OscComponent.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeelCyan.h"

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
    juce::ToggleButton dayButton;
    juce::ToggleButton nightButton;
    juce::TextButton detuneButton;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> dayButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> nightButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> detuneButtonAttachment;

    CustomLookAndFeelCyan customLookAndFeelCyan; // New look and feel for detune button

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
