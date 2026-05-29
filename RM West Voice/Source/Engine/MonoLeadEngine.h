/*
  ==============================================================================

    MonoLeadEngine.h
    Author:  micheler1208

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../Data/AdsrData.h"
#include "../Data/LeadFilterData.h"
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
        float oscMix { 0.35f };
        float detuneCents { 0.0f };
        GlideState::Mode glideMode { GlideState::Mode::autoLegato };
        float glideTimeSecondsPerOctave { 0.08f };
        float pitchBendRangeSemitones { 12.0f };
        float vibratoDepthCents { 35.0f };
        float vibratoRateHz { 5.5f };
        float vibratoFadeSeconds { 0.15f };
        float vibratoAftertouchAmount { 0.0f };

        float ampAttack { 0.1f };
        float ampDecay { 1.0f };
        float ampSustain { 0.8f };
        float ampRelease { 0.2f };

        float filterCutoffHz { 4800.0f };
        float filterResonance { 0.25f };
        float filterDrive { 1.35f };
        float filterKeyTracking { 0.35f };
        float filterEnvelopeAmountOctaves { 0.75f };
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
    void handlePitchWheel(const juce::MidiMessage& message);
    void handleModWheel(const juce::MidiMessage& message);
    void handleAftertouch(const juce::MidiMessage& message);
    void startActiveNote(const MonoNoteStack::UpdateResult& noteUpdate);
    void stopOrFallbackFromActiveNote(const MonoNoteStack::UpdateResult& noteUpdate);

    MonoNoteStack noteStack;
    Parameters parameters;

    AdsrData adsr;
    OscData osc;
    LeadFilterData leadFilter;
    juce::dsp::Gain<float> gain;
    juce::AudioBuffer<float> synthBuffer;

    bool isPrepared { false };
};
} // namespace RMWestVoice
