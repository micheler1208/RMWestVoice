/*
  ==============================================================================

    CustomSlider.h
    Author:  micheler1208

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>

class CustomSlider : public juce::Slider
{
public:
    CustomSlider()
    {
        setNumDecimalPlacesToDisplay(2);
    }

    void setDecimalPlaces(int places)
    {
        numDecimalPlaces = places;
        setNumDecimalPlacesToDisplay(places);
    }

protected:
    juce::String getTextFromValue(double value) override
    {
        juce::String suffix;
        juce::String textValue;

        if (getName() == "Attack" || getName() == "Decay" || getName() == "Release")
        {
            suffix = " ms";
            textValue = juce::String(value, numDecimalPlaces);
        }
        else if (getName() == "Sustain" || getName() == "Volume")
        {
            suffix = " %";
            textValue = juce::String(value * 100.0, 0);
        }
        else if (getName() == "Cutoff")
        {
            suffix = " Hz";
            textValue = juce::String(value, 0);  // No decimal places for Cutoff
        }

        return textValue + suffix;
    }

    double getValueFromText(const juce::String& text) override
    {
        auto valueStr = text.upToFirstOccurrenceOf(" ", false, false);
        auto value = valueStr.getDoubleValue();

        if (getName() == "Sustain" || getName() == "Volume")
        {
            value /= 100.0;
        }

        return value;
    }

private:
    int numDecimalPlaces = 2;
};
