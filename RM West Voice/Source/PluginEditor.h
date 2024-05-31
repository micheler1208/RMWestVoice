/*
  ==============================================================================

    PluginEditor.h
    Author:  micheler1208

  ==============================================================================
*/


#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeelViolet.h"
#include "PluginProcessor.h"
#include "UI/AdsrComponent.h"

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

    //ADSR
    AdsrComponent adsr;

    //CUSTOM SLIDE
    CustomLookAndFeelViolet customLookAndFeelViolet;

    // OSCILLATOR SELECTOR
    juce::ComboBox  oscSelector; 
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelectorAttachment;

    // LP CUTOFF AND VOLUME
    juce::Slider cutoffSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment;
    juce::Slider volumeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;
    
    // LABELS
    juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel, cutoffLabel;
    juce::Label volumeLabel;    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RMWestVoiceAudioProcessorEditor)
};
