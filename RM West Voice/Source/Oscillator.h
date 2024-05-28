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
            oscillator.initialise([this](float x) { return generateSaw(x, phaseIncrement); }, 512);
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
        phaseIncrement = frequency / sampleRate;
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
            setFrequency(currentFrequency, true);
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
            setFrequency(currentFrequency, true);
            currentStep++;
        }

        phase += phaseIncrement;
        if (phase >= 2.0f * juce::MathConstants<float>::pi)
            phase -= 2.0f * juce::MathConstants<float>::pi;

        return oscillator.processSample(phase);
    }

private:
    static float generateSaw(float phase, float dt)
    {
        if (dt == 0.0f)
            dt = 1.0e-7f; // Small value to avoid division by zero

        float t = phase / (2.0f * juce::MathConstants<float>::pi);
        float value = 2.0f * t - 1.0f;
        value -= polyBlep(t, dt);

        return value;
    }

    static float generateTriangle(float phase)
    {
        float value = 2.0f * std::abs(2.0f * (phase / (2.0f * juce::MathConstants<float>::pi) - std::floor(0.5f + phase / (2.0f * juce::MathConstants<float>::pi)))) - 1.0f;
        return value;
    }

    static float polyBlep(float t, float dt)
    {
        if (dt == 0.0f)
            dt = 1.0e-7f; // Small value to avoid division by zero

        if (t < dt) // t is the phase value
        {
            t /= dt;
            return t + t - t * t - 1.0f;
        }
        else if (t > 1.0f - dt)
        {
            t = (t - 1.0f) / dt;
            return t * t + t + t + 1.0f;
        }
        else
        {
            return 0.0f;
        }
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
    float phaseIncrement; // Added this to pass it to the generateSaw method
};
