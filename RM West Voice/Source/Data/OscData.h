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
    void setWaveType (const bool day);
    void setWaveFrequency (const int midiNoteNumber);
    void getNextAudioBlock (juce::dsp::AudioBlock<float>& block);
    void updateFm (const float freq, const float depth);
    void setDetune(bool isDetuned);
    void OscData::prepareLFO(double sampleRate, int samplesPerBlock, int numChannels);
    
private:
    void processFmOsc (juce::dsp::AudioBlock<float>& block);
    
    juce::dsp::Oscillator<float> fmOsc { [](float x) { return std::sin (x); } };
    float fmMod { 0.0f };
    float fmDepth { 0.0f };
    int lastMidiNote { 0 };

    float detune = 0.06f;
    bool detuneActive = false;
    
    juce::dsp::Oscillator<float> lfo { [](float x) { return std::sin(x); } };   
    float lfoMod = 0.0f;
    
};