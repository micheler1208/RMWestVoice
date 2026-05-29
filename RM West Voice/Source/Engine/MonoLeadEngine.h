/*
  ==============================================================================

    MonoLeadEngine.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Data/AdsrData.h"
#include "../Data/OscData.h"
#include "MonoNoteStack.h"

namespace RMWestVoice
{
class MonoLeadEngine
{
public:
    struct Parameters
    {
        int waveType { 0 };
        float detuneCents { 0.0f };

        float ampAttack { 0.1f };
        float ampDecay { 1.0f };
        float ampSustain { 0.8f };
        float ampRelease { 0.2f };
    };

    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void reset();

    void updateParameters(const Parameters& newParameters);
    void renderNextBlock(
        juce::AudioBuffer<float>& outputBuffer,
        const juce::MidiBuffer& midiMessages,
        int startSample,
        int numSamples);

    MonoNoteStack& getNoteStack() noexcept { return noteStack; }
    const MonoNoteStack& getNoteStack() const noexcept { return noteStack; }

private:
    void renderAudioRange(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples);
    void handleMidiMessage(const juce::MidiMessage& message);
    void startActiveNote(const MonoNoteStack::UpdateResult& noteUpdate);
    void stopOrFallbackFromActiveNote(const MonoNoteStack::UpdateResult& noteUpdate);

    MonoNoteStack noteStack;
    Parameters parameters;

    AdsrData adsr;
    OscData osc;
    juce::dsp::Gain<float> gain;
    juce::AudioBuffer<float> synthBuffer;

    bool isPrepared { false };
};
} // namespace RMWestVoice
