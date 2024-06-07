/*
  ==============================================================================

    CustomLookAndFeelCyan.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomLookAndFeelDetune : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeelDetune()
    {
        customFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::heaters_ttf, BinaryData::heaters_ttfSize));
    }

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
        bool isMouseOverButton, bool isButtonDown) override
    {
        auto buttonArea = button.getLocalBounds();
        auto edge = 4;

        buttonArea.reduce(edge, edge);

        auto baseColour = backgroundColour.withMultipliedSaturation(button.getToggleState() ? 1.3f : 0.9f)
            .withMultipliedAlpha(button.isEnabled() ? 0.9f : 0.5f);

        if (isButtonDown || isMouseOverButton)
            baseColour = baseColour.contrasting(0.2f);

        g.setColour(baseColour);
        g.fillRoundedRectangle(buttonArea.toFloat(), 6.0f);

        g.setColour(button.getToggleState() ? juce::Colour::fromRGB(2, 141, 180) : juce::Colour::fromRGB(167, 240, 229));
        g.drawRoundedRectangle(buttonArea.toFloat(), 6.0f, 1.0f);
    }

    void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool isButtonDown) override
    {
        auto buttonFont = customFont.withHeight(24.0f);
        g.setFont(buttonFont);

        g.setColour(button.findColour(button.getToggleState() ? juce::TextButton::textColourOnId
            : juce::TextButton::textColourOffId));
        auto textBounds = button.getLocalBounds();
        g.drawFittedText(button.getButtonText(), textBounds, juce::Justification::centred, 2);
    }

private:
    juce::Font customFont;
};
