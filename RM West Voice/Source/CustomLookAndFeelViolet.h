#pragma once

#include <JuceHeader.h>

class CustomLookAndFeelViolet : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeelViolet() {}

    // ROTATORY SLIDER
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        const float radius = juce::jmin(width / 2, height / 2) - 4.0f;
        const float centreX = x + width * 0.5f;
        const float centreY = y + height * 0.5f;
        const float rx = centreX - radius;
        const float ry = centreY - radius;
        const float rw = radius * 2.0f;
        const float angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

        // background of the circle
        g.setColour(juce::Colour::fromRGB(1, 0, 57));
        g.fillEllipse(rx, ry, rw, rw);

        // Arc of the circle
        g.setColour(juce::Colour::fromRGB(235, 47, 144));
        juce::Path filledArc;
        filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, rotaryEndAngle, 0.8f);
        g.fillPath(filledArc);

        // Filled Arc
        g.setColour(juce::Colour::fromRGB(69, 3, 111));
        filledArc.clear();
        filledArc.addPieSegment(rx, ry, rw, rw, rotaryStartAngle, angle, 0.8f);
        g.fillPath(filledArc);

        // thumb - Pointer
        juce::Path thumb;
        const float thumbWidth = 4.0f;
        thumb.addRectangle(-thumbWidth / 2, -radius, thumbWidth, radius * 0.6f);
        g.setColour(juce::Colour::fromRGB(69, 3, 111));
        g.fillPath(thumb, juce::AffineTransform::rotation(angle).translated(centreX, centreY));
    }
    
};
