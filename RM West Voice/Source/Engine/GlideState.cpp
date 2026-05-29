/*
  ==============================================================================

    GlideState.cpp
    Author:  micheler1208

  ==============================================================================
*/

#include "GlideState.h"

#include <algorithm>
#include <cmath>

namespace RMWestVoice
{
void GlideState::prepare(double newSampleRate) noexcept
{
    sampleRate = newSampleRate > 0.0 ? newSampleRate : 44100.0;
    reset();
}

void GlideState::reset() noexcept
{
    currentLog2Frequency = 0.0;
    targetLog2Frequency = 0.0;
    log2Increment = 0.0;
    remainingSamples = 0;
    hasFrequency = false;
}

void GlideState::setTimeSecondsPerOctave(float newTimeSecondsPerOctave) noexcept
{
    timeSecondsPerOctave = std::max(0.0f, newTimeSecondsPerOctave);
}

void GlideState::setCurrentFrequency(float frequencyHz) noexcept
{
    currentLog2Frequency = frequencyToLog2(sanitizeFrequency(frequencyHz));
    targetLog2Frequency = currentLog2Frequency;
    log2Increment = 0.0;
    remainingSamples = 0;
    hasFrequency = true;
}

void GlideState::startTransition(float targetFrequencyHz, bool isLegato) noexcept
{
    const auto targetFrequency = sanitizeFrequency(targetFrequencyHz);
    const auto newTargetLog2Frequency = frequencyToLog2(targetFrequency);

    if (! hasFrequency || ! shouldGlide(isLegato) || timeSecondsPerOctave <= 0.0f)
    {
        setCurrentFrequency(targetFrequency);
        return;
    }

    const auto octaveDistance = std::abs(newTargetLog2Frequency - currentLog2Frequency);
    const auto exactGlideSamples = octaveDistance * timeSecondsPerOctave * sampleRate;
    const auto glideSamples = static_cast<int>(std::ceil(exactGlideSamples - 1.0e-9));

    if (glideSamples <= 1)
    {
        setCurrentFrequency(targetFrequency);
        return;
    }

    targetLog2Frequency = newTargetLog2Frequency;
    remainingSamples = glideSamples;
    log2Increment = (targetLog2Frequency - currentLog2Frequency) / static_cast<double>(remainingSamples);
}

float GlideState::getNextFrequency() noexcept
{
    if (! hasFrequency)
        return 0.0f;

    if (remainingSamples > 0)
    {
        currentLog2Frequency += log2Increment;
        --remainingSamples;

        if (remainingSamples == 0)
        {
            currentLog2Frequency = targetLog2Frequency;
            log2Increment = 0.0;
        }
    }

    return getCurrentFrequency();
}

float GlideState::getCurrentFrequency() const noexcept
{
    if (! hasFrequency)
        return 0.0f;

    return log2ToFrequency(currentLog2Frequency);
}

float GlideState::sanitizeFrequency(float frequencyHz) noexcept
{
    return std::max(1.0f, frequencyHz);
}

double GlideState::frequencyToLog2(float frequencyHz) noexcept
{
    return std::log2(static_cast<double>(frequencyHz));
}

float GlideState::log2ToFrequency(double log2Frequency) noexcept
{
    return static_cast<float>(std::pow(2.0, log2Frequency));
}

bool GlideState::shouldGlide(bool isLegato) const noexcept
{
    if (mode == Mode::always)
        return true;

    if (mode == Mode::autoLegato)
        return isLegato;

    return false;
}
} // namespace RMWestVoice
