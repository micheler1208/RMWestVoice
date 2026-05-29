#include "PostVoiceFxState.h"

#include <algorithm>
#include <cmath>

namespace RMWestVoice
{
void PostVoiceFxState::prepare(double newSampleRate) noexcept
{
    sampleRate = newSampleRate > 0.0 ? newSampleRate : 44100.0;
    updateDerivedTiming();
}

void PostVoiceFxState::setWidth(float normalized) noexcept
{
    settings.width = clamp(normalized, 0.0f, 1.0f);
    updateDerivedTiming();
}

void PostVoiceFxState::setDelayMix(float normalized) noexcept
{
    settings.delayMix = clamp(normalized, 0.0f, 0.45f);
}

void PostVoiceFxState::setDelayTimeSeconds(float seconds) noexcept
{
    settings.delayTimeSeconds = clamp(seconds, 0.05f, 0.75f);
    updateDerivedTiming();
}

void PostVoiceFxState::setDelayFeedback(float normalized) noexcept
{
    settings.delayFeedback = clamp(normalized, 0.0f, 0.75f);
}

void PostVoiceFxState::setReverbMix(float normalized) noexcept
{
    settings.reverbMix = clamp(normalized, 0.0f, 0.35f);
}

void PostVoiceFxState::setReverbSize(float normalized) noexcept
{
    settings.reverbSize = clamp(normalized, 0.1f, 0.9f);
}

void PostVoiceFxState::setReverbDamping(float normalized) noexcept
{
    settings.reverbDamping = clamp(normalized, 0.0f, 1.0f);
}

float PostVoiceFxState::clamp(float value, float low, float high) noexcept
{
    return std::min(std::max(value, low), high);
}

void PostVoiceFxState::updateDerivedTiming() noexcept
{
    settings.delaySamples = std::max(1, static_cast<int>(std::lround(settings.delayTimeSeconds * sampleRate)));

    if (settings.width <= 0.0f)
    {
        settings.doublerDelaySamples = 0;
        return;
    }

    constexpr auto minimumDoublerDelaySeconds = 0.008f;
    constexpr auto widthDelayRangeSeconds = 0.010f;
    const auto doublerDelaySeconds = minimumDoublerDelaySeconds + (settings.width * widthDelayRangeSeconds);

    settings.doublerDelaySamples = std::max(1, static_cast<int>(std::lround(doublerDelaySeconds * sampleRate)));
}
} // namespace RMWestVoice
