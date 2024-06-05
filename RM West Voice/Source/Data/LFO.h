#pragma once
#include <JuceHeader.h>

class LFO
{
public:
    LFO() {}

    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
    }

    void setFrequency(float newFrequency, double sampleRate)
    {
        frequency = newFrequency;
        increment = (2.0 * juce::MathConstants<double>::pi * frequency) / sampleRate;
    }

    float getNextSample()
    {
        auto currentSample = std::sin(phase);
        phase += increment;
        if (phase >= (2.0 * juce::MathConstants<double>::pi))
            phase -= (2.0 * juce::MathConstants<double>::pi);
        return currentSample;
    }

    void reset()
    {
        phase = 0.0;
    }

private:
    double phase = 0.0;
    double increment = 0.0;
    float frequency = 1.0f;
    double sampleRate = 44100.0;
};
