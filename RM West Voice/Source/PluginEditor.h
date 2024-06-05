/*
  ==============================================================================

    PluginEditor.h
    Author:  micheler1208

  ==============================================================================
*/


#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/AdsrComponent.h"
#include "UI/OscComponent.h"
#include "UI/FilterComponent.h"

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
    RMWestVoiceAudioProcessor& audioProcessor;    
    
    // IMAGES
    juce::Image backgroundImage;

    //OSC
    OscComponent osc;

    //ADSR
    AdsrComponent adsr;

    //FILTERS
    FilterComponent filters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RMWestVoiceAudioProcessorEditor)
};
