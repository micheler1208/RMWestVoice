#pragma once

#include <JuceHeader.h>

#include "../Engine/PostVoiceFxState.h"

class PostVoiceFxData
{
public:
    struct Parameters
    {
        float width { 0.18f };
        float delayMix { 0.0f };
        float delayTimeSeconds { 0.28f };
        float delayFeedback { 0.25f };
        float reverbMix { 0.0f };
        float reverbSize { 0.35f };
        float reverbDamping { 0.45f };
    };

    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void reset();

    void updateParameters(const Parameters& newParameters);
    void process(juce::AudioBuffer<float>& buffer);

private:
    void processWidth(juce::AudioBuffer<float>& buffer, const RMWestVoice::PostVoiceFxSettings& settings);
    void processDelay(juce::AudioBuffer<float>& buffer, const RMWestVoice::PostVoiceFxSettings& settings);
    void processReverb(juce::AudioBuffer<float>& buffer, const RMWestVoice::PostVoiceFxSettings& settings);

    static int getReadPosition(int writePosition, int delaySamples, int bufferLength) noexcept;

    RMWestVoice::PostVoiceFxState state;
    juce::AudioBuffer<float> widthBuffer;
    juce::AudioBuffer<float> delayBuffer;
    juce::Reverb reverb;

    int widthWritePosition { 0 };
    int delayWritePosition { 0 };
    bool isPrepared { false };
};
