/*
  ==============================================================================

    AdsrComponent.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AdsrComponent.h"

//==============================================================================
AdsrComponent::AdsrComponent(juce::AudioProcessorValueTreeState& apvts)
{
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    // Slider di Attack
    attackSlider.setLookAndFeel(&customLookAndFeelYellow);
    attackSlider.setName("Attack");
    attackSlider.setSliderStyle(juce::Slider::Rotary);
    attackSlider.setRange(0.01, 2000.0, 0.01); // Intervallo in ms
    attackSlider.setTextValueSuffix(" ms");
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    attackSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    attackSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(255, 228, 163));
    attackSlider.setNumDecimalPlacesToDisplay(2);
    addAndMakeVisible(&attackSlider);
    attackAttachment = std::make_unique<SliderAttachment>(apvts, "ATTACK", attackSlider);

    // Slider di Decay
    decaySlider.setLookAndFeel(&customLookAndFeelYellow);
    decaySlider.setName("Decay");
    decaySlider.setSliderStyle(juce::Slider::Rotary);
    decaySlider.setRange(0.01, 2000.0, 0.01); // Intervallo in ms
    decaySlider.setTextValueSuffix(" ms");
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    decaySlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    decaySlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(255, 228, 163));
    decaySlider.setNumDecimalPlacesToDisplay(2); // Imposta il numero di cifre decimali da visualizzare
    addAndMakeVisible(&decaySlider);
    decayAttachment = std::make_unique<SliderAttachment>(apvts, "DECAY", decaySlider);

    // Slider di Sustain
    sustainSlider.setLookAndFeel(&customLookAndFeelYellow);
    sustainSlider.setName("Sustain");
    sustainSlider.setSliderStyle(juce::Slider::Rotary);
    sustainSlider.setRange(-5.0, 5.0, 0.1); // Intervallo in dB
    sustainSlider.setTextValueSuffix(" dB");
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    sustainSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    sustainSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(255, 228, 163));
    sustainSlider.setNumDecimalPlacesToDisplay(1); // Imposta il numero di cifre decimali da visualizzare
    addAndMakeVisible(&sustainSlider);
    sustainAttachment = std::make_unique<SliderAttachment>(apvts, "SUSTAIN", sustainSlider);

    // Slider di Release
    releaseSlider.setLookAndFeel(&customLookAndFeelYellow);
    releaseSlider.setName("Release");
    releaseSlider.setSliderStyle(juce::Slider::Rotary);
    releaseSlider.setRange(1.0, 3000.0, 0.01); // Intervallo in ms
    releaseSlider.setTextValueSuffix(" ms");
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 30);
    releaseSlider.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentWhite);
    releaseSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colour::fromRGB(255, 228, 163));
    releaseSlider.setNumDecimalPlacesToDisplay(2); // Imposta il numero di cifre decimali da visualizzare
    addAndMakeVisible(&releaseSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(apvts, "RELEASE", releaseSlider);
}

AdsrComponent::~AdsrComponent()
{
    attackSlider.setLookAndFeel(nullptr);
    decaySlider.setLookAndFeel(nullptr);
    sustainSlider.setLookAndFeel(nullptr);
    releaseSlider.setLookAndFeel(nullptr);
}

void AdsrComponent::paint (juce::Graphics& g)
{
    //g.fillAll (juce::Colours::black);
}

void AdsrComponent::resized()
{
    const int margin = 25;
    const int sliderWidth = 75;
    const int sliderHeight = 75;
    const int initialX = 0;
    const int yPosition = 0;

    attackSlider.setBounds(initialX, yPosition, sliderWidth, sliderHeight);
    decaySlider.setBounds(initialX + sliderWidth + margin, yPosition, sliderWidth, sliderHeight);
    sustainSlider.setBounds(initialX + 2 * (sliderWidth + margin), yPosition, sliderWidth, sliderHeight);
    releaseSlider.setBounds(initialX + 3 * (sliderWidth + margin), yPosition, sliderWidth, sliderHeight);
}
