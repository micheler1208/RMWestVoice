#pragma once

#include <JuceHeader.h>

#include "../Engine/FilterModulationState.h"

class LeadFilterData
{
public:
    struct Parameters
    {
        float cutoffHz { 4800.0f };
        float resonance { 0.25f };
        float drive { 1.35f };
        float keyTracking { 0.35f };
        float envelopeAmountOctaves { 0.75f };

        float envelopeAttack { 0.1f };
        float envelopeDecay { 1.0f };
        float envelopeSustain { 0.8f };
        float envelopeRelease { 0.2f };
    };

    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void reset();

    void updateParameters(const Parameters& newParameters);
    void process(juce::AudioBuffer<float>& buffer);

    void noteStarted(int midiNoteNumber, bool isLegatoTransition);
    void noteStopped();

private:
    class SampleLadderFilter final : public juce::dsp::LadderFilter<float>
    {
    public:
        void beginSample() noexcept { updateSmoothers(); }
        float processSingleSample(float input, std::size_t channel) noexcept { return processSample(input, channel); }
    };

    SampleLadderFilter ladderFilter;
    juce::ADSR filterEnvelope;
    juce::ADSR::Parameters envelopeParameters;
    RMWestVoice::FilterModulationState modulationState;

    bool isPrepared { false };
};
