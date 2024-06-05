/*
  ==============================================================================

    FilterComponent.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../CustomLookAndFeelViolet.h"

//==============================================================================
/*
*/
class FilterComponent  : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& apvts);
    ~FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    // SLIDERS CUSTOM STYLE
    CustomLookAndFeelViolet customLookAndFeelViolet;

    // CUTOFF
    juce::Slider cutoffSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttachment;
    
    // VOLUME
    juce::Slider volumeSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;

    // LABELS
    juce::Label cutoffLabel, volumeLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
