/*
  ==============================================================================

    OscData.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "../Engine/GlideState.h"

class OscData : public juce::dsp::Oscillator<float>
{
public:
    void prepareToPlay (juce::dsp::ProcessSpec& spec);
    void reset();
    void setWaveType (int waveType);
    void setWaveFrequency (int midiNoteNumber);
    void setWaveFrequency (int midiNoteNumber, bool isLegatoTransition);
    void getNextAudioBlock (juce::dsp::AudioBlock<float>& block);
    void updateFm (const float freq, const float depth);
    void setDetuneCents(float cents);
    void setGlideMode(RMWestVoice::GlideState::Mode mode);
    void setGlideTimeSecondsPerOctave(float secondsPerOctave);
    void prepareLFO(double playbackSampleRate, int samplesPerBlock, int numChannels);
    
private:
    void processFmOsc (juce::dsp::AudioBlock<float>& block);
    float applyDetune(float sourceFrequency) const;
    
    juce::dsp::Oscillator<float> fmOsc { [](float x) { return std::sin (x); } };
    float fmMod { 0.0f };
    float fmDepth { 0.0f };
    int lastMidiNote { 0 };

    float detuneCents = 0.0f;
    RMWestVoice::GlideState glideState;
    
    juce::dsp::Oscillator<float> lfo { [](float x) { return std::sin(x); } };   
    float lfoMod = 0.0f;
    
};
