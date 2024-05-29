#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "LFO.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    SynthVoice()
    {
        // Initialize the oscillator with a sawtooth waveform
        //oscillator.initialise([](float x) { return generateSaw(x); }, 512);
    }

    bool canPlaySound(juce::SynthesiserSound* sound) override
    {
    }


    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
   
    }

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
    }

    void prepare(const juce::dsp::ProcessSpec& spec)
    {
    }
    
    /*
    void updateParameters(const juce::AudioProcessorValueTreeState& apvts)
    {
        // Update ADSR parameters
        adsrParams.attack = apvts.getRawParameterValue("ATTACK")->load() / 1000.0f;
        adsrParams.decay = apvts.getRawParameterValue("DECAY")->load() / 1000.0f;
        adsrParams.sustain = juce::Decibels::decibelsToGain(apvts.getRawParameterValue("SUSTAIN")->load());
        adsrParams.release = apvts.getRawParameterValue("RELEASE")->load() / 1000.0f;
        adsr.setParameters(adsrParams);

        // Update LFO parameters
        float lfoRate = apvts.getRawParameterValue("LFO_RATE")->load();
        lfo.setFrequency(lfoRate, sampleRate);

        // Update filter cutoff frequency
        float cutoff = apvts.getRawParameterValue("CUTOFF")->load();
        float lfoValue = lfo.getNextSample() * 500.0f;
        cutoff += lfoValue;
        cutoff = std::fmax(20.0f, std::fmin(cutoff, sampleRate * 0.5f - 1.0f));
        filter.setCutoffFrequencyHz(cutoff);

        // Update volume
        volume = apvts.getRawParameterValue("VOLUME")->load();
    }*/

private:
    





    /*static float generateSaw(float phase)
    {
        return 2.0f * (phase / (2.0f * juce::MathConstants<float>::pi)) - 1.0f;
    }

    void updatePhaseIncrement()
    {
        phaseIncrement = (2.0 * juce::MathConstants<double>::pi * currentFrequency) / sampleRate;
        jassert(phaseIncrement >= 0);
    }

    juce::dsp::Oscillator<float> oscillator { [] (float x) { return 2.0f * (x / juce::MathConstants<float>::twoPi) - 1.0f; } };
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    LFO lfo;
    juce::dsp::LadderFilter<float> filter;

    int currentMidiNoteNumber = -1;
    float currentFrequency = 0.0f;
    double sampleRate = 44100.0;
    double phaseIncrement = 0.0;
    double phase = 0.0;
    float volume = 1.0f;

    // Glide properties
    bool glideEnabled = true;
    int currentStep = 0;
    const int totalSteps = 55; // Steps for portamento (e.g., 55ms)*/
};
