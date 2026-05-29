/*
  ==============================================================================

    OscData.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "../Engine/GlideState.h"
#include "../Engine/PitchModulationState.h"

class OscData : public juce::dsp::Oscillator<float>
{
public:
    void prepareToPlay (juce::dsp::ProcessSpec& spec);
    void reset();
    void setWaveType (int waveType);
    void setOscMix(float mix);
    void setWaveFrequency (int midiNoteNumber);
    void setWaveFrequency (int midiNoteNumber, bool isLegatoTransition);
    void getNextAudioBlock (juce::dsp::AudioBlock<float>& block);
    void setDetuneCents(float cents);
    void setGlideMode(RMWestVoice::GlideState::Mode mode);
    void setGlideTimeSecondsPerOctave(float secondsPerOctave);
    void setPitchBendRangeSemitones(float semitones);
    void setPitchWheel(float normalizedBipolar);
    void setModWheel(float normalized);
    void setAftertouch(float normalized);
    void setVibratoDepthCents(float cents);
    void setVibratoRateHz(float hz);
    void setVibratoFadeSeconds(float seconds);
    void setVibratoAftertouchAmount(float normalized);
    void noteStarted(bool isLegatoTransition);
    void noteStopped();
    
private:
    static float sawWave(float phase) noexcept;
    static float triangleWave(float phase) noexcept;
    static float pulseWave(float phase) noexcept;
    static float applyDetune(float sourceFrequency, float cents) noexcept;
    
    juce::dsp::Oscillator<float> secondaryOsc;
    int currentWaveType { -1 };
    int lastMidiNote { 0 };

    float oscMix = 0.35f;
    float detuneCents = 0.0f;
    RMWestVoice::GlideState glideState;
    RMWestVoice::PitchModulationState pitchModulation;
};
