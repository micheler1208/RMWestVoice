#pragma once

#include <JuceHeader.h>
#include <cmath>
#include <array>

class Oscillator
{
public:
    enum Waveform
    {
        Sine = 0,
        Saw,
        Triangle
    };

    Oscillator() : sampleRate(44100.0), phase(0.0), increment(0.0), currentStep(0), totalSteps(55), previousFrequency(0.0f), targetFrequency(0.0f), glideEnabled(false), detune(0.0f)
    {
        setWaveform(Saw); // Default to Saw wave
    }

    void setWaveform(Waveform waveformType)
    {
        currentWaveform = waveformType;
        switch (waveformType)
        {
        case Sine:
            oscillator.initialise([](float x) { return std::sin(x); }, 128);
            break;
        case Saw:
            oscillator.initialise([](float x) { return generateSaw(x); }, 128);
            break;
        case Triangle:
            oscillator.initialise([](float x) { return generateTriangle(x); }, 128);
            break;
        default:
            jassertfalse; // waveformType not recognized
            break;
        }
    }

    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        oscillator.prepare(spec);
    }

    void setFrequency(float frequency, bool immediate = false)
    {
        frequency *= (1.0f + detune); // Apply detune
        if (immediate || !glideEnabled)
        {
            oscillator.setFrequency(frequency);
            currentStep = totalSteps;
        }
        else
        {
            targetFrequency = frequency;
            currentStep = 0;
        }
        previousFrequency = frequency;
    }

    void enableGlide(bool enable)
    {
        glideEnabled = enable;
    }

    void setDetune(float detuneAmount)
    {
        detune = detuneAmount;
    }

    void process(juce::dsp::AudioBlock<float>& block)
    {
        if (currentStep < totalSteps)
        {
            float currentFrequency = juce::jmap(currentStep / (float)totalSteps, previousFrequency, targetFrequency);
            oscillator.setFrequency(currentFrequency);
            currentStep++;
        }

        juce::dsp::ProcessContextReplacing<float> context(block);
        oscillator.process(context);
    }

    float getNextSample()
    {
        if (currentStep < totalSteps)
        {
            float currentFrequency = juce::jmap(currentStep / (float)totalSteps, previousFrequency, targetFrequency);
            oscillator.setFrequency(currentFrequency);
            currentStep++;
        }

        return oscillator.processSample(0.0f);
    }

private:
    static float generateSaw(float phase)
    {
        return 2.0f * (phase / (2.0f * juce::MathConstants<float>::pi) - std::floor(0.5f + phase / (2.0f * juce::MathConstants<float>::pi)));
    }

    static float generateTriangle(float phase)
    {
        float value = 2.0f * std::abs(2.0f * (phase / (2.0f * juce::MathConstants<float>::pi) - std::floor(0.5f + phase / (2.0f * juce::MathConstants<float>::pi)))) - 1.0f;
        return value;
    }

    juce::dsp::Oscillator<float> oscillator;
    double sampleRate;
    double phase;
    double increment;
    Waveform currentWaveform;

    float previousFrequency;
    float targetFrequency;
    int currentStep;
    const int totalSteps; // Steps for portamento (e.g., 55ms)
    bool glideEnabled;
    float detune; // Detune amount
};
