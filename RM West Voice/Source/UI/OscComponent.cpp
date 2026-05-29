/*
  ==============================================================================

    OscComponent.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"
#include "../PluginParameters.h"

//==============================================================================

// COSTRUCTOR
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts)
{
    waveSelector.addItem("Tri", RMWestVoiceParameters::Wave::tri + 1);
    waveSelector.addItem("Saw", RMWestVoiceParameters::Wave::saw + 1);
    waveSelector.setSelectedId(RMWestVoiceParameters::Wave::tri + 1, juce::dontSendNotification);
    addAndMakeVisible(waveSelector);
    waveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        RMWestVoiceParameters::ID::wave,
        waveSelector);

    detuneSlider.setName("Detune");
    detuneSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    detuneSlider.setRange(-25.0, 25.0, 0.1);
    detuneSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    detuneSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    detuneSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(167, 240, 229));
    addAndMakeVisible(detuneSlider);
    detuneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        RMWestVoiceParameters::ID::detuneCents,
        detuneSlider);

    detuneLabel.setText("Detune", juce::dontSendNotification);
    detuneLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(167, 240, 229));
    detuneLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(detuneLabel);
}

// DESTRUCTOR
OscComponent::~OscComponent()
{
}

// PAINT
void OscComponent::paint (juce::Graphics& g)
{
    juce::ignoreUnused (g);
}

// RESIZED
void OscComponent::resized()
{
    waveSelector.setBounds(5, 0, 130, 24);
    detuneLabel.setBounds(15, 29, 110, 18);
    detuneSlider.setBounds(10, 48, 120, 52);
}
