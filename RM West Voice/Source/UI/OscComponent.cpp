/*
  ==============================================================================

    OscComponent.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId)
{
    juce::StringArray choices{ "Triangle","Saw" };
    oscWaveSelector.addItemList(choices, 1);
    addAndMakeVisible(oscWaveSelector);

    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorId, oscWaveSelector);
}
OscComponent::~OscComponent()
{
}

void OscComponent::paint (juce::Graphics& g)
{
}

void OscComponent::resized()
{
    oscWaveSelector.setBounds(0, 0, 90, 20);//TO BE CHANGED
}
