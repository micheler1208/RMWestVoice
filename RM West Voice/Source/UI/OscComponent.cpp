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
    waveSelector.addItem("Saw", RMWestVoiceParameters::Wave::saw + 1);
    waveSelector.addItem("Tri", RMWestVoiceParameters::Wave::tri + 1);
    waveSelector.addItem("Saw+Tri", RMWestVoiceParameters::Wave::sawTri + 1);
    waveSelector.addItem("Saw+Pulse", RMWestVoiceParameters::Wave::sawPulse + 1);
    waveSelector.setSelectedId(RMWestVoiceParameters::Wave::sawTri + 1, juce::dontSendNotification);
    addAndMakeVisible(waveSelector);
    waveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        apvts,
        RMWestVoiceParameters::ID::wave,
        waveSelector);

    mixSlider.setName("Mix");
    mixSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mixSlider.setRange(0.0, 1.0, 0.01);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 58, 20);
    mixSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    mixSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(167, 240, 229));
    addAndMakeVisible(mixSlider);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        RMWestVoiceParameters::ID::oscMix,
        mixSlider);

    detuneSlider.setName("Detune");
    detuneSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    detuneSlider.setRange(-25.0, 25.0, 0.1);
    detuneSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 62, 20);
    detuneSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    detuneSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(167, 240, 229));
    addAndMakeVisible(detuneSlider);
    detuneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts,
        RMWestVoiceParameters::ID::detuneCents,
        detuneSlider);

    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.setColour(juce::Label::textColourId, juce::Colour::fromRGB(167, 240, 229));
    mixLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mixLabel);

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
    mixLabel.setBounds(5, 29, 60, 18);
    mixSlider.setBounds(5, 48, 62, 52);
    detuneLabel.setBounds(72, 29, 70, 18);
    detuneSlider.setBounds(70, 48, 72, 52);
}
