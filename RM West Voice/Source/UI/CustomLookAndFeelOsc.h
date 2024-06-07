/*
  ==============================================================================

    CustomLookAndFeelOsc.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomLookAndFeelOsc : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeelOsc()
    {
        customFont = juce::Font(juce::Typeface::createSystemTypefaceFor(BinaryData::heaters_ttf, BinaryData::heaters_ttfSize));
    }

    void drawButtonBackground(juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
        bool isMouseOverButton, bool isButtonDown) override
    {
        auto buttonArea = button.getLocalBounds();
        auto edge = 4;

        buttonArea.reduce(edge, edge);

        // Set the base color with transparency to keep the background visible
        auto baseColour = backgroundColour.withAlpha(0.0f);

        if (isButtonDown || isMouseOverButton)
            baseColour = baseColour.contrasting(0.2f);

        g.setColour(baseColour);
        g.fillRoundedRectangle(buttonArea.toFloat(), 6.0f);

        g.setColour(button.getToggleState() ? juce::Colour::fromRGB(167, 240, 229) : juce::Colour::fromRGB(2, 141, 180));
        g.drawRoundedRectangle(buttonArea.toFloat(), 6.0f, 1.0f);
    }

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool isMouseOverButton, bool isButtonDown) override
    {
        auto font = customFont.withHeight(28.0f);
        g.setFont(font);

        auto tickWidth = 20.0f;
        auto tickHeight = 20.0f;
        auto edge = 4;
        auto margin = 6; // Adjust this value for the desired space

        auto buttonArea = button.getLocalBounds();
        buttonArea.reduce(edge, edge);

        // Draw the tick box with transparency
        auto tickColour = button.getToggleState() ? juce::Colour::fromRGB(167, 240, 229) : juce::Colour::fromRGB(2, 141, 180);
        g.setColour(tickColour);
        auto tickArea = buttonArea.removeFromLeft(static_cast<int>(tickWidth));
        g.drawRoundedRectangle(tickArea.toFloat(), 6.0f, 1.0f);

        // Draw the tick inside the box
        if (button.getToggleState())
        {
            auto tick = juce::Path();
            tick.startNewSubPath(tickArea.getX() + 4.0f, tickArea.getCentreY());
            tick.lineTo(tickArea.getCentreX() - 2.0f, tickArea.getBottom() - 6.0f);
            tick.lineTo(tickArea.getRight() - 4.0f, tickArea.getY() + 6.0f);

            g.setColour(tickColour);
            g.strokePath(tick, juce::PathStrokeType(2.0f));
        }

        // Draw the text with the specified color
        auto textColour = button.getToggleState() ? juce::Colour::fromRGB(167, 240, 229) : juce::Colour::fromRGB(2, 141, 180);
        g.setColour(textColour);

        // Adjust the text bounds to include the margin
        auto textBounds = buttonArea.reduced(static_cast<int>(+margin), 0);
        g.drawFittedText(button.getButtonText(), textBounds, juce::Justification::centredLeft, 2);
    }

private:
    juce::Font customFont;
};
