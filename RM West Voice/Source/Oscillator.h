#pragma once

#include <JuceHeader.h>

class Oscillator
{
public:
    Oscillator()
    {
        setWaveform(Triangle);
        lfo.initialise([](float x) { return std::sin(x); });
    }

    void setWaveform(int waveformType)
    {
        switch (waveformType)
        {
        case Sine:
            oscillator.initialise([](float x) { return std::sin(x); });
            break;
        case Saw:
            oscillator.initialise([](float x) { return x / juce::MathConstants<float>::pi; });
            break;
        case Square:
            oscillator.initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
            break;
        case Triangle:
            oscillator.initialise([](float x) { return x < 0.0f ? 1.0f + x / juce::MathConstants<float>::pi : 1.0f - x / juce::MathConstants<float>::pi; });
            break;
        default:
            jassertfalse; // waveformType not recognized
            break;
        }
    }

    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        oscillator.prepare(spec);
        lfo.prepare(spec);
        lfoBuffer.setSize(1, spec.maximumBlockSize);
    }

    void setFrequency(float frequency) { oscillator.setFrequency(frequency); }
    void setLfoFrequency(float frequency) { lfo.setFrequency(frequency); }

    void process(juce::dsp::AudioBlock<float>& block)
    {
        juce::dsp::ProcessContextReplacing<float> context(block);
        oscillator.process(context);
    }

    void processLFO(juce::dsp::AudioBlock<float>& block)
    {
        juce::dsp::ProcessContextReplacing<float> context(block);
        lfo.process(context);
    }

    float getLfoValue()
    {
        lfoBuffer.clear();
        juce::dsp::AudioBlock<float> lfoBlock(lfoBuffer);
        juce::dsp::ProcessContextReplacing<float> context(lfoBlock);
        lfo.process(context);
        return lfoBlock.getSample(0, 0);
    }

    enum Waveform
    {
        Sine = 0,
        Saw,
        Square,
        Triangle
    };

private:
    juce::dsp::Oscillator<float> oscillator;
    juce::dsp::Oscillator<float> lfo; // LFO for modulation
    juce::AudioBuffer<float> lfoBuffer; // Buffer for LFO processing
};
