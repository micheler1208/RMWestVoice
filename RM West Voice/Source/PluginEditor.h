/*
  ==============================================================================

    PluginEditor.h
    Author:  micheler1208

  ==============================================================================
*/


#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class RMWestVoiceAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    RMWestVoiceAudioProcessorEditor (RMWestVoiceAudioProcessor&);
    ~RMWestVoiceAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    class ParameterSection;

    RMWestVoiceAudioProcessor& audioProcessor;

    juce::Image backgroundImage;

    std::unique_ptr<ParameterSection> performanceSection;
    std::unique_ptr<ParameterSection> toneSection;
    std::unique_ptr<ParameterSection> filterSection;
    std::unique_ptr<ParameterSection> fxSection;
    std::unique_ptr<ParameterSection> outputSection;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RMWestVoiceAudioProcessorEditor)
};
