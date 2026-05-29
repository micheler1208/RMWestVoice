/*
  ==============================================================================

    OscData.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class OscData : public juce::dsp::Oscillator<float>
{
public:
    void prepareToPlay (juce::dsp::ProcessSpec& spec);
    void setWaveType (int waveType);
    void setWaveFrequency (const int midiNoteNumber);
    void getNextAudioBlock (juce::dsp::AudioBlock<float>& block);
    void updateFm (const float freq, const float depth);
    void setDetuneCents(float cents);
    void prepareLFO(double playbackSampleRate, int samplesPerBlock, int numChannels);
    
private:
    void processFmOsc (juce::dsp::AudioBlock<float>& block);
    float applyDetune(float sourceFrequency) const;
    
    juce::dsp::Oscillator<float> fmOsc { [](float x) { return std::sin (x); } };
    float fmMod { 0.0f };
    float fmDepth { 0.0f };
    int lastMidiNote { 0 };

    float detuneCents = 0.0f;
    
    juce::dsp::Oscillator<float> lfo { [](float x) { return std::sin(x); } };   
    float lfoMod = 0.0f;
    
};
