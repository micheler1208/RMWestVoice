/*
  ==============================================================================

    OscData.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "../Engine/CharacterState.h"
#include "../Engine/GlideState.h"
#include "../Engine/PitchModulationState.h"

class OscData : public juce::dsp::Oscillator<float>
{
public:
    void prepareToPlay (juce::dsp::ProcessSpec& spec);
    void reset();
    void setWaveType (int waveType);
    void setCharacterMode(int characterMode);
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
    static float hybridCarrierWave(float phase) noexcept;
    static float hybridShimmerWave(float phase) noexcept;
    static float applyDetune(float sourceFrequency, float cents) noexcept;
    static float applyCharacterDrive(float sample, const RMWestVoice::CharacterSettings& settings) noexcept;

    float getCharacterDriftRatio() noexcept;
    void advanceCharacterDrift() noexcept;
    
    juce::dsp::Oscillator<float> secondaryOsc;
    juce::dsp::Oscillator<float> hybridCarrierOsc;
    juce::dsp::Oscillator<float> hybridShimmerOsc;
    RMWestVoice::CharacterSettings characterSettings;

    double sampleRate { 44100.0 };
    float characterDriftPhaseA { 0.0f };
    float characterDriftPhaseB { 0.0f };
    int currentWaveType { -1 };
    int lastMidiNote { 0 };

    float oscMix = 0.35f;
    float detuneCents = 0.0f;
    RMWestVoice::GlideState glideState;
    RMWestVoice::PitchModulationState pitchModulation;
};
