/*
  ==============================================================================

    OscComponent.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================

// COSTRUCTOR
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts)
{
    dayButton.setButtonText("Day");
    nightButton.setButtonText("Night");
    detuneButton.setButtonText("IN TUNE");

    addAndMakeVisible(dayButton);
    addAndMakeVisible(nightButton);
    addAndMakeVisible(detuneButton);

    dayButton.setRadioGroupId(1);
    nightButton.setRadioGroupId(1);

    dayButton.setToggleState(true, juce::dontSendNotification);

    dayButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "DAY", dayButton);
    nightButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts,"NIGHT", nightButton);
    detuneButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "DETUNE", detuneButton);

    detuneButton.setLookAndFeel(&customLookAndFeelCyan);
    detuneButton.setClickingTogglesState(true);

    // Listener per aggiornare il testo del pulsante e i colori
    detuneButton.onClick = [this]()
        {
            if (detuneButton.getToggleState())
            {
                detuneButton.setButtonText("DETUNED");
                detuneButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
                detuneButton.setColour(juce::TextButton::textColourOffId, juce::Colour::fromRGB(2, 141, 180));
                detuneButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
                detuneButton.setColour(juce::TextButton::textColourOnId, juce::Colour::fromRGB(2, 141, 180));
            }
            else
            {
                detuneButton.setButtonText("IN TUNE");
                detuneButton.setColour(juce::TextButton::buttonColourId, juce::Colours::transparentBlack);
                detuneButton.setColour(juce::TextButton::textColourOffId, juce::Colour::fromRGB(167, 240, 229));
                detuneButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::transparentBlack);
                detuneButton.setColour(juce::TextButton::textColourOnId, juce::Colour::fromRGB(167, 240, 229));
            }
        };

    // Initialize detune button state
    detuneButton.onClick();
}

// DESTRUCTOR
OscComponent::~OscComponent()
{
    detuneButton.setLookAndFeel(nullptr);
}

// PAINT
void OscComponent::paint (juce::Graphics& g)
{
}

// RESIZED
void OscComponent::resized()
{
    dayButton.setBounds(0, 33, 90, 30);
    nightButton.setBounds(66, 33, 90, 30);
    detuneButton.setBounds(18, 0, 90, 30);
}
