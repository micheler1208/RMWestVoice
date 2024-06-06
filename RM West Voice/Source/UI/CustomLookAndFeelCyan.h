/*
  ==============================================================================

    CustomLookAndFeelCyan.h
    Author:  micheler1208
    
  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomLookAndFeelCyan : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeelCyan() {}

    void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown) override
    {
        auto buttonArea = button.getLocalBounds();
        auto edge = 4;

        buttonArea.reduce (edge, edge);

        auto baseColour = backgroundColour.withMultipliedSaturation (button.getToggleState() ? 1.3f : 0.9f)
                                           .withMultipliedAlpha (button.isEnabled() ? 0.9f : 0.5f);

        if (isButtonDown || isMouseOverButton)
            baseColour = baseColour.contrasting (0.2f);

        g.setColour (baseColour);
        g.fillRect (buttonArea);

        g.setColour (button.getToggleState() ? juce::Colour::fromRGB(2, 141, 180): juce::Colour::fromRGB(167, 240, 229));
        g.drawRect (buttonArea, 1.0f);
    }
};

 