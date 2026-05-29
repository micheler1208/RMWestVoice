#include "PitchModulationState.h"

#include <algorithm>
#include <cmath>

namespace RMWestVoice
{
namespace
{
constexpr auto twoPi = 6.28318530717958647692f;
constexpr auto pitchWheelSmoothingSeconds = 0.015f;
}

void PitchModulationState::prepare(double newSampleRate) noexcept
{
    sampleRate = newSampleRate > 0.0 ? newSampleRate : 44100.0;
    updatePitchWheelSmoothing();
}

void PitchModulationState::reset() noexcept
{
    targetPitchWheel = 0.0f;
    currentPitchWheel = 0.0f;
    currentPitchBendSemitones = 0.0f;
    modWheel = 0.0f;
    aftertouch = 0.0f;
    vibratoFadeLevel = 0.0f;
    vibratoPhaseRadians = 0.0f;
    currentVibratoCents = 0.0f;
    noteIsActive = false;
}

void PitchModulationState::setPitchBendRangeSemitones(float semitones) noexcept
{
    pitchBendRangeSemitones = clamp(semitones, 1.0f, 24.0f);
}

void PitchModulationState::setPitchWheel(float normalizedBipolar) noexcept
{
    targetPitchWheel = clamp(normalizedBipolar, -1.0f, 1.0f);
}

void PitchModulationState::setModWheel(float normalized) noexcept
{
    modWheel = clamp(normalized, 0.0f, 1.0f);
}

void PitchModulationState::setAftertouch(float normalized) noexcept
{
    aftertouch = clamp(normalized, 0.0f, 1.0f);
}

void PitchModulationState::setVibratoDepthCents(float cents) noexcept
{
    vibratoDepthCents = clamp(cents, 0.0f, 100.0f);
}

void PitchModulationState::setVibratoRateHz(float hz) noexcept
{
    vibratoRateHz = clamp(hz, 0.1f, 12.0f);
}

void PitchModulationState::setVibratoFadeSeconds(float seconds) noexcept
{
    vibratoFadeSeconds = clamp(seconds, 0.0f, 2.0f);
}

void PitchModulationState::setVibratoAftertouchAmount(float normalized) noexcept
{
    vibratoAftertouchAmount = clamp(normalized, 0.0f, 1.0f);
}

void PitchModulationState::noteStarted(bool isLegato) noexcept
{
    noteIsActive = true;

    if (! isLegato)
    {
        vibratoFadeLevel = vibratoFadeSeconds <= 0.0f ? 1.0f : 0.0f;
        vibratoPhaseRadians = 0.0f;
    }
}

void PitchModulationState::noteStopped() noexcept
{
    noteIsActive = false;
    aftertouch = 0.0f;
}

float PitchModulationState::getNextPitchRatio() noexcept
{
    currentPitchWheel += (targetPitchWheel - currentPitchWheel) * (1.0f - pitchWheelSmoothingCoefficient);
    currentPitchBendSemitones = currentPitchWheel * pitchBendRangeSemitones;

    if (noteIsActive)
    {
        if (vibratoFadeSeconds <= 0.0f)
            vibratoFadeLevel = 1.0f;
        else
            vibratoFadeLevel = clamp(vibratoFadeLevel + static_cast<float>(1.0 / (vibratoFadeSeconds * sampleRate)), 0.0f, 1.0f);
    }

    vibratoPhaseRadians += static_cast<float>((twoPi * vibratoRateHz) / sampleRate);

    while (vibratoPhaseRadians >= twoPi)
        vibratoPhaseRadians -= twoPi;

    const auto vibratoSource = clamp(modWheel + (aftertouch * vibratoAftertouchAmount), 0.0f, 1.0f);
    currentVibratoCents = std::sin(vibratoPhaseRadians) * vibratoDepthCents * vibratoSource * vibratoFadeLevel;

    return centsToPitchRatio((currentPitchBendSemitones * 100.0f) + currentVibratoCents);
}

float PitchModulationState::clamp(float value, float low, float high) noexcept
{
    return std::min(std::max(value, low), high);
}

float PitchModulationState::centsToPitchRatio(float cents) noexcept
{
    return std::pow(2.0f, cents / 1200.0f);
}

float PitchModulationState::calculateSmoothingCoefficient(double sourceSampleRate, float seconds) noexcept
{
    if (sourceSampleRate <= 0.0 || seconds <= 0.0f)
        return 0.0f;

    return std::exp(static_cast<float>(-1.0 / (seconds * sourceSampleRate)));
}

void PitchModulationState::updatePitchWheelSmoothing() noexcept
{
    pitchWheelSmoothingCoefficient = calculateSmoothingCoefficient(sampleRate, pitchWheelSmoothingSeconds);
}
} // namespace RMWestVoice
