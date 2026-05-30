#include "PostVoiceFxData.h"

#include <cmath>

namespace
{
constexpr auto maxDelaySeconds = 0.85;
constexpr auto maxWidthDelaySeconds = 0.03;
constexpr auto widthSideGain = 0.18f;

int getSafeChannelCount(int outputChannels) noexcept
{
    return juce::jmax(1, outputChannels);
}
} // namespace

void PostVoiceFxData::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    const auto safeSampleRate = sampleRate > 0.0 ? sampleRate : 44100.0;
    const auto safeBlockSize = juce::jmax(1, samplesPerBlock);
    const auto safeChannelCount = getSafeChannelCount(outputChannels);

    state.prepare(safeSampleRate);

    const auto widthBufferSamples = juce::jmax(
        1,
        static_cast<int>(std::ceil(maxWidthDelaySeconds * safeSampleRate)) + safeBlockSize + 1);
    const auto delayBufferSamples = juce::jmax(
        1,
        static_cast<int>(std::ceil(maxDelaySeconds * safeSampleRate)) + safeBlockSize + 1);

    widthBuffer.setSize(safeChannelCount, widthBufferSamples);
    delayBuffer.setSize(safeChannelCount, delayBufferSamples);

    reverb.setSampleRate(safeSampleRate);
    reset();

    isPrepared = true;
}

void PostVoiceFxData::reset()
{
    widthWritePosition = 0;
    delayWritePosition = 0;
    widthBuffer.clear();
    delayBuffer.clear();
    reverb.reset();
}

void PostVoiceFxData::updateParameters(const Parameters& newParameters)
{
    state.setWidth(newParameters.width);
    state.setDelayMix(newParameters.delayMix);
    state.setDelayTimeSeconds(newParameters.delayTimeSeconds);
    state.setDelayFeedback(newParameters.delayFeedback);
    state.setReverbMix(newParameters.reverbMix);
    state.setReverbSize(newParameters.reverbSize);
    state.setReverbDamping(newParameters.reverbDamping);

    const auto& settings = state.getSettings();

    juce::Reverb::Parameters reverbParameters;
    reverbParameters.roomSize = settings.reverbSize;
    reverbParameters.damping = settings.reverbDamping;
    reverbParameters.wetLevel = settings.reverbMix / 3.0f;
    reverbParameters.dryLevel = 1.0f;
    reverbParameters.width = 1.0f;
    reverbParameters.freezeMode = 0.0f;
    reverb.setParameters(reverbParameters);
}

void PostVoiceFxData::process(juce::AudioBuffer<float>& buffer)
{
    jassert(isPrepared);

    if (buffer.getNumSamples() <= 0)
        return;

    const auto& settings = state.getSettings();

    processWidth(buffer, settings);
    processDelay(buffer, settings);
    processReverb(buffer, settings);
}

void PostVoiceFxData::processWidth(juce::AudioBuffer<float>& buffer, const RMWestVoice::PostVoiceFxSettings& settings)
{
    if (settings.width <= 0.0f || settings.doublerDelaySamples <= 0 || buffer.getNumChannels() < 2)
        return;

    const auto bufferLength = widthBuffer.getNumSamples();
    const auto delaySamples = juce::jmin(settings.doublerDelaySamples, bufferLength - 1);
    const auto numSamples = buffer.getNumSamples();

    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getWritePointer(1);

    for (int sample = 0; sample < numSamples; ++sample)
    {
        const auto readPosition = getReadPosition(widthWritePosition, delaySamples, bufferLength);
        const auto dryLeft = left[sample];
        const auto dryRight = right[sample];
        const auto delayedLeft = widthBuffer.getSample(0, readPosition);
        const auto delayedRight = widthBuffer.getSample(1, readPosition);

        widthBuffer.setSample(0, widthWritePosition, dryLeft);
        widthBuffer.setSample(1, widthWritePosition, dryRight);

        const auto dryMid = (dryLeft + dryRight) * 0.5f;
        const auto delayedMid = (delayedLeft + delayedRight) * 0.5f;
        const auto side = (dryMid - delayedMid) * settings.width * widthSideGain;

        left[sample] = dryLeft + side;
        right[sample] = dryRight - side;

        widthWritePosition = (widthWritePosition + 1) % bufferLength;
    }
}

void PostVoiceFxData::processDelay(juce::AudioBuffer<float>& buffer, const RMWestVoice::PostVoiceFxSettings& settings)
{
    if (settings.delayMix <= 0.0f && settings.delayFeedback <= 0.0f)
        return;

    const auto bufferLength = delayBuffer.getNumSamples();
    const auto numSamples = buffer.getNumSamples();
    const auto numChannels = juce::jmin(buffer.getNumChannels(), delayBuffer.getNumChannels());
    const auto baseDelaySamples = juce::jlimit(1, bufferLength - 1, settings.delaySamples);

    for (int sample = 0; sample < numSamples; ++sample)
    {
        for (int channel = 0; channel < numChannels; ++channel)
        {
            const auto channelDelaySamples = channel == 1
                ? juce::jmin(bufferLength - 1, static_cast<int>(std::lround(static_cast<float>(baseDelaySamples) * 1.125f)))
                : baseDelaySamples;
            const auto readPosition = getReadPosition(delayWritePosition, channelDelaySamples, bufferLength);
            const auto delayedSample = delayBuffer.getSample(channel, readPosition);

            auto* channelData = buffer.getWritePointer(channel);
            const auto drySample = channelData[sample];

            delayBuffer.setSample(channel, delayWritePosition, drySample + (delayedSample * settings.delayFeedback));
            channelData[sample] = drySample + (delayedSample * settings.delayMix);
        }

        delayWritePosition = (delayWritePosition + 1) % bufferLength;
    }
}

void PostVoiceFxData::processReverb(juce::AudioBuffer<float>& buffer, const RMWestVoice::PostVoiceFxSettings& settings)
{
    if (settings.reverbMix <= 0.0f)
        return;

    if (buffer.getNumChannels() >= 2)
    {
        reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
        return;
    }

    if (buffer.getNumChannels() == 1)
        reverb.processMono(buffer.getWritePointer(0), buffer.getNumSamples());
}

int PostVoiceFxData::getReadPosition(int writePosition, int delaySamples, int bufferLength) noexcept
{
    return (writePosition + bufferLength - delaySamples) % bufferLength;
}
