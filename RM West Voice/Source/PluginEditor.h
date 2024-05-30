/*
  ==============================================================================

    PluginEditor.h
    Author:  micheler1208

  ==============================================================================
*/


#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeelYellow.h"
#include "CustomLookAndFeelViolet.h"
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
    RMWestVoiceAudioProcessor& audioProcessor;    
    
    // IMAGES
    juce::Image backgroundImage;
    
    // SLIDERS
    CustomLookAndFeelYellow customLookAndFeelYellow;
    CustomLookAndFeelViolet customLookAndFeelViolet;

    // OSCILLATOR SELECTOR
    juce::ComboBox  oscSelector; 

    // ADSR
    juce::Slider attackSlider, decaySlider, sustainSlider, releaseSlider;

    // LP CUTOFF AND VOLUME
    juce::Slider cutoffSlider;
    juce::Slider volumeSlider;
    
    // LABELS
    juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel, cutoffLabel;
    juce::Label volumeLabel;
    
    // SLIDER ATTACHMENT
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelectorAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RMWestVoiceAudioProcessorEditor)
};
